#include "pch.h"
#include "mtrndr.h"

// *************************************************************************
// local functions
// *************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.rndr");

///
/// Function type to create renderer.
///
typedef GN::gfx::Renderer * (*CreateRendererFunc)( const GN::gfx::RendererOptions & );


/// Xenon renderer creator
#if GN_XENON
extern
#else
inline
#endif
GN::gfx::Renderer *
GNgfxCreateXenonRenderer( const GN::gfx::RendererOptions & )
#if GN_XENON
;
#else
{
    GN_ERROR(sLogger)( "Xenon renderer is not available on platform other than Xbox360." );
    return 0;
}
#endif

///
/// OGL renderer creator
///
#if GN_XENON
inline
#else
extern
#endif
GN::gfx::Renderer *
GNgfxCreateOGLRenderer( const GN::gfx::RendererOptions & )
#if GN_XENON
{
    GN_ERROR(sLogger)( "OpenGL renderer is not available on Xbox360." );
    return 0;
}
#else
;
#endif

///
/// D3D10 renderer creator
///
#if GN_XENON | GN_POSIX
inline
#else
extern
#endif
GN::gfx::Renderer *
GNgfxCreateD3D10Renderer( const GN::gfx::RendererOptions & )
#if GN_XENON | GN_POSIX
{
    GN_ERROR(sLogger)( "D3D10 renderer is not available on Xbox360." );
    return 0;
}
#else
;
#endif

///
/// D3D11 renderer creator
///
#ifndef HAS_D3D11
inline
#else
extern
#endif
GN::gfx::Renderer *
GNgfxCreateD3D11Renderer( const GN::gfx::RendererOptions & )
#ifndef HAS_D3D11
{
    GN_ERROR(sLogger)( "D3D11 renderer is not available on Xbox360." );
    return 0;
}
#else
;
#endif

//
// determine renderer API
// -----------------------------------------------------------------------------
static GN::gfx::RendererAPI sDetermineDefaultRendererAPI()
{
#if GN_XENON
    return GN::gfx::RendererAPI::XENON;
#else
    return GN::gfx::RendererAPI::OGL;
#endif
}

static const GN::Guid RENDERER_DLL_GUID = { 0xa397a51a, 0xa008, 0x4ffd, { 0xbd, 0x8a, 0x7f, 0xd4, 0xa3, 0xaf, 0x59, 0xd7 } };

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    Renderer * createSingleThreadRenderer( const RendererOptions & inputOptions )
    {
        GN_GUARD;

        RendererOptions ro = inputOptions;

        if( RendererAPI::AUTO == ro.api ) ro.api = sDetermineDefaultRendererAPI();

        // then create new one.
        if( RendererAPI::FAKE == ro.api )
        {
            GN_UNIMPL();
            return NULL;
        }

#if GN_BUILD_STATIC
        switch( ro.api )
        {
            case RendererAPI::OGL   : return GNgfxCreateOGLRenderer( ro );
            case RendererAPI::D3D9  : GN_UNIMPL(); return 0;
            case RendererAPI::D3D10 : return GNgfxCreateD3D10Renderer( ro );
            case RendererAPI::D3D11 : return GNgfxCreateD3D11Renderer( ro );
            case RendererAPI::XENON : return GNgfxCreateXenonRenderer( ro );
            case RendererAPI::FAKE  : GN_UNIMPL(); return 0;
            default        : GN_ERROR(sLogger)( "Invalid API(%d)", ro.api.toRawEnum() ); return 0;
        }
#else
        const char * dllName;
        CreateRendererFunc creator;
        switch( ro.api )
        {
            case RendererAPI::D3D9  : dllName = "GNrndrD3D9"; break;
            case RendererAPI::D3D10 : dllName = "GNrndrD3D10"; break;
            case RendererAPI::D3D11 : dllName = "GNrndrD3D11"; break;
            case RendererAPI::OGL   : dllName = "GNrndrOGL"; break;
            case RendererAPI::XENON : dllName = "GNrndrXenon"; break;
            default        : GN_ERROR(sLogger)( "Invalid API(%d)", ro.api.toRawEnum() ); return 0;
        }
        AutoObjPtr<SharedLib> dll( new SharedLib );
        if( !dll->load( dllName ) ) return 0;
        creator = (CreateRendererFunc)dll->getSymbol( "GNgfxCreateRenderer" );
        if( !creator ) return 0;
        Renderer * r = creator( ro );
        if( 0 == r ) return 0;
        SharedLib * dllptr = dll;
        r->setUserData( RENDERER_DLL_GUID, &dllptr, sizeof(dllptr) );
        dll.detach();
        return r;
#endif
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    Renderer * createMultiThreadRenderer( const RendererOptions & ro )
    {
        GN_GUARD;

        MultiThreadRenderer * r = new MultiThreadRenderer;
        MultiThreadRendererOptions mo; // use default multithread options
        if( !r->init( ro, mo ) ) delete r, r = NULL;
        return r;

        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    void deleteRenderer( Renderer * r )
    {
        GN_GUARD;

        if( r )
        {
            bool hasdll = r->hasUserData( RENDERER_DLL_GUID );
            SharedLib * dll = NULL;
            if( hasdll ) r->getUserData( RENDERER_DLL_GUID, &dll, sizeof(dll) );
            delete r;
            if( hasdll ) delete dll;
        }

        GN_UNGUARD;
    }
}}
