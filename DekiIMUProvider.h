#pragma once

#include "IDekiIMU.h"

/**
 * @brief Published-instance registry for the currently active IMU driver.
 *
 * Chip-specific SetupComponents (e.g., LSM6DS3Component) call SetCurrent() in
 * their Setup() once the driver is configured and initialized. Game code reads
 * the current IMU via GetCurrent().
 */
class DekiIMUProvider
{
public:
    static void      SetCurrent(IDekiIMU* imu);
    static IDekiIMU* GetCurrent();

private:
    static IDekiIMU* s_Current;
};
