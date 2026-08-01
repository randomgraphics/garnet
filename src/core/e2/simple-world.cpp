// simple-world.cpp — a minimal concrete world used to verify the engine2 world and
// visual-moment workflow. Behavior and visible state live in facets attached to plain
// structural forms: a spinning box (spin behavior + box mesh) and a point light. The box is
// cast from a Mold recipe to exercise that workflow; the light is assembled directly. The
// world's blocking run() loop advances the form trees at a fixed cadence while exposing
// thread-safe snapshot capture. The actual rendering of the captured moment is the
// responsibility of the official VisualDomain (see visual.cpp).

#include "e2-internal.h"

#include <glm/gtc/matrix_transform.hpp>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

using namespace GN;
using namespace GN::e2;

namespace {

GN::Logger * sLogger = GN::getLogger("GN.e2.simple");

void buildUnitBoxMesh(MeshData & mesh) {
    using V = MeshData::Vertex;
    // 6 faces, 4 vertices each, all sharing the face normal. Winding is CCW when viewed
    // from outside the box (front-face = CCW), matching the default raster front face.
    static const glm::vec3 kNormals[6] = {
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1},
    };
    // Per-face quad corners (CCW from outside), as offsets of the unit cube [-0.5, 0.5].
    static const glm::vec3 kCorners[6][4] = {
        {{0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}},     // +X
        {{-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}}, // -X
        {{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}},     // +Y
        {{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}}, // -Y
        {{-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}},     // +Z
        {{0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}}, // -Z
    };

    mesh.vertices.clear();
    mesh.indices.clear();
    for (uint32_t f = 0; f < 6; ++f) {
        uint16_t base = (uint16_t) mesh.vertices.size();
        for (uint32_t c = 0; c < 4; ++c) mesh.vertices.append(V {kCorners[f][c], kNormals[f]});
        const uint16_t quad[6] = {base, uint16_t(base + 1), uint16_t(base + 2), base, uint16_t(base + 2), uint16_t(base + 3)};
        for (uint16_t i : quad) mesh.indices.append(i);
    }
}

// ---------------------------------------------------------------------------
// Facets
// ---------------------------------------------------------------------------

/// Slowly spins the owning form around a fixed axis while the world runs, so that motion is
/// visible across captured visual moments.
struct SpinnerFacet : Facet {
    GN_REGISTER_RUNTIME_TYPE(Facet);

    SpinnerFacet(Universe & u): Facet(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-spinner") {}

    // Advances on the world's simulation thread under the world lock.
    void live() override {
        auto * f = form();
        if (!f) GN_UNLIKELY return;
        f->setRotation(glm::normalize(glm::angleAxis(kSpinPerTick, kSpinAxis) * f->rotation()));
    }

private:
    static constexpr float kSpinPerTick = 0.01f; // radians per simulation tick

    static inline const glm::vec3 kSpinAxis = glm::normalize(glm::vec3(0.3f, 1.0f, 0.2f));
};

/// Contributes a solid box mesh rendered at the owning form's world transform.
struct BoxMeshFacet : VisualFacet {
    GN_REGISTER_RUNTIME_TYPE(VisualFacet);

    BoxMeshFacet(Universe & u, const LocalVector3 & dimensions)
        : VisualFacet(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-box-mesh"), mDimensions(dimensions) {
        mMesh     = std::make_shared<MeshData>();
        mMesh->id = id;
        buildUnitBoxMesh(*mMesh);
    }

    // Called by the world (under its lock) while capturing a moment.
    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) override {
        auto * f = form();
        auto * w = f ? f->world() : nullptr;
        if (!w) GN_UNLIKELY {
                GN_WARN(sLogger, "box mesh facet is not attached to a form living in a world; nothing to capture.");
                return {};
            }
        auto                         moment = referenceTo(new VisualMomentImpl(w->universe, w->scale));
        VisualMomentImpl::Renderable r;
        r.mesh        = mMesh;
        r.translation = f->worldPosition();
        r.rotation    = f->worldRotation();
        r.scaling     = mDimensions;
        r.baseColor   = mColor;
        moment->renderables.append(r);
        return moment;
    }

private:
    std::shared_ptr<MeshData> mMesh;
    LocalVector3              mDimensions;
    glm::vec3                 mColor = {0.70f, 0.74f, 0.80f};
};

/// Contributes a point light emitting from the owning form's world position.
struct PointLightFacet : VisualFacet {
    GN_REGISTER_RUNTIME_TYPE(VisualFacet);

    PointLightFacet(Universe & u, const IntensityRGB & color)
        : VisualFacet(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-point-light"), mColor(glm::vec3(color.r, color.g, color.b) * color.intensity.value) {}

    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) override {
        auto * f = form();
        auto * w = f ? f->world() : nullptr;
        if (!w) GN_UNLIKELY {
                GN_WARN(sLogger, "point light facet is not attached to a form living in a world; nothing to capture.");
                return {};
            }
        auto                    moment = referenceTo(new VisualMomentImpl(w->universe, w->scale));
        VisualMomentImpl::Light l;
        l.position = f->worldPosition();
        l.color    = mColor;
        moment->lights.append(l);
        return moment;
    }

private:
    glm::vec3 mColor; // pre-scaled by luminous intensity
};

// ---------------------------------------------------------------------------
// Molds
// ---------------------------------------------------------------------------

