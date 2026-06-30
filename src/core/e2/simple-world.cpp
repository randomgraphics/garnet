// simple-world.cpp — a minimal concrete world used to verify the engine2 world and
// visual-moment workflow. It contains a couple of trivial form types (a spinning box mesh
// and a point light) and a world that advances them on its own background-thread cadence
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

    BoxForm(Universe & u, const WorldPosition & position, const Vector3<UnitOfLength> & dimensions)
        : Form(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-box"), mUniverse(u), mPosition(toVec3(position)),
          mDimensions(toMeters(dimensions.x), toMeters(dimensions.y), toMeters(dimensions.z)) {
        mMesh     = std::make_shared<MeshData>();
        mMesh->id = id;
        buildUnitBoxMesh(*mMesh);
    }

    // Advances on the world's simulation thread under the world lock.
    void update() override {
        mOrientation = glm::normalize(glm::angleAxis(kSpinPerTick, kSpinAxis) * mOrientation);
    }

    // Called by the world (under its lock) while capturing a moment.
    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) override {
        glm::mat4 model = glm::translate(glm::mat4(1.f), mPosition) * glm::mat4_cast(mOrientation) * glm::scale(glm::mat4(1.f), mDimensions);

        auto                       moment = referenceTo(new VisualMomentImpl(mUniverse));
        VisualMomentImpl::Renderable r;
        r.mesh      = mMesh;
        r.model     = model;
        r.baseColor = mColor;
        moment->renderables.append(r);
        return moment;
    }

private:
    static constexpr float kSpinPerTick = 0.01f; // radians per simulation tick

    Universe &                  mUniverse;
    std::shared_ptr<MeshData>   mMesh;
    glm::vec3                   mPosition;
    glm::quat                   mOrientation = glm::quat(1.f, 0.f, 0.f, 0.f);
    glm::vec3                   mDimensions;
    glm::vec3                   mColor    = {0.70f, 0.74f, 0.80f};
    static inline const glm::vec3 kSpinAxis = glm::normalize(glm::vec3(0.3f, 1.0f, 0.2f));
};

/// A point light source.
struct PointLightForm : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);

    PointLightForm(Universe & u, const WorldPosition & position, const IntensityRGB & color)
        : Form(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-point-light"), mUniverse(u), mPosition(toVec3(position)),
          mColor(glm::vec3(color.r, color.g, color.b) * color.intensity.value) {}

    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters &) override {
        auto                    moment = referenceTo(new VisualMomentImpl(mUniverse));
        VisualMomentImpl::Light l;
        l.position = mPosition;
        l.color    = mColor;
        moment->lights.append(l);
        return moment;
    }

private:
    Universe & mUniverse;
    glm::vec3  mPosition;
    glm::vec3  mColor; // pre-scaled by luminous intensity
};

// ---------------------------------------------------------------------------
// World
// ---------------------------------------------------------------------------
//
// Threading model: run() starts a dedicated simulation thread that advances every form at
// a fixed timestep. populate() and captureVisualMoment() may be called from any thread; the
// world's mutex is the synchronization boundary that guards both the form collection and
// every form's live state, so a captured moment is always internally consistent. The
// renderer observes the world only through these self-contained snapshots, never through
// the live forms, which is what lets simulation and rendering run at independent rates.

struct SimpleWorld : World {
    GN_REGISTER_RUNTIME_TYPE(World);

    explicit SimpleWorld(Universe & u): World(TYPE_INFO(), u.generateUniqueIdentifier(), "simple-world"), mUniverse(u) {}

    ~SimpleWorld() override {
        mStop.store(true, std::memory_order_relaxed);
        if (mSimThread.joinable()) mSimThread.join();
    }

    void populate(ArrayView<Ref<Form>> forms) override {
        std::lock_guard<std::mutex> lock(mMutex);
        for (auto & f : forms) {
            if (f) mForms.append(f);
        }
    }

    void run() override {
        bool expected = false;
        if (!mRunning.compare_exchange_strong(expected, true)) {
            GN_WARN(sLogger)("SimpleWorld::run() called more than once; ignored.");
            return;
        }
        mSimThread = std::thread([this] { simLoop(); });
    }

    Ref<VisualMoment> captureVisualMoment(const VisualMoment::CaptureParameters & params) override {
        auto moment = referenceTo(new VisualMomentImpl(mUniverse));

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
    void simLoop() {
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

    Universe &           mUniverse;
    std::mutex           mMutex;             // guards mForms and the live state of every form
    DynaArray<Ref<Form>> mForms;             // guarded by mMutex
    std::thread          mSimThread;
    std::atomic<bool>    mRunning = {false}; // run() guard
    std::atomic<bool>    mStop    = {false}; // simulation-thread stop signal
};

} // namespace

namespace GN::e2::Simple {

Ref<World> createWorld(Universe & universe) { return referenceTo(new SimpleWorld(universe)); }

Ref<Form> createBox(Universe & universe, const WorldPosition & position, const Vector3<UnitOfLength> & dimensions) {
    return referenceTo(new BoxForm(universe, position, dimensions));
}

Ref<Form> createPointLight(Universe & universe, const WorldPosition & position, const IntensityRGB & color) {
    return referenceTo(new PointLightForm(universe, position, color));
}

} // namespace GN::e2::Simple
