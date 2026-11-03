#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

/// Optional GNfiz collider capability. The collider is independent of VisualFacet geometry.
/// Transformed box/convex hulls and static triangle meshes are baked through the full affine path.
/// Smooth primitives under non-uniform affine transforms need an explicit proxy hull.
struct PhysicsFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
        AutoRef<fiz::Hull>  collider;
        fiz::MotionType     motionType = fiz::MotionType::STATIC;
        fiz::Temper         temper;
        fiz::CollisionLayer layer        = fiz::CollisionLayer::NON_MOVING;
        fiz::Scalar         massOverride = 0;
        bool                allowSleep   = true;
        bool                ccd          = false;
        DynaArray<FormId>   touching; ///< Sorted body partners from the most recent GNfiz step.
    };

    explicit PhysicsFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}
    GN_API bool                   validate(const PrimeView &, FormId, const FacetValue &) const override;
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet>                    clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new PhysicsFacet(*this));
    }
    DynaArray<const RuntimeType::TypeInfo *> requirements() const override { return {&TransformFacet::TYPE_INFO()}; }
};

struct GNfizLawOptions {
    PhysicalScale scale = PhysicalScale::NANOMETER();
    WorldVector3  origin {WorldCoordinate::ZERO(), WorldCoordinate::ZERO(), WorldCoordinate::ZERO()};
    glm::dvec3    gravity {0, -9.81, 0};
    uint32_t      workerThreads = 1; ///< Fixes solver scheduling; E2 sample 1/4-worker replay matched on Windows, cross-platform equality is unverified.
};

/// Register the optional GNfiz collider capability.
GN_API bool registerPhysicsFacet(World &);

/// Create a GNfiz rigid-body Law. It reads PhysicsFacet, TransformFacet and MotionFacet for dynamic bodies,
/// owns its backend engine, and commits resulting poses/velocities through Prime/Slate. Transform edits recook
/// transformed collider geometry when needed. Call registerPhysicsFacet() and register the Transform/Motion facets.
GN_API Ref<Law> createGNfizLaw(const GNfizLawOptions & = {});

} // namespace GN::e2
