#include "LSM6DS3IMU.h"
#include "DekiI2C.h"  // from deki-i2c
#include <deki/LogSystem.h>

namespace DekiImu
{

namespace
{
// LSM6DS3 register map (subset — see ST AN4650)
constexpr uint8_t kRegWhoAmI = 0x0F;
constexpr uint8_t kRegCtrl1Xl = 0x10;  // accel control
constexpr uint8_t kRegCtrl2G = 0x11;   // gyro control
constexpr uint8_t kRegCtrl3C = 0x12;   // common control (BDU, etc.)
constexpr uint8_t kRegCtrl10C = 0x19;  // embedded-function enables
constexpr uint8_t kRegTapCfg = 0x58;   // pedometer/tap/tilt enable
constexpr uint8_t kRegOutGL = 0x22;    // gyro  X/Y/Z LSB/MSB (6 bytes)
constexpr uint8_t kRegOutXlL = 0x28;   // accel X/Y/Z LSB/MSB (6 bytes)
constexpr uint8_t kRegStepCounterL = 0x4B;

// Expected WHO_AM_I values for LSM6DS3 family
constexpr uint8_t kWhoAmILsm6ds3 = 0x69;
constexpr uint8_t kWhoAmILsm6ds3tr = 0x69;
constexpr uint8_t kWhoAmILsm6ds3C = 0x6A;  // LSM6DS3-C variant

// CTRL1_XL: 0x60 = 416 Hz ODR + ±2g + 400Hz analog BW
constexpr uint8_t kCtrl1Xl416Hz2G = 0x60;
// CTRL2_G : 0x60 = 416 Hz ODR + ±245 dps
constexpr uint8_t kCtrl2G416Hz245 = 0x60;
// CTRL3_C : BDU (block data update)
constexpr uint8_t kCtrl3CBdu = 0x44;

// Pedometer enable pattern:
//   TAP_CFG bit 7 (INTERRUPTS_ENABLE) + bit 6 (PEDO_RST_STEP)... we set TAP_CFG = 0x40 = enable funcs
//   CTRL10_C bit 4 (PEDO_EN) + bit 2 (FUNC_EN)
constexpr uint8_t kTapCfgPedoEn = 0x40;
constexpr uint8_t kCtrl10CPedoFun = 0x14;

inline int16_t ToS16(uint8_t lo, uint8_t hi)
{
    return (int16_t)((uint16_t)lo | ((uint16_t)hi << 8));
}
}  // namespace

void LSM6DS3IMU::Configure(const Deki::PackageConfig& config)
{
    m_BusPort = config.GetInt("i2cPort", 0);
    m_I2cAddr = (uint8_t)config.GetInt("i2cAddress", 0x6A);
    m_Pedometer = config.GetBool("enablePedometer", true);
}

bool LSM6DS3IMU::Initialize()
{
    m_Bus = DekiI2c::DekiI2C::GetBus(m_BusPort);
    if (!m_Bus)
    {
        m_LastError = "LSM6DS3: no I2C bus registered on requested port (add an I2C Bus component)";
        m_State = Deki::PackageState::Error;
        return false;
    }

    // Probe + WHO_AM_I check
    if (!m_Bus->Probe(m_I2cAddr))
    {
        DEKI_LOG_WARNING("LSM6DS3IMU: chip did not ACK at 0x%02X on I2C port %d", m_I2cAddr, m_BusPort);
        m_HardwareConnected = false;
        m_State = Deki::PackageState::Initialized;
        return true;  // Still considered Initialized; reads will return zeros.
    }

    uint8_t who = 0;
    if (m_Bus->Read(m_I2cAddr, kRegWhoAmI, &who, 1) && (who == kWhoAmILsm6ds3 || who == kWhoAmILsm6ds3C))
    {
        m_HardwareConnected = true;
    }
    else
    {
        DEKI_LOG_WARNING("LSM6DS3IMU: unexpected WHO_AM_I=0x%02X at addr 0x%02X", who, m_I2cAddr);
        m_HardwareConnected = false;
    }

    // Core config: 416 Hz on both accel & gyro, BDU on
    const uint8_t ctrl1 = kCtrl1Xl416Hz2G;
    const uint8_t ctrl2 = kCtrl2G416Hz245;
    const uint8_t ctrl3 = kCtrl3CBdu;
    m_Bus->Write(m_I2cAddr, kRegCtrl1Xl, &ctrl1, 1);
    m_Bus->Write(m_I2cAddr, kRegCtrl2G, &ctrl2, 1);
    m_Bus->Write(m_I2cAddr, kRegCtrl3C, &ctrl3, 1);

    if (m_Pedometer)
    {
        if (!EnablePedometer())
        {
            DEKI_LOG_WARNING("LSM6DS3IMU: pedometer enable failed");
        }
    }

    m_State = Deki::PackageState::Initialized;
    return true;
}

void LSM6DS3IMU::Shutdown()
{
    m_Bus = nullptr;
    m_State = Deki::PackageState::Uninitialized;
    m_HardwareConnected = false;
}

bool LSM6DS3IMU::EnablePedometer()
{
    if (!m_Bus)
    {
        return false;
    }
    const uint8_t tapCfg = kTapCfgPedoEn;
    const uint8_t ctrl10C = kCtrl10CPedoFun;
    bool ok = true;
    ok &= m_Bus->Write(m_I2cAddr, kRegTapCfg, &tapCfg, 1);
    ok &= m_Bus->Write(m_I2cAddr, kRegCtrl10C, &ctrl10C, 1);
    return ok;
}

DekiVec3f LSM6DS3IMU::ReadAccelMetersPerSecondSquared() const
{
    DekiVec3f v{};
    if (!m_Bus)
    {
        return v;
    }

    uint8_t raw[6] = {};
    if (!m_Bus->Read(m_I2cAddr, kRegOutXlL, raw, 6))
    {
        return v;
    }

    v.x = (float)ToS16(raw[0], raw[1]) * m_AccelScale;
    v.y = (float)ToS16(raw[2], raw[3]) * m_AccelScale;
    v.z = (float)ToS16(raw[4], raw[5]) * m_AccelScale;
    return v;
}

DekiVec3f LSM6DS3IMU::ReadGyroRadiansPerSecond() const
{
    DekiVec3f v{};
    if (!m_Bus)
    {
        return v;
    }

    uint8_t raw[6] = {};
    if (!m_Bus->Read(m_I2cAddr, kRegOutGL, raw, 6))
    {
        return v;
    }

    v.x = (float)ToS16(raw[0], raw[1]) * m_GyroScale;
    v.y = (float)ToS16(raw[2], raw[3]) * m_GyroScale;
    v.z = (float)ToS16(raw[4], raw[5]) * m_GyroScale;
    return v;
}

uint32_t LSM6DS3IMU::GetStepCount() const
{
    if (!m_Bus)
    {
        return 0;
    }
    uint8_t raw[2] = {};
    if (!m_Bus->Read(m_I2cAddr, kRegStepCounterL, raw, 2))
    {
        return 0;
    }
    return (uint32_t)((uint16_t)raw[0] | ((uint16_t)raw[1] << 8));
}

void LSM6DS3IMU::ResetStepCount()
{
    if (!m_Bus)
    {
        return;
    }
    // TAP_CFG bit 6 = PEDO_RST_STEP (toggled on to clear, hardware auto-clears)
    const uint8_t reset = kTapCfgPedoEn | 0x02;  // keep func enable + set PEDO_RST_STEP bit
    m_Bus->Write(m_I2cAddr, kRegTapCfg, &reset, 1);
    const uint8_t normal = kTapCfgPedoEn;
    m_Bus->Write(m_I2cAddr, kRegTapCfg, &normal, 1);
}

}  // namespace DekiImu
