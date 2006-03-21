#include "pch.h"

// *************************************************************************
// local functions
// *************************************************************************

//!
//! Function prototype to create instance of renderer.
//!
typedef GN::gfx::Renderer * (*CreateRendererFunc)( const GN::gfx::RendererOptions & );

//
// create fake renderer
//
extern GN::gfx::Renderer * createFakeRenderer( const GN::gfx::RendererOptions & );

#if GN_STATIC

//
// create D3D renderer
//
#if GN_MSWIN
extern GN::gfx::Renderer * createD3D9Renderer( const GN::gfx::RendererOptions & );
#else
inline GN::gfx::Renderer * createD3D9Renderer( const GN::gfx::RendererOptions & )
{ GN_ERROR( "No D3D support on platform other than MS Windows." ); return 0; }
#endif

//
// create OGL renderer
//
#if GN_XENON
inline GN::gfx::Renderer * createOGLRenderer( const GN::gfx::RendererOptions & )
{ GN_ERROR( "No OGL support on Xenon." ); return 0; }
#else
extern GN::gfx::Renderer * createOGLRenderer( const GN::gfx::RendererOptions & );
#endif

#endif

//
// determine renderer API
// -----------------------------------------------------------------------------
static GN::gfx::RendererAPI sDetermineRendererAPI()
{
#if GN_XENON
    return GN::gfx::API_D3D9;
#elif GN_MSWIN
    return GN::gfx::API_D3D9;
#else
    return GN::gfx::API_OGL;
#endif
}

// *************************************************************************
// public functions
// *************************************************************************

//
//
// -------------------------------------------------------------------------
GN::gfx::Renderer * GN::gfx::createRenderer(
    const RendererOptions & ro, RendererAPI api )
{
    GN_GUARD;

    // release old renderer
    deleteRenderer();

    if( API_AUTO == api ) api = sDetermineRendererAPI();

    // then create new one.
    if( API_FAKE == api ) return createFakeRenderer( ro );
#if GN_STATIC
    switch( api )
    {
        case API_D3D9 : return createD3D9Renderer( ro );
        case API_OGL  : return createOGLRenderer( ro );
        default       : GN_ERROR( "Invalid API(%d)", api ); return 0;
    }
#else
    const char * dllName;
    CreateRendererFunc creator;
    switch( api )
    {
        case API_D3D9 : dllName = "GNgfxD3D9"; break;
        case API_OGL  : dllName = "GNgfxOGL"; break;
        default       : GN_ERROR( "Invalid API(%d)", api ); return 0;
    }
    if( !Renderer::msSharedLib.load( dllName ) ) return 0;
    creator = (CreateRendererFunc)Renderer::msSharedLib.getSymbol( "GNgfxCreateRenderer" );
    if( !creator ) return 0;
    return creator( ro );
#endif
    GN_UNGUARD;
}

//
//
// -------------------------------------------------------------------------
void GN::gfx::deleteRenderer()
{
    GN_GUARD;

    delete gRendererPtr;
    Renderer::msSharedLib.free();

    GN_UNGUARD;
}
