#include "e2-internal.h"

namespace GN::e2 {

namespace {

GN::Logger * sLogger = GN::getLogger("GN.e2.model");

struct ModelVisualFacetImpl final : ModelVisualFacet {
    GN_REGISTER_RUNTIME_TYPE(ModelVisualFacet);

    ModelVisualFacetImpl(Universe & universe, AutoRef<const fx2::ModelScene> model)
        : ModelVisualFacet(TYPE_INFO(), universe.generateUniqueIdentifier(), "model-visual"), mModel(std::move(model)) {}

    AutoRef<const fx2::ModelScene> model() const override { return mModel; }

    void setVisible(bool visible) override { mVisible.store(visible, std::memory_order_relaxed); }
    bool visible() const override { return mVisible.load(std::memory_order_relaxed); }

    Ref<VisualMoment> snapshot(const VisualTableau::SnapshotParameters &) override {
        if (!visible()) return {};
        Form *  owner = form();
        World * world = owner ? owner->world() : nullptr;
        if (!world) GN_UNLIKELY {
                GN_WARN(sLogger, "model facet is not attached to a form living in a world; nothing to capture");
                return {};
            }

        auto                         moment = referenceTo(new VisualMomentImpl(world->universe, world->scale));
        VisualMomentImpl::Renderable renderable;
        renderable.model       = mModel;
        renderable.translation = owner->worldPosition();
        renderable.rotation    = owner->worldRotation();
        moment->renderables.append(std::move(renderable));
        return moment;
    }

private:
    AutoRef<const fx2::ModelScene> mModel;
    std::atomic_bool               mVisible = true;
};

} // namespace

Ref<ModelVisualFacet> ModelVisualFacet::create(const CreateParameters & parameters) {
    if (!parameters.model) {
        GN_ERROR(sLogger, "ModelVisualFacet::create requires a model");
        return {};
    }
    return referenceTo(new ModelVisualFacetImpl(parameters.universe, parameters.model));
}

Ref<Form> createModelForm(Universe & universe, const StrA & name, AutoRef<const fx2::ModelScene> model) {
    auto form  = Form::create(universe, name);
    auto facet = ModelVisualFacet::create({.universe = universe, .model = std::move(model)});
    if (!form || !facet || !form->addFacet(facet)) return {};
    return form;
}

} // namespace GN::e2
