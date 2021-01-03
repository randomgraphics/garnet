#include "pch.h"
#include <cuda_runtime.h>
#include <optix.h>
#include <optix_function_table_definition.h>
#include <optix_stubs.h>
#include <optix_stack_size.h>

#include "ptx.h"
#include "shared.h"

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

#define CUDA_CHECK( x, failed )         do { auto result__ = (x); if( CUDA_SUCCESS != result__ ) { GN_ERROR(sLogger)( "CUDA function failed: (%s) %s", getCudaErrorName( result__ ), getCudaErrorString( result__ ) ); failed; } } while ( 0 )
#define CUDA_RETURN_ON_FAIL( x )        CUDA_CHECK( x, return )
#define CUDA_RETURN_FALSE_ON_FAIL( x )  CUDA_CHECK( x, return false )

#define OPTIX_CHECK( x, failed )        do { OptixResult result__ = (x); if( OPTIX_SUCCESS != result__ ) { GN_ERROR(sLogger)( "OptiX function failed: (%s) %s", optixGetErrorName( result__ ), optixGetErrorString( result__ ) ); failed; } } while ( 0 )
#define OPTIX_RETURN_ON_FAIL( x )       OPTIX_CHECK( x, return )
#define OPTIX_RETURN_FALSE_ON_FAIL( x ) OPTIX_CHECK( x, return false )

template<typename T>
class CudaBuffer {
    T * _d    = 0; ///< device pointer
    T * _h    = 0; ///< host pointer
    size_t _s = 0; ///< count of elements;

public:

    ~CudaBuffer() { quit(); }

    bool init(size_t count, const T * initialContent = nullptr) {
        quit();
        _s = count * sizeof( T );
        CUDA_RETURN_FALSE_ON_FAIL( cudaMalloc( &_d, _s ) );
        _h = (T*)malloc( _s ); // TODO: check OOM
        if (initialContent) {
            memcpy( _h, initialContent, _s );
            copyToDevice();
        }
        return true;
    }

    void quit() {
        if( _d ) cudaFree( _d ), _d = 0;
        if( _h ) ::free( _h ), _h = nullptr;
        _s = 0;
    }

    void copyToDevice() {
        CUDA_CHECK( cudaMemcpy( _d, _h, _s, cudaMemcpyHostToDevice ), );
    }

    void copyToHost() {
        CUDA_CHECK( cudaMemcpy( _h, _d, _s, cudaMemcpyDeviceToHost ), );
    }

    size_t size() const { return _s; }

    T * d() const { return _d; }
    T * h() const { return _h; }
};

struct OptixStats {

    struct AutoContext {
        OptixDeviceContext c = 0;
        ~AutoContext() { quit(); }
        bool init() {
            quit();
            OptixDeviceContextOptions options = {};
            options.logCallbackFunction       = &contextLogCallback;
            options.logCallbackLevel          = 4;
            OPTIX_RETURN_FALSE_ON_FAIL( optixDeviceContextCreate( 0, &options, &c ) );
            return true;
        }
        void quit() {
            if( c ) optixDeviceContextDestroy( c ); c = 0;
        }
        static void contextLogCallback( unsigned int level, const char* tag, const char* message, void* /*cbdata */ ) {
            std::cerr << "[OPTIX][" << std::setw( 2 ) << level << "][" << std::setw( 12 ) << tag << "]: "
            << message << "\n";
        }
    };

    struct AutoModule {
        OptixModule m = 0;
        ~AutoModule() { quit(); }        
        bool init(OptixDeviceContext context, OptixPipelineCompileOptions & pipeline_compile_options, const char * ptx) {
            quit();
            OptixModuleCompileOptions module_compile_options = {};
            module_compile_options.maxRegisterCount     = OPTIX_COMPILE_DEFAULT_MAX_REGISTER_COUNT;
            module_compile_options.optLevel             = OPTIX_COMPILE_OPTIMIZATION_LEVEL_0;
            module_compile_options.debugLevel           = OPTIX_COMPILE_DEBUG_LEVEL_FULL;

            pipeline_compile_options.usesMotionBlur        = false;
            pipeline_compile_options.traversableGraphFlags = OPTIX_TRAVERSABLE_GRAPH_FLAG_ALLOW_SINGLE_LEVEL_INSTANCING;
            pipeline_compile_options.numPayloadValues      = 2;
            pipeline_compile_options.numAttributeValues    = 2;
            pipeline_compile_options.exceptionFlags        = OPTIX_EXCEPTION_FLAG_NONE;  // TODO: should be OPTIX_EXCEPTION_FLAG_STACK_OVERFLOW;
            pipeline_compile_options.pipelineLaunchParamsVariableName = "launchParameters";

            static char log[2048];
            size_t sizeof_log = sizeof( log );
            OPTIX_CHECK(
                optixModuleCreateFromPTX(
                        context,
                        &module_compile_options,
                        &pipeline_compile_options,
                        ptx, strlen(ptx),
                        log, &sizeof_log,
                        &m
                        ),
                GN_ERROR(sLogger)("failed to create optix module from PTX: %s", log); return false; );
            return true;
        }
        void quit() {
            if( m ) { optixModuleDestroy( m ); m = 0; }
        }
    };

