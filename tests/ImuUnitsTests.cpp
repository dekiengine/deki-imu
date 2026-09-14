// The two spellings of a reading must never disagree.
//
// IDekiIMU declares the SI accessor virtual and derives the datasheet one
// from it, so a driver cannot implement the pair inconsistently. These tests
// pin that arrangement: that the derived value is exactly the documented
// conversion of the virtual one, and that a driver overriding only the SI
// accessor gets a correct datasheet accessor for free.

#include <gtest/gtest.h>

#include <IDekiIMU.h>

#include <cmath>

namespace
{

// A driver that returns whatever the test puts in, in SI, and counts calls so
// the derived accessors can be shown to go through the virtual ones.
class FakeIMU : public IDekiIMU
{
   public:
    DekiVec3f accel{};  // m/s^2
    DekiVec3f gyro{};   // rad/s
    mutable int accelReads = 0;
    mutable int gyroReads = 0;

    DekiVec3f ReadAccelMetersPerSecondSquared() const override
    {
        ++accelReads;
        return accel;
    }
    DekiVec3f ReadGyroRadiansPerSecond() const override
    {
        ++gyroReads;
        return gyro;
    }

    uint32_t GetStepCount() const override { return 0; }
    void ResetStepCount() override {}
    bool IsHardwareConnected() const override { return true; }

    const char* GetPackageId() const override { return "fake-imu"; }
    const char* GetPackageName() const override { return "Fake IMU"; }
    void Configure(const Deki::PackageConfig&) override {}
    bool Initialize() override { return true; }
    void Shutdown() override {}
    void Update(float) override {}
    Deki::PackageState GetState() const override { return Deki::PackageState::Initialized; }
    const char* GetLastError() const override { return ""; }
};

constexpr float kG = 9.80665f;

}  // namespace

TEST(ImuUnits, AccelGIsTheSIValueDividedByStandardGravity)
{
    FakeIMU imu;
    imu.accel = {kG, 2.0f * kG, -0.5f * kG};

    const DekiVec3f g = imu.ReadAccelG();
    EXPECT_FLOAT_EQ(g.x, 1.0f);
    EXPECT_FLOAT_EQ(g.y, 2.0f);
    EXPECT_FLOAT_EQ(g.z, -0.5f);
}

TEST(ImuUnits, AccelAtRestOnItsBackReadsOneG)
{
    // What a datasheet's "±2 g" range means in practice: a stationary sensor
    // reads one g on whichever axis points up.
    FakeIMU imu;
    imu.accel = {0.0f, 0.0f, kG};
    EXPECT_NEAR(imu.ReadAccelG().z, 1.0f, 1e-6f);
}

TEST(ImuUnits, GyroDegreesIsTheSIValueConverted)
{
    FakeIMU imu;
    imu.gyro = {Deki::Math::kPi, Deki::Math::kPi * 0.5f, -Deki::Math::kPi};

    const DekiVec3f d = imu.ReadGyroDegreesPerSecond();
    EXPECT_NEAR(d.x, 180.0f, 1e-3f);
    EXPECT_NEAR(d.y, 90.0f, 1e-3f);
    EXPECT_NEAR(d.z, -180.0f, 1e-3f);
}

TEST(ImuUnits, GyroFullScaleMatchesTheDatasheetFigure)
{
    // The LSM6DS3's default range is ±245 dps. A driver storing that rate in
    // SI must read back as 245 in the datasheet spelling, or the two
    // documented full-scale numbers describe different sensors.
    FakeIMU imu;
    imu.gyro = {245.0f * Deki::Math::kDegToRad, 0.0f, 0.0f};
    EXPECT_NEAR(imu.ReadGyroDegreesPerSecond().x, 245.0f, 1e-3f);
}

TEST(ImuUnits, DerivedAccessorsGoThroughTheVirtualOnes)
{
    // This is what makes the two spellings impossible to desync: the derived
    // accessor is not virtual and has no state of its own, so it must call the
    // driver's SI accessor to answer at all.
    FakeIMU imu;
    imu.ReadAccelG();
    imu.ReadGyroDegreesPerSecond();
    EXPECT_EQ(imu.accelReads, 1);
    EXPECT_EQ(imu.gyroReads, 1);
}

TEST(ImuUnits, ConversionsRoundTrip)
{
    FakeIMU imu;
    for (float dps : {0.0f, 1.0f, -1.0f, 245.0f, -245.0f, 2000.0f})
    {
        imu.gyro = {dps * Deki::Math::kDegToRad, 0.0f, 0.0f};
        EXPECT_NEAR(imu.ReadGyroDegreesPerSecond().x, dps, std::abs(dps) * 1e-5f + 1e-5f);
    }
    for (float g : {0.0f, 1.0f, -1.0f, 2.0f, -16.0f})
    {
        imu.accel = {g * kG, 0.0f, 0.0f};
        EXPECT_NEAR(imu.ReadAccelG().x, g, std::abs(g) * 1e-5f + 1e-5f);
    }
}

TEST(ImuUnits, ZeroIsZeroInBothSpellings)
{
    FakeIMU imu;
    EXPECT_FLOAT_EQ(imu.ReadAccelG().x, 0.0f);
    EXPECT_FLOAT_EQ(imu.ReadGyroDegreesPerSecond().x, 0.0f);
}
