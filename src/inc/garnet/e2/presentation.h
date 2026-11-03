#if !defined(__GN_INSIDE_ENGINE2_H__)
    #error "Include <garnet/GNengine2.h> instead."
#endif

namespace GN::e2 {

/// Data-only mesh presentation capability. Geometry choice and local scale belong to each Form.
/// The current basis renderer consumes this data as a simple lit white model without owning Prime.
struct VisualFacet : Facet {
    GN_API GN_REGISTER_RUNTIME_TYPE(Facet);

    struct Value : FacetValue {
        GN_API GN_REGISTER_RUNTIME_TYPE(FacetValue);
        Value(const RuntimeType::TypeInfo & type = TYPE_INFO()): FacetValue(type) {}
        Ref<FacetValue> clone() const override {
            if (typeInfo().id != TYPE_INFO().id) return {};
            return Ref<FacetValue>(new Value(*this));
        }
        basis::Assets::MeshId meshId {nullptr};
        glm::vec3 scale {0.25f}; ///< Local mesh scale, independent from the collision shape.
    };

    explicit VisualFacet(const RuntimeType::TypeInfo & type = TYPE_INFO()): Facet(type) {}
    const RuntimeType::TypeInfo & valueType() const override { return Value::TYPE_INFO(); }
    Ref<Facet> clone() const override {
        if (typeInfo().id != TYPE_INFO().id) return {};
        return Ref<Facet>(new VisualFacet(*this));
    }

    /// Read this capability's value for an individual in an explicit committed version.
    const Value * read(const Form & form, const PrimeView & view) const { return view.get<VisualFacet>(form.formId()); }
};

/// Extract immutable mesh instances from a committed Prime snapshot for presentation.
GN_API basis::Visual::Tableau extractTableau(const PrimeView &, const basis::Visual::Camera &, PhysicalScale = PhysicalScale::NANOMETER());

} // namespace GN::e2