    struct AutoProgramGroup {
        OptixProgramGroup g = 0;
        ~AutoProgramGroup() { quit(); }
        bool init(OptixDeviceContext context, OptixModule module, OptixProgramGroupKind kind, const char * entry) {
            quit();
            OptixProgramGroupOptions options = {};
            OptixProgramGroupDesc desc       = {};
            desc.kind                        = kind;
            desc.raygen.module               = entry ? module : 0;
            desc.raygen.entryFunctionName    = entry;
            static char log[2048];
            size_t sizeof_log = sizeof( log );
            OPTIX_CHECK(
                optixProgramGroupCreate(
                        context, &desc,
                        1,  // num program groups
                        &options,
                        log, &sizeof_log,
                        &g),
                GN_ERROR(sLogger)("failed to create program group: %s", log); return false; );
            return true;
        }
        void quit() {
            if (g) optixProgramGroupDestroy(g), g = 0;
        }
    };

    struct AutoPipeline {
        OptixPipeline p = 0;
        ~AutoPipeline() { quit(); }
        void quit() {
        if( p ) { optixPipelineDestroy( p ); p = 0; }
        }
    };

    struct AutoSBT : OptixShaderBindingTable {
        ~AutoSBT() {
            quit();
        }
        void quit() {
            if( raygenRecord ) cudaFree( (void*)raygenRecord ), raygenRecord = 0;
            if( missRecordBase ) cudaFree( (void*)missRecordBase ), missRecordBase = 0;
            missRecordCount = 0;
            missRecordStrideInBytes = 0;
        }
    };

    AutoContext                 context;
    AutoModule                  module;
    AutoProgramGroup            raygen, miss;
    AutoPipeline                pipeline;
    OptixPipelineCompileOptions pco = {};
    AutoSBT                     sbt = {};

    bool createDummySBT() {
        struct DummyRecord
        {
            __align__( OPTIX_SBT_RECORD_ALIGNMENT ) char header[OPTIX_SBT_RECORD_HEADER_SIZE];
            int data;
        };
        DummyRecord rec;

        sbt.quit();

        CUDA_RETURN_FALSE_ON_FAIL( cudaMalloc( (void**)&sbt.raygenRecord, sizeof(DummyRecord) ) );
        OPTIX_RETURN_FALSE_ON_FAIL( optixSbtRecordPackHeader( raygen.g, &rec ) );
        CUDA_RETURN_FALSE_ON_FAIL( cudaMemcpy( (void*)sbt.raygenRecord, &rec, sizeof(rec), cudaMemcpyHostToDevice ) );

        CUDA_RETURN_FALSE_ON_FAIL( cudaMalloc( (void**)&sbt.missRecordBase, sizeof(DummyRecord) ) );
        OPTIX_RETURN_FALSE_ON_FAIL( optixSbtRecordPackHeader( miss.g, &rec ) );
        CUDA_RETURN_FALSE_ON_FAIL( cudaMemcpy( (void*)sbt.missRecordBase, &rec, sizeof(rec), cudaMemcpyHostToDevice ) );
        sbt.missRecordCount = 1;
        sbt.missRecordStrideInBytes = sizeof(DummyRecord);
        return true;

    }

    bool createPipeline() {
        GN_GUARD;

        pipeline.quit();
        
        OptixProgramGroup program_groups[] = { raygen.g, miss.g };

        OptixPipelineLinkOptions plo = {};
        plo.maxTraceDepth            = 2;
        plo.debugLevel               = OPTIX_COMPILE_DEBUG_LEVEL_FULL;
        OPTIX_RETURN_FALSE_ON_FAIL( optixPipelineCreate(
            context.c,
            &pco,
            &plo,
            program_groups, (uint32_t)std::size(program_groups),
            nullptr, nullptr,
            &pipeline.p ) );

        OptixStackSizes stack_sizes = {};
        for( auto& prog_group : program_groups )
        {
            OPTIX_RETURN_FALSE_ON_FAIL( optixUtilAccumulateStackSizes( prog_group, &stack_sizes ) );
        }

        uint32_t direct_callable_stack_size_from_traversal;
        uint32_t direct_callable_stack_size_from_state;
        uint32_t continuation_stack_size;
        OPTIX_RETURN_FALSE_ON_FAIL( optixUtilComputeStackSizes(
            &stack_sizes, plo.maxTraceDepth,
            0,  // maxCCDepth
            0,  // maxDCDEpth
            &direct_callable_stack_size_from_traversal,
            &direct_callable_stack_size_from_state,
            &continuation_stack_size ) );
        OPTIX_RETURN_FALSE_ON_FAIL( optixPipelineSetStackSize(
            pipeline.p,
            direct_callable_stack_size_from_traversal,
            direct_callable_stack_size_from_state,
            continuation_stack_size,
            plo.maxTraceDepth ) );

        // done 
        return true;

        GN_UNGUARD;
    }

