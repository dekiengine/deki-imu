#pragma once

#include <cstdint>
#include <deki/SetupComponent.h>
#include <deki/reflection/Property.h>
#include "chips/LSM6DS3IMU.h"

namespace DekiImu
{

/// Boot-scene component for the LSM6DS3 6-axis IMU.
///
/// Talks to the chip on a shared I2C bus at 0x6A (0x6B when SDO is pulled
/// high), so boot.scene needs an I2CBusComponent on the same port.
///
/// `enablePedometer` turns on the chip's hardware step counter.
DEKI_CATEGORY("Sensors")
DEKI_DISPLAY_NAME("LSM6DS3 IMU")
DEKI_DESCRIPTION("Reads the LSM6DS3 motion sensor over I2C, step counter included.")
class LSM6DS3IMUComponent : public Deki::SetupComponent
{
public:
    DEKI_EXPORT
    DEKI_TOOLTIP("Which I2C bus the sensor is on. Must match the I2C Bus component that set that port up.")
    DEKI_RANGE(0, 3)
    int32_t i2cPort = 0;

    // 7-bit I2C address: 0x6A (SDO low, the default) or 0x6B (SDO high).
    DEKI_EXPORT
    DEKI_TOOLTIP("The sensor's address on the bus, set by its SDO/SA0 pin: 0x6A when that pin is low, 0x6B when high. "
                 "Two of these chips can share a bus by wiring that pin differently.")
    DEKI_RANGE(0, 127)
    int32_t i2cAddress = 0x6A;

    DEKI_EXPORT
    DEKI_TOOLTIP("Run the chip's built-in step counter. It counts in hardware, so steps keep accumulating without the "
                 "CPU waking, at a small extra current draw.")
    bool enablePedometer = true;

    LSM6DS3IMUComponent() = default;
    virtual ~LSM6DS3IMUComponent() = default;

    void Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "LSM6DS3 IMU"; }
};

}  // namespace DekiImu
