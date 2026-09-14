# Changelog

Notable changes to `deki-imu`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

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
