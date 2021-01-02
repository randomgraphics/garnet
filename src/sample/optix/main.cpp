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

    struct DummySBT {
        OptixShaderBindingTable sbt = {};
        struct DummyRecord
        {
            __align__( OPTIX_SBT_RECORD_ALIGNMENT ) char header[OPTIX_SBT_RECORD_HEADER_SIZE];
            int data;
        };
        ~DummySBT() {
            quit();
        }
        bool init() {
            quit();
            CUDA_RETURN_FALSE_ON_FAIL( cudaMallocManaged<DummyRecord>( (DummyRecord**)&sbt.raygenRecord, 1 ) );
            CUDA_RETURN_FALSE_ON_FAIL( cudaMallocManaged<DummyRecord>( (DummyRecord**)&sbt.missRecordBase, 1 ) );
            sbt.missRecordCount = 1;
            sbt.missRecordStrideInBytes = sizeof(DummyRecord);
            return true;
        }
        void quit() {
            if( sbt.raygenRecord ) cudaFree( (void*)sbt.raygenRecord ), sbt.raygenRecord = 0;
        }
    };

    AutoContext                 context;
    AutoModule                  module;
    AutoProgramGroup            raygen, miss;
    AutoPipeline                pipeline;
    OptixPipelineCompileOptions pco = {};
    DummySBT                    sbt = {};

    bool createProgramGroups() {

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
    void launch(cudaStream_t stream, LAUNCH_PARAMETERS * parameters, uint32_t width, uint32_t height, uint32_t depth = 1 ) {
        OPTIX_CHECK( optixLaunch( pipeline.p, stream, (CUdeviceptr)parameters, sizeof(*parameters), &sbt.sbt, width, height, depth ), );
    }
};

class OptixSample : public GN::util::SampleApp {

    AutoRef<gfx::Texture> _texture;

    CUstream                _stream = 0;
    uchar4 *                _outputImage = 0;
    AutoObjPtr<OptixStats>  _optix;
    LaunchParameters *      _launchParameters = 0;

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
        std::vector<gfx::Rgba8> image( dd.width * dd.height );
        for( size_t y = 0; y < dd.height; ++y ) {
            for( size_t x = 0; x < dd.width; ++x ) {
                image[ y * dd.width + x ] = gfx::Rgba8(
                    (uint8_t)( x * 255 / ( dd.width - 1 ) ),
                    (uint8_t)( y * 255 / ( dd.height - 1 ) ),
                    0,
                    255 );
            }
        }
        _texture->updateMipmap( 0, 0, nullptr, dd.width * 4, 0, image.data() );

        // Initialize CUDA
        CUDA_RETURN_FALSE_ON_FAIL( cudaMallocManaged( (void**)&_outputImage, dd.width * dd.height * 4 ) );
        CUDA_RETURN_FALSE_ON_FAIL( cudaStreamCreate( &_stream ) );

        // Initialize Optix
        OPTIX_RETURN_FALSE_ON_FAIL( optixInit() );
        _optix.attach(new OptixStats());
        if( !_optix->context.init() ) return false;
        if( !_optix->module.init( _optix->context.c, _optix->pco, trace_ptx ) ) return false;
        if( !_optix->raygen.init( _optix->context.c, _optix->module.m, OPTIX_PROGRAM_GROUP_KIND_RAYGEN, "__raygen__test1" ) ) return false;
        if( !_optix->miss.init( _optix->context.c, _optix->module.m, OPTIX_PROGRAM_GROUP_KIND_MISS, nullptr ) ) return false;
        if( !_optix->sbt.init() ) return false;
        if( !_optix->createPipeline() ) return false;

        // initialize launch parameters
        CUDA_RETURN_FALSE_ON_FAIL( cudaMallocManaged( (void**)&_launchParameters, sizeof(LaunchParameters) ) );
        _launchParameters->image  = _outputImage;
        _launchParameters->width  = dd.width;
        _launchParameters->height = dd.height;

        // done
        return true;

        GN_UNGUARD;
    }

    void onQuit() {
        _optix.clear();
        if( _stream ) cudaStreamDestroy( _stream ), _stream = 0;
        if( _outputImage ) cudaFree( _outputImage ), _outputImage = 0;
        if( _launchParameters ) cudaFree( _launchParameters ); _launchParameters = 0;
        _texture.clear();
    }

    void onKeyPress( input::KeyEvent key ) {
        SampleApp::onKeyPress( key );
    }

    void onUpdate() {
    }

    void onRender() {
        GN_GUARD_SLOW;

        _optix->launch( _stream, _launchParameters, _launchParameters->width, _launchParameters->height );
        CUDA_CHECK( cudaDeviceSynchronize(), );

        auto g = engine::getGpu();
        g->clearScreen();
        auto w = g->getOptions().displayMode.width;
        auto h = g->getOptions().displayMode.height;
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
