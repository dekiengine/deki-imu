# Deki IMU

Documentation: https://dekiengine.github.io/deki-imu/ (components and properties, generated from the code)

Accelerometer + gyroscope peripheral interface for the Deki Engine, with an LSM6DS3 backend (hardware pedometer).

Part of the [Deki Engine](https://github.com/dekiengine/deki-engine) package ecosystem.

## Namespace

This package's types live in `DekiImu`. Scene files store the qualified
name, so a component is `DekiImu::SomeComponent` there, and code naming one
needs the namespace:

```cpp
using namespace DekiImu;
obj->AddComponent<SomeComponent>();
```

Scenes saved before 0.16.0 used bare names and still load: every component
records what it used to be called, and a save writes the current name.

## Dependencies

| Dependency | Type |
|---|---|
| `deki-i2c` | Deki package |

## Installation

Install via the Package Manager inside the Deki Editor.

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.
