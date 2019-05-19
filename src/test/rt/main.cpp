#include "pch.h"
#include <garnet/GNrt.h>
#include <garnet/GNutil.h>
#include <cli/CLI.hpp>
#include <chrono>

using namespace GN;
using namespace GN::gfx;
using namespace GN::rt;

struct Scene
{
    // currently, rect light only
    struct Light
    {
        Vec4 position;
        Vec4 edges[2];
    };

    template<typename T>
    struct TypedSurface
    {
        AutoRef<GN::gfx::Gpu2::Surface> g;
        std::vector<T>                  c;

        T & operator[](size_t i) { return c[i]; }
        const T & operator[](size_t i) const { return c[i]; }
    };

    TypedSurface<Vec4>  positions; // w channel is primitive ID
    TypedSurface<Vec4>  normals;   // w channel is material ID
    TypedSurface<AABB>  bvh;
    TypedSurface<Light> lights;

    ~Scene() { Cleanup(); }

    void Cleanup()
    {

    }

    void Load(const std::string &)
    {

    }

    struct DrawFrameParameters
    {
        Matrix44f proj;
        Matrix44f view;
        float     frameDuration;
    };

    void DrawFrame(const DrawFrameParameters &)
    {
        
    }
};

struct OrbitCamera
{
    GN::util::ArcBall arcball;
    Matrix44f proj;
    Matrix44f view;

public:

    OrbitCamera()
    {
        // TODO: hook with gInput signals,
    }

    void reset(const AABB & bb)
    {
        // setup default view and proj matrices.
        float radius = bb.GetDiagonalDistance();
        view.lookAtRh(GN::Vector3f(0,0,radius), GN::Vector3f(0,0,0), GN::Vector3f(0,1,0));
        proj.perspectiveD3D(GN_PI/4.0f, 16.0f/9.0f, radius / 100.0f, radius * 2.0f);
    }
};


int main( int argc, const char * argv[] )
{
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
    s.Load({filename});

    // main camera
    OrbitCamera camera;
    camera.reset(s.bvh[0]);

    // start the main loop
    float frameDuration = 1.0f / 30.0f;
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    while(win->runUntilNoNewEvents()) {

        gInput.processInputEvents();

        s.DrawFrame({
            camera.view,
            camera.proj,
        });

        gpu->present({});

        // calculate frame time
        auto now = std::chrono::high_resolution_clock::now();
        frameDuration = (float)std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrameTime).count() / 1e6f;
        lastFrameTime = now;
    }
}
