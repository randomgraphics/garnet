#include "pch.h"

#ifdef HAS_D3D9
#include "garnet/GNgfx2D3D9.h"
#else if GN_STATIC
static GN::gfx2::GraphicsSystem * createD3D9GraphicsSystem( const GN::gfx2::GraphicsSystemCreationParameter & )
{
    GN_UNEXPECTED();
    return 0;
}
#endif

//
//
// -----------------------------------------------------------------------------
GN::gfx2::GraphicsSystem *
GN::gfx2::GraphicsSystemCreator::create( const GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    destroy();

    GN_ASSERT( 0 == mInstance );

#if GN_STATIC

    switch( gscp.api.u32 )
    {
        case GN_MAKE_FOURCC('D','X','9',0)   : mInstance = createD3D9GraphicsSystem( gscp ); break;
        case GN_MAKE_FOURCC('O','G','L',0)   :
        case GN_MAKE_FOURCC('D','X','1','0') :
        case GN_MAKE_FOURCC('X','B','2',0)   : GN_UNIMPL(); break;
        default                              : GN_UNEXPECTED(); break;
    }

#else

    typedef GraphicsSystem * (*GraphicsSystemCreateFunc)( const GraphicsSystemCreationParameter & );

    const char * dllname;
    const char * symbol;
    GraphicsSystemCreateFunc func;

    switch( gscp.api.u32 )
    {
        case GN_MAKE_FOURCC('D','X','9',0)   : dllname = "GNgfx2D3D9"; symbol = "createD3D9GraphicsSystem"; break;
        case GN_MAKE_FOURCC('O','G','L',0)   :
        case GN_MAKE_FOURCC('D','X','1','0') :
        case GN_MAKE_FOURCC('X','B','2',0)   : GN_UNIMPL(); return 0;
        default                              : GN_UNEXPECTED(); return 0;
    }

    if( !mDll.load( dllname ) ) return 0;
    func = (GraphicsSystemCreateFunc)mDll.getSymbol( symbol );
    if( !func ) return 0;

    mInstance = func( gscp );

#endif

    return mInstance;

    GN_UNGUARD;
}

//
//
// -------------------------------------------------------------------------
void GN::gfx2::GraphicsSystemCreator::destroy()
{
    GN_GUARD;

    safeDelete( mInstance );
    mDll.free();

    GN_UNGUARD;
}
