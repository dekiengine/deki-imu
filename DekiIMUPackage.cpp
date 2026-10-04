/**
 * @file DekiIMUPackage.cpp
 * @brief Package entry point for deki-imu
 */
#include "DekiIMUPackage.h"
#include <deki/interop/Plugin.h>
#include <deki/LogSystem.h>

extern void DekiIMURegisterComponents();
extern int DekiIMUGetAutoComponentCount();
extern const Deki::ComponentMeta* DekiIMUGetAutoComponentMeta(int index);

namespace DekiImu
{

#ifdef DEKI_EDITOR

static bool s_IMURegistered = false;

// The exports below are C symbols at global scope; the package's own
// registration helpers and statics live in its namespace.
using namespace DekiImu;

extern "C"
{
    DEKI_IMU_API int DekiIMUEnsureRegistered(void)
    {
        if (s_IMURegistered)
        {
            return ::DekiIMUGetAutoComponentCount();
        }
        s_IMURegistered = true;
        ::DekiIMURegisterComponents();
        return ::DekiIMUGetAutoComponentCount();
    }

    DEKI_PLUGIN_API const char* DekiPluginGetName(void)
    {
        return "Deki IMU Package";
    }
    DEKI_PLUGIN_API const char* DekiPluginGetVersion(void)
    {
#ifdef DEKI_PACKAGE_VERSION
        return DEKI_PACKAGE_VERSION;
#else
        return "0.0.0-dev";
#endif
    }
    DEKI_PLUGIN_API int DekiPluginInit(void)
    {
        return 0;
    }
    DEKI_PLUGIN_API void DekiPluginShutdown(void)
    {
        s_IMURegistered = false;
    }
    DEKI_PLUGIN_API int DekiPluginGetComponentCount(void)
    {
        return ::DekiIMUGetAutoComponentCount();
    }
    DEKI_PLUGIN_API const Deki::ComponentMeta* DekiPluginGetComponentMeta(int index)
    {
        return ::DekiIMUGetAutoComponentMeta(index);
    }
    DEKI_PLUGIN_API void DekiPluginRegisterComponents(void)
    {
        DekiIMUEnsureRegistered();
    }

}  // extern "C"

#endif  // DEKI_EDITOR
}  // namespace DekiImu
