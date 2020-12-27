#include "pch.h"
#include <cuda_runtime.h>
#include <optix.h>
#include <optix_function_table_definition.h>
#include <optix_stubs.h>

using namespace GN;

static auto sLogger = GN::getLogger("GN.test.optix");

const char * getCudaErrorName(CUresult r) {
    const char * s = nullptr;
    cuGetErrorName(r, &s);
    return s ? s : "unknown";
}

const char * getCudaErrorName(cudaError_t r) {
    return cudaGetErrorName(r);
}

const char * getCudaErrorString(CUresult r) {
    const char * s = nullptr;
    cuGetErrorString(r, &s);
    return s ? s : "unknown";
}

const char * getCudaErrorString(cudaError_t r) {
    return cudaGetErrorString(r);
}

#define CUDA_CHECK(x, failed) do { auto result__ = (x); if (CUDA_SUCCESS != result__) { GN_ERROR(sLogger)("CUDA function failed: (%s) %s", getCudaErrorName(result__), getCudaErrorString(result__)); failed; } } while (0)
#define CUDA_RETURN_ON_FAIL(x) CUDA_CHECK(x, return)
#define CUDA_RETURN_FALSE_ON_FAIL(x) CUDA_CHECK(x, return false)

#define OPTIX_CHECK(x, failed) do { OptixResult result__ = (x); if (OPTIX_SUCCESS != result__) { GN_ERROR(sLogger)("OptiX function failed: (%s) %s", optixGetErrorName(result__), optixGetErrorString(result__)); failed; } } while (0)
#define OPTIX_RETURN_ON_FAIL(x) OPTIX_CHECK(x, return)
#define OPTIX_RETURN_FALSE_ON_FAIL(x) OPTIX_CHECK(x, return false)

class OptixSample : public GN::util::SampleApp {

    gfx::SpriteRenderer _sprite;
    AutoRef<gfx::Texture> _texture;

    OptixDeviceContext _context = 0;

    static void contextLogCallback( unsigned int level, const char* tag, const char* message, void* /*cbdata */)
    {
        std::cerr << "[OPTIX][" << std::setw( 2 ) << level << "][" << std::setw( 12 ) << tag << "]: "
        << message << "\n";
    }

public:

    OptixSample() {}

    bool onPreInit( InitParam & )
    {
        return true;
    }

    bool onInit()
    {
        // create graphics tuff
        auto g = engine::getGpu();
        if (!_sprite.init(*g)) return false;
        auto w = g->getDispDesc().width;
        auto h = g->getDispDesc().height;
        _texture = g->create2DTexture(gfx::ColorFormat::RGBA8, w, h);

        // Initialize CUDA
        CUDA_RETURN_FALSE_ON_FAIL(cudaFree(0));

        // initialize optix
        OPTIX_RETURN_FALSE_ON_FAIL(optixInit());
        OptixDeviceContextOptions options = {};
        options.logCallbackFunction       = &contextLogCallback;
        options.logCallbackLevel          = 4;
        OPTIX_RETURN_FALSE_ON_FAIL(optixDeviceContextCreate(0, &options, &_context));

        // create a output buffer

        // done
        return true;
    }

    void onQuit() {
        if (_context) {
            optixDeviceContextDestroy(_context);
            _context = 0;
        }
        _texture.clear();
        _sprite.quit();
    }

    void onKeyPress(input::KeyEvent key) {
        SampleApp::onKeyPress(key);
    }

    void onUpdate() {
    }

    void onRender() {
        auto g = engine::getGpu();
        g->clearScreen();
        //OPTIX_RETURN_ON_FAIL(optixLaunch( pipeline, stream, d_param, sizeof( Params ), &sbt, width, height, /*depth=*/1 ));
        //CUDA_RETURN_ON_FAIL(cudaDeviceSynchronize());
    }
};

int main(int argc, char * argv[]) {
    GN::enableCRTMemoryCheck();
    OptixSample app;
    return app.run( argc, argv );
}
