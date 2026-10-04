# Changelog

Notable changes to `deki-imu`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

## Unreleased

### Changed
- **Names follow the code style** (deki-engine/docs/codestyle): types, functions and enum values are PascalCase, constants kPascalCase, members m_PascalCase, locals and parameters camelCase. The code is formatted with clang-format 22.
- The functions the editor finds by name are PascalCase: DekiIMURegisterComponents, DekiIMUGetAutoComponentCount, DekiIMUEnsureRegistered and the rest. Built against engine ABI 21; a build of this package from before does not load and is rebuilt.

### Fixed
- LSM6DS3: resetting the step count works. It set a bit in the wrong
  register, so the count never cleared.
- LSM6DS3: with the pedometer on, the gyroscope's X and Z axes stay on. Turning
  the pedometer on switched them off on the original LSM6DS3. The pedometer
  bits now follow the chip: the LSM6DS3 and the LSM6DS3TR-C keep them in
  different places.

## 0.17.0

### Changed
- `minEngine` 0.17.0. Reflection ABI 20: the package must be rebuilt.

## 0.16.0

### Changed
- **Moved into the `DekiImu` namespace.** Every component was declared at global
  scope, which made its identity a bare class name — the name a scene file
  stores and the name the registry keys on — so two packages defining one name
  collided there with nothing to tell them apart. Each component carries
  `DEKI_FORMER_NAME` with the name it was saved under before, so existing
  scenes load unchanged and are written back qualified on the next save.
  Code naming these types needs the namespace: `using namespace DekiImu;` or a
  qualified name.
- Enum properties are stored by name rather than by number, so appending to an
  enum or reordering one no longer changes what a saved scene means. Files
  written before this still read.
- `minEngine` 0.16.0. Reflection ABI 17: the package must be rebuilt.

## 0.15.0

### Changed
- Readings say their unit in their own name. `ReadAccelMetersPerSecondSquared()`
  and `ReadGyroRadiansPerSecond()` are the virtuals a driver implements, in the
  SI units the engine stores; `ReadAccelG()` and `ReadGyroDegreesPerSecond()`
  are derived from them and are not virtual, so the two spellings cannot
  disagree. A bare `ReadGyro()` left the unit in a comment, and a value fed
  into a rotation on the wrong guess is out by 57.3x.

  Migration: `ReadGyro()` becomes `ReadGyroRadiansPerSecond()`, `ReadAccel()`
  becomes `ReadAccelMetersPerSecondSquared()`. Use the datasheet spellings when
  comparing against a chip's full-scale figures.

### Added
- Unit tests covering the conversions in both directions and that the derived
  accessors go through the virtual ones.
