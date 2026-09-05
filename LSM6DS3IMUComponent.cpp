#include "LSM6DS3IMUComponent.h"
#include "DekiIMU.h"
#include "PackageConfig.h"
#include "DekiLogSystem.h"
#include <string>

static LSM6DS3IMU* s_Driver = nullptr;

void LSM6DS3IMUComponent::Setup(SetupCallback onComplete)
{
    if (!s_Driver)
        s_Driver = new LSM6DS3IMU();

    Deki::PackageConfig cfg;
    cfg.packageId = "imu";
    cfg.enabled  = true;
    cfg.settings["i2cPort"]         = std::to_string(i2cPort);
    cfg.settings["i2cAddress"]      = std::to_string(i2cAddress);
    cfg.settings["enablePedometer"] = enablePedometer ? "true" : "false";

    s_Driver->Configure(cfg);

    const bool success = s_Driver->Initialize();
    if (success)
    {
        DekiIMU::SetCurrent(s_Driver);
    }
    else
    {
        DEKI_LOG_ERROR("LSM6DS3IMUComponent: Failed to initialize LSM6DS3 on I2C port %d addr 0x%02X",
                       (int)i2cPort, (unsigned)i2cAddress);
    }

    if (onComplete) onComplete(success);
}
