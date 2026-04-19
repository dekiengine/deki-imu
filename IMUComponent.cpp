#include "IMUComponent.h"
#include "DekiIMUProvider.h"
#include "modules/ModuleConfig.h"
#include "DekiLogSystem.h"
#include <string>

static IDekiIMU* s_IMUModule = nullptr;

void IMUComponent::Setup(SetupCallback onComplete)
{
    if (!s_IMUModule)
    {
        s_IMUModule = DekiIMUProvider::Create();
        if (!s_IMUModule)
        {
            DEKI_LOG_ERROR("IMUComponent: No IMU backend registered");
            if (onComplete) onComplete(false);
            return;
        }
    }

    ModuleConfig config;
    config.moduleId = "imu";
    config.enabled  = true;
    config.settings["i2c_port"]         = std::to_string(i2c_port);
    config.settings["i2c_address"]      = std::to_string(i2c_address);
    config.settings["enable_pedometer"] = enable_pedometer ? "true" : "false";

    s_IMUModule->Configure(config);

    bool success = s_IMUModule->Initialize();
    if (success)
    {
        DekiIMUProvider::SetCurrent(s_IMUModule);
    }
    else
    {
        DEKI_LOG_ERROR("IMUComponent: Failed to initialize IMU backend");
    }

    if (onComplete) onComplete(success);
}

IDekiIMU* IMUComponent::GetIMUModule()
{
    return s_IMUModule;
}
