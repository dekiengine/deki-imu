#include "LSM6DS3IMUComponent.h"
#include "DekiIMUProvider.h"
#include "modules/ModuleConfig.h"
#include "DekiLogSystem.h"
#include <string>

static LSM6DS3IMU* s_Driver = nullptr;

void LSM6DS3IMUComponent::Setup(SetupCallback onComplete)
{
    if (!s_Driver)
        s_Driver = new LSM6DS3IMU();

    ModuleConfig cfg;
    cfg.moduleId = "imu";
    cfg.enabled  = true;
    cfg.settings["i2c_port"]         = std::to_string(i2c_port);
    cfg.settings["i2c_address"]      = std::to_string(i2c_address);
    cfg.settings["enable_pedometer"] = enable_pedometer ? "true" : "false";

    s_Driver->Configure(cfg);

    const bool success = s_Driver->Initialize();
    if (success)
    {
        DekiIMUProvider::SetCurrent(s_Driver);
    }
    else
    {
        DEKI_LOG_ERROR("LSM6DS3IMUComponent: Failed to initialize LSM6DS3 on I2C port %d addr 0x%02X",
                       (int)i2c_port, (unsigned)i2c_address);
    }

    if (onComplete) onComplete(success);
}