    template<class LAUNCH_PARAMETERS>
    void launch( cudaStream_t stream, LAUNCH_PARAMETERS * parameters, uint32_t width, uint32_t height, uint32_t depth = 1 ) {
        GN_DO_EVERY_N_SEC( 3, GN_INFO( sLogger ) << "pipline = " << pipeline.p << ", width = " << width << ", height = " << height );
        OPTIX_CHECK( optixLaunch(
            pipeline.p,
            stream,
            (CUdeviceptr)parameters,
            sizeof(*parameters),
            &sbt,
            width,
            height,
            depth ), );
    }
};

class OptixSample : public GN::util::SampleApp {

    AutoRef<gfx::Texture>           _texture;
    CUstream                        _stream = 0;
    CudaBuffer<gfx::Rgba8>          _outputImage;
    AutoObjPtr<OptixStats>          _optix;
    CudaBuffer<LaunchParameters>    _launchParameters;

public:

    OptixSample() {}

    bool onPreInit( InitParam & ip ) {
        ip.useMultithreadGpu = false;
        ip.ro.api = GN::gfx::GpuAPI::OGL;
        return true;
    }

    bool onInit() {
        GN_GUARD;

        // create output texture
        auto g = engine::getGpu();
        auto & dd = g->getDispDesc();
        _texture.attach( g->create2DTexture( gfx::ColorFormat::RGBA8, dd.width, dd.height, 1, gfx::TextureUsage::FAST_CPU_WRITE ) );
        if( !_texture ) return false;

        // Initialize optix output buffer with pure red color
        std::vector<gfx::Rgba8> image(dd.width * dd.height, gfx::Rgba8(255, 0, 0, 255));
        if( !_outputImage.init( dd.width * dd.height, image.data() ) ) return false;

        // Initialize Optix
        CUDA_RETURN_FALSE_ON_FAIL( cudaStreamCreate( &_stream ) );
        OPTIX_RETURN_FALSE_ON_FAIL( optixInit() );
        _optix.attach(new OptixStats());
        if( !_optix->context.init() ) return false;
        if( !_optix->module.init( _optix->context.c, _optix->pco, trace_ptx ) ) return false;
        if( !_optix->raygen.init( _optix->context.c, _optix->module.m, OPTIX_PROGRAM_GROUP_KIND_RAYGEN, "__raygen__test1" ) ) return false;
        if( !_optix->miss.init( _optix->context.c, _optix->module.m, OPTIX_PROGRAM_GROUP_KIND_MISS, nullptr ) ) return false;
        if( !_optix->createDummySBT() ) return false;
        if( !_optix->createPipeline() ) return false;

        // initialize launch parameters
        if( !_launchParameters.init( 1 ) ) return false;
        _launchParameters.h()->image  = (uchar4*)_outputImage.d();
        _launchParameters.h()->width  = dd.width;
        _launchParameters.h()->height = dd.height;
        _launchParameters.copyToDevice();

        // done
        return true;

        GN_UNGUARD;
    }

    void onQuit() {
        _launchParameters.quit();
        _optix.clear();
        _outputImage.quit();
        if( _stream ) cudaStreamDestroy( _stream ), _stream = 0;
        _texture.clear();
    }

    void onKeyPress( input::KeyEvent key ) {
        SampleApp::onKeyPress( key );
    }

    void onUpdate() {
    }

    void onRender() {
        GN_GUARD_SLOW;

        auto g = engine::getGpu();
        auto w = g->getOptions().displayMode.width;
        auto h = g->getOptions().displayMode.height;

        // run optix tracing
        _optix->launch( _stream, _launchParameters.d(), w, h );
        CUDA_CHECK( cudaDeviceSynchronize(), );

        // upload tracing result to texture
        _outputImage.copyToHost();
        _texture->updateMipmap(0, 0, nullptr, w * 4, 0, _outputImage.h());

        // draw the texture to screen
        g->clearScreen();
        auto sr = engine::getSpriteRenderer();
        sr->drawSingleTexturedSprite( _texture, GN::gfx::SpriteRenderer::SOLID_2D_IMAGE, .0f, .0f, (float)w, (float)h );

        GN_UNGUARD_SLOW;
    }
};

int main(int argc, char * argv[]) {
    GN::enableCRTMemoryCheck();
    OptixSample app;
    return app.run( argc, argv );
}
