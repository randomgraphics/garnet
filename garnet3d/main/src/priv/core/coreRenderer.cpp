#include "pch.h"
#include "garnet/GNgfx.h"

// *************************************************************************
// local functions
// *************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.core.Renderer");

///
/// Function prototype to create instance of renderer.
///
typedef GN::gfx::Renderer * (*CreateRendererFunc)( const GN::gfx::RendererOptions & );

#if GN_BUILD_STATIC

//
// create D3D9 renderer
//
#if GN_MSWIN
extern GN::gfx::Renderer * createD3D9Renderer( const GN::gfx::RendererOptions & );
#else
inline GN::gfx::Renderer * createD3D9Renderer( const GN::gfx::RendererOptions & )
{ GN_ERROR(sLogger)( "No D3D9 support on platform other than MS Windows." ); return 0; }
#endif

//
// create D3D10 renderer
//
#if GN_MSWIN && !GN_XENON && defined(HAS_D3D10)
extern GN::gfx::Renderer * createD3D10Renderer( const GN::gfx::RendererOptions & );
#else
inline GN::gfx::Renderer * createD3D10Renderer( const GN::gfx::RendererOptions & )
{ GN_ERROR(sLogger)( "No D3D10 support on platform other than MS Vista." ); return 0; }
#endif

//
// create OGL renderer
//
#if GN_XENON
inline GN::gfx::Renderer * createOGLRenderer( const GN::gfx::RendererOptions & )
{ GN_ERROR(sLogger)( "No OGL support on Xenon." ); return 0; }
#else
extern GN::gfx::Renderer * createOGLRenderer( const GN::gfx::RendererOptions & );
#endif

#endif

//
// determine renderer API
// -----------------------------------------------------------------------------
static GN::gfx::RendererAPI sDetermineRendererAPI()
{
#if defined(HAS_D3D9)
    return GN::gfx::API_D3D9;
#elif defined(HAS_OGL)
    return GN::gfx::API_OGL;
#else
    return GN::gfx::API_FAKE;
#endif
}

namespace GN { namespace gfx
{
    // global renderer signals
    typedef Signal4<void, Renderer&, HandleType, UInt32, UInt32> SizeMoveSignal;
    GN_PUBLIC Signal1<void,Renderer&>  & getSigRendererDeviceLost() { static Signal1<void,Renderer&> s; return s; }
    GN_PUBLIC SizeMoveSignal           & getSigRendererWindowSizeMove() { static SizeMoveSignal s; return s; }
    GN_PUBLIC Signal1<void,Renderer&>  & getSigRendererWindowClose() { static Signal1<void,Renderer&> s; return s; }

    //
    //
    // -------------------------------------------------------------------------
    GN_PUBLIC Renderer * createRenderer( const RendererOptions & ro )
    {
        GN_GUARD;

        RendererAPI api = ro.api;

        if( API_AUTO == api ) api = sDetermineRendererAPI();

        // then create new one.
        if( API_FAKE == api )
        {
            GN_UNIMPL();
            return NULL;
        }

#if GN_BUILD_STATIC
        switch( api )
        {
            case API_D3D9  : return createD3D9Renderer( ro );
            case API_D3D10 : return createD3D10Renderer( ro );
            case API_OGL   : return createOGLRenderer( ro );
            default        : GN_ERROR(sLogger)( "Invalid API(%d)", api ); return 0;
        }
#else
        const char * dllName;
        CreateRendererFunc creator;
        switch( api )
        {
            case API_D3D9  : dllName = "GNrndrD3D9"; break;
            case API_D3D10 : dllName = "GNrndrD3D10"; break;
            case API_OGL   : dllName = "GNrndrOGL"; break;
            default        : GN_ERROR(sLogger)( "Invalid API(%d)", api ); return 0;
        }
        std::auto_ptr<SharedLib> dll( new SharedLib );
        if( !dll->load( dllName ) ) return 0;
        creator = (CreateRendererFunc)dll->getSymbol( "GNgfxCreateRenderer" );
        if( !creator ) return 0;
        Renderer * r = creator( ro );
        if( 0 == r ) return 0;
        r->mSharedLib = dll.release();
        return r;
#endif
        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
    GN_PUBLIC void deleteRenderer( Renderer * r )
    {
        GN_GUARD;

        if( r )
        {
            SharedLib * dll = r->mSharedLib;
            delete r;
            delete dll;
        }

        GN_UNGUARD;
    }
}}
