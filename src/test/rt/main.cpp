#include "pch.h"
#include <garnet/GNrt.h>
#include <garnet/GNutil.h>
#include <garnet/gfx/fatModel.h>
#include <CLI/CLI.hpp>
#include <chrono>

using namespace GN;
using namespace GN::gfx;
using namespace GN::rt;

enum BuiltInMaterial : uint32_t {
    LAMBERT_WHITE,
    LAMBERT_RED,
    LAMBERT_GREEN,
    LAMBERT_BLUE,
    LAMBERT_YELLOW,
};

std::vector<FatMaterial> GetBuiltInMaterials() {
    return {
        {"", "", "", {1.0, 1.0, 1.0}}, // white
        {"", "", "", {1.0, 0.0, 0.0}}, // red
        {"", "", "", {0.0, 1.0, 0.0}}, // green
        {"", "", "", {0.0, 0.0, 1.0}}, // blue
        {"", "", "", {1.0, 1.0, 0.0}}, // yellow
    };
};

static void AddRectFace(FatModel & model, const Eigen::Vector3f * v, int a, int b, int c, int d, const Eigen::Vector3f & normal, uint32_t material) {
    model.meshes.resize(model.meshes.size() + 1);
    auto & mesh = model.meshes.back();

    mesh.vertices.resize(FatVertexBuffer::POS_NORMAL, 6);
    mesh.vertices.setElementFormat(FatVertexBuffer::POSITION, PixelFormat::FLOAT3());
    mesh.vertices.setElementFormat(FatVertexBuffer::NORMAL, PixelFormat::FLOAT3());
    auto p    = mesh.vertices.getPosition();
    auto n    = mesh.vertices.getNormal();
    p[0].v3() = v[a];
    p[1].v3() = v[b];
    p[2].v3() = v[c];
    p[3].v3() = v[a];
    p[4].v3() = v[c];
    p[5].v3() = v[d];
    for (int i = 0; i < 6; ++i, ++n) n->v3() = normal;

    // create one subset
    mesh.subsets.append({material, 0, 6});
}

static void CreateCornellBox(FatModel & m, float dimension) {
    float           l   = -dimension / 2.0f; // left
    float           r   = +dimension / 2.0f; // right
    float           t   = +dimension / 2.0f; // top
    float           b   = -dimension / 2.0f; // bottom
    float           f   = +dimension / 2.0f; // front
    float           k   = -dimension / 2.0f; // back
    Eigen::Vector3f v[] = {
        {l, b, f}, {r, b, f}, {r, t, f}, {l, t, f}, {l, b, k}, {r, b, k}, {r, t, k}, {l, t, k},
    };
    AddRectFace(m, v, 5, 4, 7, 6, {0, 0, 1}, LAMBERT_WHITE);  // back
    AddRectFace(m, v, 3, 2, 6, 7, {0, -1, 0}, LAMBERT_WHITE); // top
    AddRectFace(m, v, 4, 5, 1, 0, {0, 1, 0}, LAMBERT_WHITE);  // bottom
    AddRectFace(m, v, 4, 0, 3, 7, {1, 0, 0}, LAMBERT_RED);    // left
    AddRectFace(m, v, 1, 5, 6, 2, {-1, 0, 0}, LAMBERT_GREEN); // right
}

FatModel LoadSimpleCornellBox() {
    FatModel m;
    m.materials.append(GetBuiltInMaterials());
    CreateCornellBox(m, 2.0f);
    return m;
}

struct Scene {
    // currently, rect light only
    struct Light {
        Vec4 position;
        Vec4 edges[2];
    };

    struct Position : public Eigen::Vector3f {
        int primitive;
    };

    struct Normal : public Eigen::Vector3f {
        int material;
    };

    struct Material {
        enum Type : int {
            DIFFUSE,
            DIELETRIC,
            METAL,
            LIGHT,
        };
        float albedo[3] = {.0f, .0f, .0f}; // this is emmisive for light
        float roughness = .0f;             // ignored for light
        float refindex  = .0f;             // only for dieletric material
        Type  type;
        int : 32;
        int : 32;
    };

    template<typename T>
    struct TypedSurface {
        AutoRef<GN::gfx::Gpu2::Surface> g;
        DynaArray<T>                    c;

        void clear() {
            g.clear();
            c.clear();
        }

        T &       operator[](size_t i) { return c[i]; }
        const T & operator[](size_t i) const { return c[i]; }
    };

    TypedSurface<Material> materials;
    TypedSurface<Position> positions;
    TypedSurface<Normal>   normals;
    TypedSurface<AABB>     bvh;
    TypedSurface<Light>    lights;

    ~Scene() { Cleanup(); }

    void Cleanup() {
        materials.clear();
        positions.clear();
        normals.clear();
        bvh.clear();
        lights.clear();
    }

    bool Load(Gpu2 *, const FatModel &) {
        // done
        return true;
    }

    struct DrawFrameParameters {
        Matrix44f proj {};
        Matrix44f view {};
        float     frameDuration {};
    };

    void DrawFrame(const DrawFrameParameters &) {}
};

struct OrbitCamera {
    GN::util::ArcBall arcball {};
    Matrix44f         proj {};
    Matrix44f         view {};

public:
    OrbitCamera() {
        // TODO: hook with gInput signals,
    }

    void reset(const AABB & bb) {
        // setup default view and proj matrices.
        float radius = bb.GetDiagonalDistance();
        view.lookAtRh(GN::Vector3f(0, 0, radius), GN::Vector3f(0, 0, 0), GN::Vector3f(0, 1, 0));
        proj.perspectiveD3D(GN_PI / 4.0f, 16.0f / 9.0f, radius / 100.0f, radius * 2.0f);
    }
};

int main(int argc, const char * argv[]) {
    std::string filename;

    CLI::App app;
    app.add_option("filename", filename, "scene file name")->check(CLI::ExistingFile);
    CLI11_PARSE(app, argc, argv);

    // Create new window
    AutoObjPtr<GN::win::Window> win(GN::win::createWindow({"GNrt", 1280, 720}));

    // initalize input manager
    GN::input::initializeInputSystem();

    // create Gpu2
    auto gpu = GN::gfx::Gpu2::createGpu2({win});

    // initialie the scene
    Scene s;
    // if (!s.Load(filename)) return -1;
    if (!s.Load(gpu, LoadSimpleCornellBox())) return -1;

    // main camera
    OrbitCamera camera;
    camera.reset(s.bvh[0]);

    // start the main loop
    // float frameDuration = 1.0f / 30.0f;
    // auto  lastFrameTime = std::chrono::high_resolution_clock::now();
    while (win->runUntilNoNewEvents()) {

        gInput.processInputEvents();

        s.DrawFrame({
            camera.view,
            camera.proj,
        });

        gpu->present({});

        // // calculate frame time
        // auto now      = std::chrono::high_resolution_clock::now();
        // frameDuration = (float) std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrameTime).count() / 1e6f;
        // lastFrameTime = now;
    }
}
