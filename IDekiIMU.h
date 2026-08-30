#pragma once

#include "providers/IDekiPackage.h"
#include <cstdint>

struct DekiVec3f
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

/**
 * @brief 6-axis IMU interface (3-axis accelerometer + 3-axis gyroscope,
 *        plus optional hardware pedometer step counter).
 *
 * Accelerometer units: g (1.0 == 9.81 m/s^2).
 * Gyroscope units: degrees per second (dps).
 * Step counter: monotonic count since power-on or last Reset.
 */
class IDekiIMU : public IDekiPackage
{
public:
    const char* GetPackageCategory() const override { return "imu"; }

    virtual DekiVec3f ReadAccel() const = 0;
    virtual DekiVec3f ReadGyro() const = 0;

    virtual uint32_t  GetStepCount() const = 0;
    virtual void      ResetStepCount() = 0;

    virtual bool      IsHardwareConnected() const = 0;
};
