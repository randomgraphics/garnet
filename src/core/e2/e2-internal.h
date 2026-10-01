#pragma once
// Internal, non-exported types shared between the engine2 (e2) implementation files.
// This header is private to src/core/e2 and is NOT part of the public e2 interface.

#include <garnet/GNengine2.h>
#include <garnet/GNrdg2.h>

namespace GN::e2 {

/// Compile the fixed outer frame skeleton used by the visual backend. Keeping this helper
/// independent of swapchain/GPU construction makes the E2-owned composition policy directly
/// testable; the four quests themselves remain generic RDG2 units.
rdg2::PlanRef compileVisualFramePlan(rdg2::QuestRef frameBegin, rdg2::QuestRef prepareSsc, rdg2::QuestRef visualRender, rdg2::QuestRef frameEnd);

// Query a form tree for facets that match, or derive from, the requested runtime type.
inline void queryFacetsByType(Form & root, const RuntimeType::TypeInfo & type, DynaArray<Ref<Facet>> & result) {
    for (auto & facet : root.facets()) {
        if (facet->typeInfo().isDerivedFrom(type)) result.append(facet);
    }
    for (auto & child : root.children()) queryFacetsByType(*child, type, result);
}

/// Storage preserves insertion order; the private scheduler groups environments and
/// overlays without exposing the collection's representation to other modules.
struct VisualTableauImpl final : VisualTableau {
    GN_REGISTER_RUNTIME_TYPE(VisualTableau);

    DynaArray<Ref<VisualMoment>> moments;

    explicit VisualTableauImpl(Universe & universe): VisualTableau(TYPE_INFO(), universe.generateUniqueIdentifier(), "visual-tableau") {}

    void add(Ref<VisualMoment> moment) override {
        if (moment) moments.append(std::move(moment));
    }

    DynaArray<Ref<VisualMoment>> orderedMoments() const;
};

} // namespace GN::e2
