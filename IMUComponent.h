#pragma once

#include <cstdint>
#include "SetupComponent.h"
#include "reflection/DekiProperty.h"
#include "IDekiIMU.h"

class IMUComponent : public SetupComponent
{
public:
    DEKI_COMPONENT(IMUComponent, SetupComponent, "System", "1f7c82a5-9d63-4e08-b4a1-6c5d2e9a0b34", "")

    /** @brief Which I2C bus (port) the IMU is wired to. Pins/freq live on the matching I2CBusComponent. */
    DEKI_EXPORT
    DEKI_RANGE(0, 3)
    int32_t i2c_port = 0;

    /** @brief 7-bit I2C address: 0x6A (SDO low, default) or 0x6B (SDO high). */
    DEKI_EXPORT
    DEKI_RANGE(0, 127)
    int32_t i2c_address = 0x6A;

    /** @brief Enable the LSM6DS3 built-in hardware pedometer. Small extra current; frees the MCU. */
    DEKI_EXPORT
    bool enable_pedometer = true;

    IMUComponent() = default;
    virtual ~IMUComponent() = default;

    void        Setup(SetupCallback onComplete) override;
    const char* GetSetupName() const override { return "IMU"; }

    static IDekiIMU* GetIMUModule();
};

#include "generated/IMUComponent.gen.h"
