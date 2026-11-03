#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

/// Spatial hierarchy capability. Root positions are world-space; child positions are parent-local.
/// Attach Value through a Mold and call resolveWorldTransform() for derived world poses.
struct GN_API TransformFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    /// Versioned local pose payload; uniform scale remains the common compact case.
    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
        FormId       parent = 0; ///< Zero selects world space; otherwise this Form supplies the parent frame, not lifetime ownership.
        WorldVector3 position {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()}; ///< Absolute at the root, parent-local otherwise.
        Rotation     orientation {1, 0, 0, 0}; ///< Unit quaternion from object space to the parent/world frame.
        glm::vec3    scale {1.f};              ///< Uniform scale uses the fast path; non-uniform scale enables affine fallback.
        bool         affineOverride = false;   ///< Rare simulation output path; when set, affineLinear replaces orientation*scale locally.
        glm::dmat3   affineLinear {1.0};       ///< Local full-affine matrix used only when affineOverride is true.
    };

    explicit TransformFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}
    GN_API bool                   validate(const PrimeView &, FormId, const FacetValue &) const override;
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                    clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new TransformFacet(*this));
    }

    /// Read this capability's value for an individual in an explicit committed version.
    const Value * read(const Form & form, const PrimeView & view) const { return view.get<TransformFacet>(form.formId()); }
};

/// Resolved world pose; affineLinear is authoritative when non-uniform scale introduces shear.
struct WorldTransform {
    WorldVector3 position {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()}; ///< Absolute world-unit translation.
    Rotation     orientation {1, 0, 0, 0}; ///< Composed rotation; affineLinear is authoritative if shear is present.
    float        uniformScale = 1.f;       ///< Fast path used until non-uniform scale/shear enters the hierarchy.
    glm::dmat3   affineLinear {1.0};       ///< Double-precision object-to-world linear part when affineFallback is true.
    bool         affineFallback = false;
};

/// Resolve a Form's world pose from an immutable Prime version. False for invalid chains or unsupported affine ranges.
GN_API bool resolveWorldTransform(const PrimeView &, FormId, WorldTransform &);

/// Return children of a spatial parent in Prime traversal order. Zero selects spatial roots.
GN_API DynaArray<FormId> transformChildren(const PrimeView &, FormId parent);

/// Convert a local metric offset without converting the large absolute origin to floating point.
GN_API WorldVector3 positionFromMeters(const glm::dvec3 &, PhysicalScale,
                                       const WorldVector3 & origin = {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()});

/// Rebase first, then convert to metres. Intended for a bounded simulation or presentation region.
GN_API glm::dvec3 positionToMeters(const WorldVector3 &, PhysicalScale,
                                   const WorldVector3 & origin = {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()});

} // namespace GN::e2
