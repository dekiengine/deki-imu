#pragma once

#include "../IDekiIMU.h"
#include <deki/PackageConfig.h>
#include "IDekiI2C.h"  // from deki-i2c
#include <string>

namespace DekiImu
{

class LSM6DS3IMU : public IDekiIMU
{
public:
    LSM6DS3IMU() = default;
    ~LSM6DS3IMU() override = default;

    const char* GetPackageId() const override   { return "imu"; }
    const char* GetPackageName() const override { return "LSM6DS3 IMU (I\xC2\xB2""C)"; }
    void        Configure(const Deki::PackageConfig& config) override;
    bool        Initialize() override;
    void        Shutdown() override;
    void        Update(float) override {}
    Deki::PackageState GetState() const override      { return m_State; }
    const char* GetLastError() const override  { return m_LastError.c_str(); }

    DekiVec3f ReadAccelMetersPerSecondSquared() const override;
    DekiVec3f ReadGyroRadiansPerSecond() const override;
    uint32_t  GetStepCount() const override;
    void      ResetStepCount() override;
    bool      IsHardwareConnected() const override { return m_HardwareConnected; }

private:
    int        m_BusPort = 0;
    uint8_t    m_I2cAddr = 0x6A;
    bool       m_Pedometer = true;
    DekiI2c::IDekiI2C*  m_Bus = nullptr;

    Deki::PackageState m_State = Deki::PackageState::Uninitialized;
    bool        m_HardwareConnected = false;
    std::string m_LastError;

    // Chip counts -> SI, in one place. The datasheet figures are the ±2 g and
    // ±245 dps full-scale ranges, kept visible in the expressions; the
    // conversion to SI happens here at the driver boundary and nowhere else.
    float       m_AccelScale = (2.0f / 32768.0f) * 9.80665f;   // LSB -> m/s^2 at ±2 g full scale
    float       m_GyroScale  = (245.0f / 32768.0f) * Deki::Math::kDegToRad;  // LSB -> rad/s at ±245 dps

    bool EnablePedometer();
};

}  // namespace DekiImu
