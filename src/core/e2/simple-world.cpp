// simple-world.cpp — a minimal concrete world used to verify the engine2 world and
// visual-moment workflow. It contains a couple of trivial form types (a spinning box mesh
// and a point light) and a world whose blocking run() loop advances them at a fixed cadence
// while exposing thread-safe snapshot capture. The actual rendering of the captured moment
// is the responsibility of the official VisualDomain (see visual.cpp).

#include "e2-internal.h"

#include <glm/gtc/matrix_transform.hpp>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

using namespace GN;
using namespace GN::e2;

namespace GN::e2 {

void buildUnitBoxMesh(MeshData & mesh) {
    using V = MeshData::Vertex;
    // 6 faces, 4 vertices each, all sharing the face normal. Winding is CCW when viewed
    // from outside the box (front-face = CCW), matching the default raster front face.
    static const glm::vec3 kNormals[6] = {
        {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1},
    };
    // Per-face quad corners (CCW from outside), as offsets of the unit cube [-0.5, 0.5].
    static const glm::vec3 kCorners[6][4] = {
        {{0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}},     // +X
        {{-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}}, // -X
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

} // namespace GN::e2

namespace {

GN::Logger * sLogger = GN::getLogger("GN.e2.simple");

// ---------------------------------------------------------------------------
// Forms
// ---------------------------------------------------------------------------

/// A solid axis-aligned box that slowly spins around a fixed axis while the world runs.
struct BoxForm : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);

    BoxForm(Universe & u, const WorldVector3 & position, const WorldVector3 & dimensions)
        : Form(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-box"), mUniverse(u), mPosition(position), mDimensions(dimensions) {
        mMesh     = std::make_shared<MeshData>();
        mMesh->id = id;
        buildUnitBoxMesh(*mMesh);
    }

    // Advances on the world's simulation thread under the world lock.
    void update() override { mOrientation = glm::normalize(glm::angleAxis(kSpinPerTick, kSpinAxis) * mOrientation); }

    // Called by the world (under its lock) while capturing a moment.
    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters & params) override {
        auto                         moment = referenceTo(new VisualMomentImpl(mUniverse, params.metersPerUnit));
        VisualMomentImpl::Renderable r;
        r.mesh        = mMesh;
        r.translation = mPosition;
        r.rotation    = mOrientation;
        r.scaling     = mDimensions;
        r.baseColor   = mColor;
        moment->renderables.append(r);
        return moment;
    }

private:
    static constexpr float kSpinPerTick = 0.01f; // radians per simulation tick

    Universe &                    mUniverse;
    std::shared_ptr<MeshData>     mMesh;
    WorldVector3                  mPosition;
    glm::quat                     mOrientation = glm::quat(1.f, 0.f, 0.f, 0.f);
    WorldVector3                  mDimensions;
    glm::vec3                     mColor    = {0.70f, 0.74f, 0.80f};
    static inline const glm::vec3 kSpinAxis = glm::normalize(glm::vec3(0.3f, 1.0f, 0.2f));
};

/// A point light source.
struct PointLightForm : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);

    PointLightForm(Universe & u, const WorldVector3 & position, const IntensityRGB & color)
        : Form(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-point-light"), mUniverse(u), mPosition(position),
          mColor(glm::vec3(color.r, color.g, color.b) * color.intensity.value) {}

    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters & params) override {
        auto                    moment = referenceTo(new VisualMomentImpl(mUniverse, params.metersPerUnit));
        VisualMomentImpl::Light l;
        l.position = mPosition;
        l.color    = mColor;
        moment->lights.append(l);
        return moment;
    }

private:
    Universe &   mUniverse;
    WorldVector3 mPosition;
    glm::vec3    mColor; // pre-scaled by luminous intensity
};

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

    SimpleWorld(Universe & u, double metersPerUnit_): World(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-world", metersPerUnit_), mUniverse(u) {}

    void populate(ArrayView<Ref<Form>> forms) override {
        std::lock_guard<std::mutex> lock(mMutex);
        for (auto & f : forms) {
            if (f) mForms.append(f);
        }
    }

    void run() override {
        bool expected = false;
        if (!mRunning.compare_exchange_strong(expected, true)) GN_UNLIKELY {
                GN_WARN(sLogger)("SimpleWorld::run() called while already running; ignored.");
                return;
            }
        using namespace std::chrono;
        constexpr auto kTimestep = milliseconds(16); // ~60 Hz fixed timestep
        while (!mStop.load(std::memory_order_relaxed)) {
            {
                std::lock_guard<std::mutex> lock(mMutex);
                for (auto & f : mForms) f->update();
            }
            std::this_thread::sleep_for(kTimestep);
        }
    }

    void stop() override { mStop.store(true, std::memory_order_relaxed); }

    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters & paramsIn) override {
        // Stamp this world's scale so the aggregate moment and every form contribution carry it.
        auto params          = paramsIn;
        params.metersPerUnit = metersPerUnit;

        auto moment = referenceTo(new VisualMomentImpl(mUniverse, metersPerUnit));

        // Snapshot the observing cameras into the moment so the snapshot is self-contained.
        for (auto & cam : params.cameras) {
            if (cam) moment->cameras.append(cam->desc);
        }

        // Briefly freeze the simulation and let each form contribute its visual state.
        std::lock_guard<std::mutex> lock(mMutex);
        for (auto & f : mForms) {
            auto contribution = f->captureVisualMoment(params);
            if (auto * vm = RuntimeType::cast<VisualMomentImpl>(contribution.get())) moment->merge(*vm);
        }
        return moment;
    }

private:
    Universe &           mUniverse;
    std::mutex           mMutex;             // guards mForms and the live state of every form
    DynaArray<Ref<Form>> mForms;             // guarded by mMutex
    std::atomic<bool>    mRunning = {false}; // concurrent-run() guard
    std::atomic<bool>    mStop    = {false}; // game-loop stop signal
};

} // namespace

namespace GN::e2::Simple {

Ref<World> createWorld(Universe & universe, double metersPerUnit) { return referenceTo(new SimpleWorld(universe, metersPerUnit)); }

Ref<Form> createBox(Universe & universe, const WorldVector3 & position, const WorldVector3 & dimensions) {
    return referenceTo(new BoxForm(universe, position, dimensions));
}

Ref<Form> createPointLight(Universe & universe, const WorldVector3 & position, const IntensityRGB & color) {
    return referenceTo(new PointLightForm(universe, position, color));
}

} // namespace GN::e2::Simple
