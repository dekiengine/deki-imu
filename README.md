# Deki IMU

Docs: https://dekiengine.github.io/deki-imu/ (components and properties, generated from the code)

Accelerometer + gyroscope peripheral interface for the Deki Engine, with an LSM6DS3 backend (hardware pedometer).

Part of [Deki Engine](https://github.com/dekiengine/deki-engine).

## Namespace

Types live in `DekiImu`. Scene files store the qualified name, and so does code:

```cpp
using namespace DekiImu;
obj->AddComponent<SomeComponent>();
```

## Dependencies

| Dependency | Type |
|---|---|
| `deki-i2c` | Deki package |

## Install

Package Manager in the Deki Editor, or `DekiEditor --packages-add deki-imu <project>`.

## License

Apache 2.0. See [LICENSE](LICENSE).
