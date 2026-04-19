#include "DekiIMUModule.h"
#include "interop/DekiPlugin.h"
#include "DekiIMUProvider.h"
#include "chips/LSM6DS3IMU.h"
#include "DekiLogSystem.h"

namespace
{
struct DekiIMUBackendInit
{
    DekiIMUBackendInit()
    {
        DEKI_LOG_INFO("[deki-imu] Module loaded, registering LSM6DS3 factory");
        DekiIMUProvider::SetFactory([]() -> IDekiIMU* { return new LSM6DS3IMU(); });
    }
};
static DekiIMUBackendInit s_imu_init;
}

#ifdef DEKI_EDITOR

extern void DekiIMU_RegisterComponents();
extern int  DekiIMU_GetAutoComponentCount();
extern const DekiComponentMeta* DekiIMU_GetAutoComponentMeta(int index);

static bool s_IMURegistered = false;

extern "C" {

DEKI_IMU_API int DekiIMU_EnsureRegistered(void)
{
    if (s_IMURegistered)
        return DekiIMU_GetAutoComponentCount();
    s_IMURegistered = true;
    DekiIMU_RegisterComponents();
    return DekiIMU_GetAutoComponentCount();
}

DEKI_PLUGIN_API const char* DekiPlugin_GetName(void)    { return "Deki IMU Module"; }
DEKI_PLUGIN_API const char* DekiPlugin_GetVersion(void)
{
#ifdef DEKI_MODULE_VERSION
    return DEKI_MODULE_VERSION;
#else
    return "0.0.0-dev";
#endif
}
DEKI_PLUGIN_API const char* DekiPlugin_GetReflectionJson(void) { return "{}"; }
DEKI_PLUGIN_API int  DekiPlugin_Init(void)             { DEKI_LOG_INFO("[deki-imu] DekiPlugin_Init"); return 0; }
DEKI_PLUGIN_API void DekiPlugin_Shutdown(void)         { s_IMURegistered = false; }
DEKI_PLUGIN_API int  DekiPlugin_GetComponentCount(void){ return DekiIMU_GetAutoComponentCount(); }
DEKI_PLUGIN_API const DekiComponentMeta* DekiPlugin_GetComponentMeta(int index)
{
    return DekiIMU_GetAutoComponentMeta(index);
}
DEKI_PLUGIN_API void DekiPlugin_RegisterComponents(void)
{
    int n = DekiIMU_EnsureRegistered();
    DEKI_LOG_INFO("[deki-imu] DekiPlugin_RegisterComponents -> %d component(s)", n);
}

DEKI_PLUGIN_API int DekiPlugin_GetFeatureCount(void) { return 0; }
DEKI_PLUGIN_API const struct DekiModuleFeatureInfo* DekiPlugin_GetFeature(int) { return nullptr; }

} // extern "C"

#endif // DEKI_EDITOR
