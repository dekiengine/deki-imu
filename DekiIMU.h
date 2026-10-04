#pragma once

#include "IDekiIMU.h"

namespace DekiImu
{

/// Holds the active IMU driver.
///
/// A chip's SetupComponent (such as LSM6DS3IMUComponent) calls SetCurrent()
/// in Setup() once its driver is initialized. Game code reads it through
/// GetCurrent().
class DekiIMU
{
public:
    static void SetCurrent(IDekiIMU* imu);
    static IDekiIMU* GetCurrent();

private:
    static IDekiIMU* s_Current;
};

}  // namespace DekiImu
