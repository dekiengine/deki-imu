/**
 * @file DekiIMUPackage.cpp
 * @brief Package entry point for deki-imu
 */
#include "DekiIMUPackage.h"
#include "interop/DekiPlugin.h"
#include "DekiLogSystem.h"

#ifdef DEKI_EDITOR

extern void DekiIMU_RegisterComponents();
extern int  DekiIMU_GetAutoComponentCount();
extern const Deki::ComponentMeta* DekiIMU_GetAutoComponentMeta(int index);

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

DEKI_PLUGIN_API const char* DekiPlugin_GetName(void)    { return "Deki IMU Package"; }
DEKI_PLUGIN_API const char* DekiPlugin_GetVersion(void)
{
#ifdef DEKI_PACKAGE_VERSION
    return DEKI_PACKAGE_VERSION;
#else
    return "0.0.0-dev";
#endif
}
DEKI_PLUGIN_API int  DekiPlugin_Init(void)             { DEKI_LOG_INFO("[deki-imu] DekiPlugin_Init"); return 0; }
DEKI_PLUGIN_API void DekiPlugin_Shutdown(void)         { s_IMURegistered = false; }
DEKI_PLUGIN_API int  DekiPlugin_GetComponentCount(void){ return DekiIMU_GetAutoComponentCount(); }
DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPlugin_GetComponentMeta(int index)
{
    return DekiIMU_GetAutoComponentMeta(index);
}
DEKI_PLUGIN_API void DekiPlugin_RegisterComponents(void)
{
    int n = DekiIMU_EnsureRegistered();
    DEKI_LOG_INFO("[deki-imu] DekiPlugin_RegisterComponents -> %d component(s)", n);
}


} // extern "C"

#endif // DEKI_EDITOR
