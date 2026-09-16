# Changelog

Notable changes to `deki-imu`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

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
