#include "pch.h"

// create fake renderer
extern GN::gfx::Renderer * createFakeRenderer( const GN::gfx::RendererOptions & );

#if GN_STATIC

// create D3D renderer
#if GN_MSWIN
extern GN::gfx::Renderer * createD3DRenderer( const GN::gfx::RendererOptions & );
#else
inline GN::gfx::Renderer * createD3DRenderer( const GN::gfx::RendererOptions & )
{ GN_ERROR( "No D3D support on platform other than MS Windows." ); return 0; }
#endif

// create OGL renderer
#if GN_XENON
inline GN::gfx::Renderer * createOGLRenderer( const GN::gfx::RendererOptions & )
{ GN_ERROR( "No OGL support on Xenon." ); return 0; }
#else
extern GN::gfx::Renderer * createOGLRenderer( const GN::gfx::RendererOptions & );
#endif

#endif

//!
//! Function prototype to create instance of renderer.
//!
typedef GN::gfx::Renderer * (*CreateRendererFunc)( const GN::gfx::RendererOptions & );

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

//
//
// -------------------------------------------------------------------------
GN::gfx::Renderer * GN::gfx::createRenderer(
    RendererAPI api, const RendererOptions & ro )
{
    GN_GUARD;

    // release old renderer
    deleteRenderer();

    // then create new one.
    if( API_FAKE == api ) return createFakeRenderer( ro );
#if GN_STATIC
    switch( api )
    {
        case API_D3D  : return createD3DRenderer( ro );
        case API_OGL  : return createOGLRenderer( ro );
        default       : GN_ERROR( "Invalid API(%d)", api ); return 0;
    }
#else
    const char * dllName;
    CreateRendererFunc creator;
    switch( api )
    {
        case API_D3D  : dllName = "GNgfxD3D"; break;
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