/// Define the reusable box recipe: a structural form carrying a box mesh facet and a spin
/// behavior facet. Casting the mold yields a fresh, independently spinning box each time.
/// Per-instance state such as position is set on the cast form, not baked into the recipe.
Ref<Mold> createBoxMold(Universe & universe, const LocalVector3 & dimensions) {
    return Mold::create(universe, "simple-box-mold", [dimensions](const Mold::CreateParameters & cp) -> Ref<Form> {
        auto form = Form::create(cp.universe, cp.name);
        if (!form) GN_UNLIKELY return {};
        if (!form->addFacet(referenceTo(new BoxMeshFacet(cp.universe, dimensions)))) GN_UNLIKELY return {};
        if (!form->addFacet(referenceTo(new SpinnerFacet(cp.universe)))) GN_UNLIKELY return {};
        return form;
    });
}

// ---------------------------------------------------------------------------
// World
// ---------------------------------------------------------------------------
//
// Threading model: run() executes the fixed-timestep simulation loop synchronously on the
// calling thread until stop() is called — the world owns no thread; the caller decides where
// the loop lives. populate() and captureVisualMoment() may be called from any thread; the
// world's mutex is the synchronization boundary that guards both the form collection and
// every form's live state, so a captured moment is always internally consistent. The
// renderer observes the world only through these self-contained snapshots, never through
// the live forms, which is what lets simulation and rendering run at independent rates.

struct SimpleWorld : World {
    GN_REGISTER_RUNTIME_TYPE(World);

    SimpleWorld(Universe & u, PhysicalScale scale_): World(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-world", {.universe = u, .scale = scale_}) {}

    // Symmetric with populate(): forms are ref-counted and may outlive the world, so clear their
    // back-pointers before the world goes away.
    ~SimpleWorld() {
        for (auto & f : mForms) f->leaveWorld(*this);
    }

    void populate(ArrayView<Ref<Form>> forms) override {
        std::lock_guard<std::mutex> lock(mMutex);
        for (auto & f : forms) {
            if (!f) continue;
            if (!f->enterWorld(*this)) GN_UNLIKELY {
                    GN_WARN(sLogger, "form is already attached to a world; ignored.");
                    continue;
                }
            mForms.append(f);
        }
    }

    void run() override {
        bool expected = false;
        if (!mRunning.compare_exchange_strong(expected, true)) GN_UNLIKELY {
                GN_WARN(sLogger, "SimpleWorld::run() called while already running; ignored.");
                return;
            }
        using namespace std::chrono;
        constexpr auto kTimestep = milliseconds(16); // ~60 Hz fixed timestep
        while (!mStop.load(std::memory_order_relaxed)) {
            {
                std::lock_guard<std::mutex> lock(mMutex);
                for (auto & f : mForms) letFormTreeLive(*f);
            }
            std::this_thread::sleep_for(kTimestep);
        }
    }

    void stop() override { mStop.store(true, std::memory_order_relaxed); }

    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters & params) override {
        auto moment = referenceTo(new VisualMomentImpl(universe, scale));

        // Snapshot the observing cameras into the moment so the snapshot is self-contained.
        for (auto & cam : params.cameras) {
            if (cam) moment->cameras.append(cam->desc);
        }

        // Briefly freeze the simulation and let each visual facet contribute its state.
        std::lock_guard<std::mutex> lock(mMutex);
        DynaArray<Ref<Facet>>       visualFacets;
        for (auto & f : mForms) queryFacetsByType(*f, VisualFacet::TYPE_INFO(), visualFacets);
        for (auto & facet : visualFacets) {
            auto * visual = RuntimeType::cast<VisualFacet>(facet.get());
            GN_ASSERT(visual);
            auto contribution = visual->captureVisualMoment(params);
            if (auto * vm = RuntimeType::cast<VisualMomentImpl>(contribution.get())) moment->merge(*vm);
        }
        return moment;
    }

private:
    std::mutex           mMutex;             // guards mForms and the live state of every form
    DynaArray<Ref<Form>> mForms;             // guarded by mMutex
    std::atomic<bool>    mRunning = {false}; // concurrent-run() guard
    std::atomic<bool>    mStop    = {false}; // game-loop stop signal

    static void letFormTreeLive(Form & form) {
        form.live();
        for (auto & child : form.children()) letFormTreeLive(*child);
    }
};

} // namespace

namespace GN::e2::Simple {

Ref<World> createWorld(Universe & universe, PhysicalScale scale) { return referenceTo(new SimpleWorld(universe, scale)); }

Ref<Form> createBox(Universe & universe, const WorldVector3 & position, const LocalVector3 & dimensions) {
    // Exercise the Mold workflow: define the box recipe first, then cast a form from it.
    auto mold = createBoxMold(universe, dimensions);
    if (!mold) GN_UNLIKELY return {};
    auto form = mold->cast(universe, "simple-box");
    if (!form) GN_UNLIKELY return {};
    form->setPosition(position);
    return form;
}

Ref<Form> createPointLight(Universe & universe, const WorldVector3 & position, const IntensityRGB & color) {
    auto form = Form::create(universe, "simple-point-light");
    form->setPosition(position);
    form->addFacet(referenceTo(new PointLightFacet(universe, color)));
    return form;
}

} // namespace GN::e2::Simple
