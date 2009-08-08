#include "pch.h"
#include "mtgpu.h"

// *************************************************************************
// local functions
// *************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.gpu");

///
/// Function type to create renderer.
///
typedef GN::gfx::Gpu * (*CreateGpuFunc)( const GN::gfx::GpuOptions & );

/// Xenon renderer creator
#if GN_BUILD_GPU_API_HAS_XENON
extern GN::gfx::Gpu *
GNgfxCreateXenonGpu( const GN::gfx::GpuOptions & );
#else
static inline GN::gfx::Gpu *
GNgfxCreateXenonGpu( const GN::gfx::GpuOptions & )
{
    GN_ERROR(sLogger)( "Xenon renderer is not available." );
    return 0;
}
#endif

///
/// OGL renderer creator
///
#if GN_BUILD_GPU_API_HAS_OGL
extern GN::gfx::Gpu *
GNgfxCreateOGLGpu( const GN::gfx::GpuOptions & );
#else
static inline GN::gfx::Gpu *
GNgfxCreateOGLGpu( const GN::gfx::GpuOptions & )
{
    GN_ERROR(sLogger)( "OpenGL renderer is not available." );
    return 0;
}
#endif

///
/// D3D10 renderer creator
///
#if GN_BUILD_GPU_API_HAS_D3D10
extern GN::gfx::Gpu *
GNgfxCreateD3D10Gpu( const GN::gfx::GpuOptions & );
#else
static inline GN::gfx::Gpu *
GNgfxCreateD3D10Gpu( const GN::gfx::GpuOptions & )
{
    GN_ERROR(sLogger)( "D3D10 renderer is not available." );
    return 0;
}
#endif

///
/// D3D11 renderer creator
///
#if GN_BUILD_GPU_API_HAS_D3D11
extern GN::gfx::Gpu *
GNgfxCreateD3D11Gpu( const GN::gfx::GpuOptions & );
#else
static inline GN::gfx::Gpu *
GNgfxCreateD3D11Gpu( const GN::gfx::GpuOptions & )
{
    GN_ERROR(sLogger)( "D3D11 renderer is not available." );
    return 0;
}
#endif

//
// determine renderer API
// -----------------------------------------------------------------------------
static GN::gfx::GpuAPI sDetermineDefaultGpuAPI()
{
#if GN_BUILD_GPU_API_HAS_XENON
    return GN::gfx::GpuAPI::XENON;
#elif GN_BUILD_GPU_API_HAS_OGL
    return GN::gfx::GpuAPI::OGL;
#elif GN_BUILD_GPU_API_HAS_D3D10
    return GN::gfx::GpuAPI::D3D10;
#elif GN_BUILD_GPU_API_HAS_D3D11
    return GN::gfx::GpuAPI::D3D11;
#else
    return GN::gfx::GpuAPI::FAKE;
#endif
}

static const GN::Guid GPU_DLL_GUID = { 0xa397a51a, 0xa008, 0x4ffd, { 0xbd, 0x8a, 0x7f, 0xd4, 0xa3, 0xaf, 0x59, 0xd7 } };

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    Gpu * createSingleThreadGpu( const GpuOptions & inputOptions )
    {
        GN_GUARD;

        GpuOptions ro = inputOptions;

        if( GpuAPI::AUTO == ro.api ) ro.api = sDetermineDefaultGpuAPI();

        // then create new one.
        if( GpuAPI::FAKE == ro.api )
        {
            GN_UNIMPL();
            return NULL;
        }

#if GN_BUILD_STATIC
        switch( ro.api )
        {
            case GpuAPI::OGL   : return GNgfxCreateOGLGpu( ro );
            case GpuAPI::D3D10 : return GNgfxCreateD3D10Gpu( ro );
            case GpuAPI::D3D11 : return GNgfxCreateD3D11Gpu( ro );
            case GpuAPI::XENON : return GNgfxCreateXenonGpu( ro );
            case GpuAPI::FAKE  : GN_UNIMPL(); return 0;
            default : GN_ERROR(sLogger)( "Invalid API(%d)", ro.api.toRawEnum() ); return 0;
        }
#else
        const char * dllName;
        CreateGpuFunc creator;
        switch( ro.api )
        {
            case GpuAPI::OGL   : dllName = "GNgpuOGL"; break;
            case GpuAPI::D3D10 : dllName = "GNgpuD3D10"; break;
            case GpuAPI::D3D11 : dllName = "GNgpuD3D11"; break;
            case GpuAPI::XENON : dllName = "GNgpuXenon"; break;
            case GpuAPI::FAKE  : GN_UNIMPL(); return 0;
            default : GN_ERROR(sLogger)( "Invalid API(%d)", ro.api.toRawEnum() ); return 0;
        }
        AutoObjPtr<SharedLib> dll( new SharedLib );
        if( !dll->load( dllName ) ) return 0;
        creator = (CreateGpuFunc)dll->getSymbol( "GNgfxCreateGpu" );
        if( !creator ) return 0;
        Gpu * r = creator( ro );
        if( 0 == r ) return 0;
        SharedLib * dllptr = dll;
        r->setUserData( GPU_DLL_GUID, &dllptr, sizeof(dllptr) );
        dll.detach();
        return r;
#endif
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    Gpu * createMultiThreadGpu( const GpuOptions & ro )
    {
        GN_GUARD;

        MultiThreadGpu * r = new MultiThreadGpu;
        MultiThreadGpuOptions mo; // use default multithread options
        if( !r->init( ro, mo ) ) delete r, r = NULL;
        return r;

        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    void deleteGpu( Gpu * r )
    {
        GN_GUARD;

        if( r )
        {
            bool hasdll = r->hasUserData( GPU_DLL_GUID );
            SharedLib * dll = NULL;
            if( hasdll ) r->getUserData( GPU_DLL_GUID, &dll, sizeof(dll) );
            delete r;
            if( hasdll ) delete dll;
        }

        GN_UNGUARD;
    }
}}
