# Per-World Physical Length Scale — Design

Date: 2026-07-10
Status: SUPERSEDED (2026-07-20) — kept for history. The shipped design evolved past this
spec: `UnitOfLength`/`WorldLength` became the 128-bit `WorldCoordinate` plus the 64-bit
`LocalCoordinate` (the only type that converts to physical units), `metersPerUnit` became
the power-of-ten `PhysicalScale`, and cross-type conversion/rotation lives on the
`spatial` utility struct. See `src/inc/garnet/e2/spatial.h` and `src/core/e2/README.md`.

## Problem

The engine2 spatial system encodes the physical scale of a length unit
(meters-per-unit) at compile time via `std::ratio`, so every world shares one
global scale. Each world should instead choose its own scale at construction
time — e.g. a millimeter-precision indoor world and a meter-precision terrain
world coexisting in one binary.

## Decisions

| Question | Decision | Rationale |
|---|---|---|
| Scale representation | plain `double metersPerUnit` | Conversion API is float in/out, so float precision already caps results; a runtime rational adds API friction for no practical gain. Type-level unit safety is lost the moment scale becomes runtime, regardless of representation. |
| Where scale lives | `World` (ctor param, public const member), carried into the visual path | Scale is a property of a world. |
| Visual path | Moments carry world-unit values plus the scale; conversion to float meters happens inside the visual domain | Future plan: convert absolute positions to camera-relative inside visual *first* (exact int64 math), then scale to physical floats — maximizes precision. Requires positions to arrive in visual still in integer world units. |
| `WorldLength`/`WorldPosition` aliases | stay in `spatial.h` | Moving them to world.h creates a header-order cycle with visual.h (`Camera::Desc` uses them; world.h needs `VisualMoment::CaptureParameters`). |

## 1. spatial.h

```cpp
template<std::integral S>
class UnitOfLength {
public:
    using Storage = S;

    constexpr UnitOfLength() = default;
    explicit constexpr UnitOfLength(Storage v): _value(v) {}

    constexpr Storage raw() const { return _value; }

    // +, -, *, / operators unchanged (length op length -> length)

    constexpr float toMeters(double metersPerUnit) const;
    constexpr float toCentimeters(double metersPerUnit) const;
    static constexpr UnitOfLength fromMeters(float m, double metersPerUnit);
    static constexpr UnitOfLength fromCentimeters(float cm, double metersPerUnit);

private:
    Storage _value = 0;
};

using WorldLength   = UnitOfLength<int64_t>;
using WorldPosition = Vector3<WorldLength>;
```

- **Removed**: `PhysicalLengthConfig` concept, `is_std_ratio` trait,
  `PhysicalLength` struct.
- Template takes only the storage type, constrained to `std::integral`.
- Conversion methods take the scale as a runtime argument.
- Centimeter math fixed: cm ↔ base conversion uses a factor of 100 (the old
  code multiplied by 1000).
- Constructor becomes public: call sites (samples, tests, forms) construct
  lengths directly.
- Raw value exposed via `raw()`; `_value` stays private.

## 2. world.h

```cpp
struct World : Thing {
    struct CreateParameters {
        Universe & universe;
        double     metersPerUnit = 1.0; ///< physical size of one WorldLength unit, in meters
    };

    /// Physical size of one WorldLength unit in this world, in meters.
    const double metersPerUnit;

    // Inlined conversion helpers bound to this world's scale.
    float       toMeters(WorldLength v) const;
    float       toCentimeters(WorldLength v) const;
    WorldLength fromMeters(float m) const;
    WorldLength fromCentimeters(float cm) const;
    glm::vec3   toVec3(const WorldPosition &) const; ///< to float meters

protected:
    World(const RuntimeType::TypeInfo &, UniqueIdentifier, const StrA & name,
          double metersPerUnit);
};
```

- Public const member follows the existing `Thing::id` / `Thing::name` style.
- `World` gains its own protected ctor (it previously inherited `Thing`'s via
  `GN_E2_DEFINE_A_THING`); subclasses pass the scale through.

## 3. Visual path

- `VisualMoment::CaptureParameters` gains `double metersPerUnit`. The world
  stamps it before fanning capture out to forms, so form-created sub-moments
  are constructed with the correct scale.
- `VisualMoment` carries `metersPerUnit` (set at construction from the capture
  parameters) so the visual domain can convert.
- `VisualMomentImpl::Renderable` (e2-internal.h): the baked `glm::mat4 model`
  is replaced by
  - `WorldPosition          translation;`
  - `glm::quat              rotation;`
  - `Vector3<WorldLength>   scaling;`

  visual.cpp composes the float model matrix at render time, converting
  translation/scaling to meters via the moment's scale.
- `VisualMomentImpl::Light::position` becomes `WorldPosition`.
- The e2-internal 1-unit-equals-1-meter `toMeters`/`toVec3` hack is deleted;
  replaced by scale-taking helpers (or direct use of `UnitOfLength` methods).
- `Camera::Desc` is unchanged (already world units); visual.cpp converts its
  position/near/far using the moment's scale.

## 4. Call-site updates

- `Simple::createWorld(Universe &, double metersPerUnit = 1.0)`; `SimpleWorld`
  forwards the scale to the `World` ctor.
- `BoxForm` / `PointLightForm` store positions and dimensions in world units
  and emit them as-is into moments (no conversion at capture).
- Tests (`simple-world-test.cpp`) and sample (`src/sample/e2/simple-world.cpp`)
  rename `UnitOfLength` → `WorldLength`; the sample's `m()` helper returns
  `WorldLength`.

## Error handling

- `metersPerUnit` must be positive and finite. Guard in the `World` ctor with
  the engine's usual assertion/check style; no runtime fallback (a zero or
  negative scale is a programming error).
- Overflow/underflow in conversions remains the caller's responsibility, as
  documented on `UnitOfLength` today.

## Testing

- Update `simple-world-test.cpp` to the new API; it exercises the full
  capture → render path with scale 1.0.
- Add a small unit test for `UnitOfLength` conversions: round-trips at scale
  1.0, non-trivial scale (e.g. 0.001 = mm units), centimeter factor
  correctness.

## Out of scope

- Camera-relative (abs→rel) position conversion inside the visual domain —
  future work this design enables.
- Dimensional analysis of length arithmetic (`length * length` still yields
  `length`).
