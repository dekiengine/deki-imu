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

/// 6-axis IMU: 3-axis accelerometer and 3-axis gyroscope, plus an optional
/// hardware step counter.
///
/// Every reading names its unit, because radians mistaken for degrees put a
/// rotation out by 57.3x.
///
/// The SI accessor is virtual, since SI is what the engine stores (see
/// deki-editor/docs/units.md: metres, radians, seconds). The datasheet
/// accessor is derived from it and is not virtual, so a driver cannot make
/// the two disagree. Use the datasheet units to compare with the chip's
/// +/-245 dps or +/-2 g figures; use SI for anything that integrates a
/// reading.
///
/// The step count only goes up, from power-on or the last reset.
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
        return { g.x * Deki::Math::kRadToDeg, g.y * Deki::Math::kRadToDeg, g.z * Deki::Math::kRadToDeg };
    }

    virtual uint32_t GetStepCount() const = 0;
    virtual void ResetStepCount() = 0;

    virtual bool IsHardwareConnected() const = 0;
};

}  // namespace DekiImu
