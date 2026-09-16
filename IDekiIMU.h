#pragma once

#include <deki/providers/IPackage.h>
#include <deki/Math.h>
#include <cstdint>

namespace DekiImu
{

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
 * Readings come in two spellings, and every one of them says its unit in its
 * own name. A bare ReadGyro() returning a DekiVec3f said nothing, so whether
 * it was radians or degrees lived in a comment — and a value fed into a
 * rotation on the strength of the wrong guess is out by 57.3x.
 *
 * The SI accessor is the virtual one, because that is what the engine stores
 * (see deki-editor/docs/units.md: metres, radians, seconds). The datasheet
 * accessor is derived from it and is NOT virtual, so a driver cannot make the
 * two disagree. Debugging against the chip's +/-245 dps or +/-2 g figures uses
 * the datasheet spelling; anything that integrates a reading uses SI.
 *
 * Step counter: monotonic count since power-on or last Reset.
 */
class IDekiIMU : public Deki::IPackage
{
public:
    const char* GetPackageCategory() const override { return "imu"; }

    /// Acceleration in metres per second squared.
    virtual DekiVec3f ReadAccelMetersPerSecondSquared() const = 0;

    /// Angular velocity in radians per second.
    virtual DekiVec3f ReadGyroRadiansPerSecond() const = 0;

    /// The same acceleration in g, which is what an accelerometer datasheet
    /// quotes its full-scale range in.
    DekiVec3f ReadAccelG() const
    {
        constexpr float kInvG = 1.0f / 9.80665f;
        const DekiVec3f a = ReadAccelMetersPerSecondSquared();
        return { a.x * kInvG, a.y * kInvG, a.z * kInvG };
    }

    /// The same rate in degrees per second, which is what a gyro datasheet
    /// quotes its full-scale range in.
    DekiVec3f ReadGyroDegreesPerSecond() const
    {
        const DekiVec3f g = ReadGyroRadiansPerSecond();
        return { g.x * Deki::Math::kRadToDeg,
                 g.y * Deki::Math::kRadToDeg,
                 g.z * Deki::Math::kRadToDeg };
    }

    virtual uint32_t  GetStepCount() const = 0;
    virtual void      ResetStepCount() = 0;

    virtual bool      IsHardwareConnected() const = 0;
};

}  // namespace DekiImu
