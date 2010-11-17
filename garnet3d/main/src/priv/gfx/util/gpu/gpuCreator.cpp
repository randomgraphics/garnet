#include "pch.h"
#include "mtgpu.h"

using namespace GN;
using namespace GN::gfx;

// *************************************************************************
// local functions
// *************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.gpu");

#if GN_XENON
#define D3D_GPU_API     GpuAPI::XENON
#define D3D_GPU_NAME    "Xenon"
#define DEFAULT_GPU_API GpuAPI::XENON
#else
#define D3D_GPU_API     GpuAPI::D3D11
#define D3D_GPU_NAME    "D3D11"
#define DEFAULT_GPU_API GpuAPI::OGL;
#endif

#if !GN_BUILD_STATIC

extern const Guid GN::gfx::GPU_DLL_GUID;

//
// create single thread GPU from DLL
// -----------------------------------------------------------------------------
static Gpu * sCreateSingleThreadGpuFromDLL( const GpuOptions & go, void * context )
{
    typedef GN::gfx::Gpu * (*GpuDLLEntry)( const GN::gfx::GpuOptions & );

    const char * dllname = (const char*)context;

    AutoObjPtr<SharedLib> dll( new SharedLib );
    if( !dll->load( dllname ) ) return NULL;

    GpuDLLEntry creator = (GpuDLLEntry)dll->getSymbol( "GNgfxCreateGpu" );
    if( !creator ) return NULL;

    Gpu * r = creator( go );
    if( 0 == r ) return NULL;

    SharedLib * dllptr = dll;
    r->setUserData( GN::gfx::GPU_DLL_GUID, &dllptr, sizeof(dllptr) );
    dll.detach();

    return r;
}

//
// create GPU from DLL
// -----------------------------------------------------------------------------
static Gpu * sCreateGpuFromDLL( const GpuOptions & go, uint32 creationFlags, const char * dllname )
{
    if( 0 != (GPU_CREATION_MULTIPLE_THREADS & creationFlags) )
    {
        return createMultiThreadGpu( go, sCreateSingleThreadGpuFromDLL, dllname );
    }
    else
    {
        return sCreateSingleThreadGpuFromDLL( go, dllname );
    }
}

#endif

// ***********************************************************************
// Public functions
// ***********************************************************************

//
//
// -------------------------------------------------------------------------
#if !GN_BUILD_STATIC
Gpu * GN::gfx::createOGLGpu( const GpuOptions & go, uint32 creationFlags )
{
    GpuOptions o = go;
    o.api = GpuAPI::OGL;
    return sCreateGpuFromDLL( o, creationFlags, "GNgpuOGL" );
}
#elif !HAS_OGL
Gpu * GN::gfx::createOGLGpu( const GpuOptions &, uint32 )
{
    GN_ERROR(sLogger)( "OpenGL renderer is not available." );
    return 0;
}
#endif

//
//
// -------------------------------------------------------------------------
#if !GN_BUILD_STATIC
Gpu * GN::gfx::createD3DGpu( const GpuOptions & go, uint32 creationFlags )
{
    GpuOptions o = go;
    o.api = D3D_GPU_API;
    return sCreateGpuFromDLL( o, creationFlags, "GNgpu" D3D_GPU_NAME );
}
#elif !HAS_D3D11 && !GN_XENON
Gpu * GN::gfx::createD3DGpu( const GpuOptions &, uint32 )
{
    GN_ERROR(sLogger)( D3D_GPU_NAME " renderer is not available." );
    return 0;
}
#endif

//
//
// -------------------------------------------------------------------------
Gpu * GN::gfx::createGpu( const GpuOptions & inputOptions, uint32 creationFlags )
{
    GpuOptions ro = inputOptions;

    if( GpuAPI::AUTO == ro.api ) ro.api = DEFAULT_GPU_API;

    switch( ro.api )
    {
        case GpuAPI::OGL   : return createOGLGpu( ro, creationFlags );
        case GpuAPI::D3D11 : return createD3DGpu( ro, creationFlags );
        case GpuAPI::XENON : return createD3DGpu( ro, creationFlags );
        case GpuAPI::FAKE  : GN_UNIMPL(); return 0;
        default : GN_ERROR(sLogger)( "Invalid API(%d)", ro.api.toRawEnum() ); return 0;
    }
}
