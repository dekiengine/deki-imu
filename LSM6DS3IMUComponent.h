#pragma once

#include <cstdint>
#include "SetupComponent.h"
#include "reflection/DekiProperty.h"
#include "chips/LSM6DS3IMU.h"

/**
 * @brief Boot-scene component for the LSM6DS3 6-axis IMU.
 *
 * Wires to the shared I2C bus at address 0x6A (or 0x6B if SDO is pulled high).
 * Requires an I2CBusComponent on the matching port in boot.scene.
 *
 * Includes the LSM6DS3's built-in hardware pedometer (step counter) — toggle
 * via `enablePedometer`.
 */
class LSM6DS3IMUComponent : public Deki::SetupComponent
{
public:
    DEKI_COMPONENT(LSM6DS3IMUComponent, Deki::SetupComponent, "Sensors", "2b4f8e91-7d36-4a50-9c18-4f6b2d8e1a35", "")
    DEKI_DISPLAY_NAME("LSM6DS3 IMU")
    DEKI_DESCRIPTION("Reads the LSM6DS3 motion sensor over I2C, step counter included.")

    DEKI_EXPORT
    DEKI_RANGE(0, 3)
    int32_t i2cPort = 0;

    /** @brief 7-bit I2C address: 0x6A (SDO low, default) or 0x6B (SDO high). */
    DEKI_EXPORT
    DEKI_RANGE(0, 127)
    int32_t i2cAddress = 0x6A;

    /** @brief Enable the LSM6DS3 built-in hardware pedometer. */
    DEKI_EXPORT
    bool enablePedometer = true;

    LSM6DS3IMUComponent() = default;
    virtual ~LSM6DS3IMUComponent() = default;

    void        Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "LSM6DS3 IMU"; }
};

#include "generated/LSM6DS3IMUComponent.gen.h"
