#include "pch.h"
#include "ceguiRenderer.h"
#include "ceguiTexture.h"

#ifdef HAS_CEGUI

// link to CEGUI libraries
#if GN_MSVC
#if GN_DEBUG_BUILD && defined(CEGUI_LOAD_MODULE_APPEND_SUFFIX_FOR_DEBUG)
#define LIBNAME(X) X CEGUI_LOAD_MODULE_DEBUG_SUFFIX ".lib"
#else
#define LIBNAME(X) X ".lib"
#endif
#pragma comment( lib, LIBNAME( "CEGUIbase" ) )
#endif

// *****************************************************************************
//  from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::addQuad(
    const Rect& dest_rect,
    float z,
    const Texture * tex,
    const Rect& texture_rect,
    const ColourRect & colours,
    QuadSplitMode )
{
    GN_GUARD_SLOW;

    float x0 = dest_rect.d_left;
    float y0 = dest_rect.d_top;
    float x1 = dest_rect.d_right;
    float y1 = dest_rect.d_bottom;
    float u0 = texture_rect.d_left;
    float v0 = texture_rect.d_top;
    float u1 = texture_rect.d_right;
    float v1 = texture_rect.d_bottom;
    UInt32 c0 = colours.d_top_left.getARGB();
    UInt32 c1 = colours.d_top_right.getARGB();
    UInt32 c2 = colours.d_bottom_right.getARGB();
    UInt32 c3 = colours.d_bottom_left.getARGB();

    QuadDesc qd =
    {
        {
            { x0, y0, z, u0, v0, c0, tex },
            { x1, y0, z, u1, v0, c1, tex },
            { x1, y1, z, u1, v1, c2, tex },
            { x0, y1, z, u0, v1, c3, tex },
        }
    };
    if( mQueueEnabled ) mQuads.push_back( qd );
    else drawQuads( &qd, 1 );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::doRender(void)
{
    GN_GUARD_SLOW;

    if( !mQuads.empty() ) drawQuads( &mQuads[0], mQuads.size() );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture* CEGUI::GarnetRenderer::createTexture(void)
{
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p( new GarnetTexture(this) );
    if( !p ) return 0;
    mTextures.push_back( p );
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture* CEGUI::GarnetRenderer::createTexture(
    const String& filename, const String& resourceGroup)
{
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p( new GarnetTexture(this) );
    if( !p ) return 0;
    p->loadFromFile( filename, resourceGroup );
    mTextures.push_back( p );
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture* CEGUI::GarnetRenderer::createTexture(float size)
{
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p( new GarnetTexture(this) );
    if( !p ) return 0;
    p->loadFromMemory( 0, (uint)size, (uint)size );
    mTextures.push_back( p );
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::destroyTexture(Texture* texture)
{
    GN_GUARD;

    std::vector<GarnetTexture*>::iterator i = std::find(
        mTextures.begin(), mTextures.end(), texture );

    if( i != mTextures.end() )
    {
        GN_ASSERT( *i );
        delete *i;
        mTextures.erase( i );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::destroyAllTextures(void)
{
    GN_GUARD;

    std::vector<GarnetTexture*>::iterator i;
    for( i = mTextures.begin(); i != mTextures.end(); ++i )
    {
        GN_ASSERT( *i );
        delete *i;
    }
    mTextures.clear();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
float CEGUI::GarnetRenderer::getWidth(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return (float)dd.width;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
float CEGUI::GarnetRenderer::getHeight(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return (float)dd.height;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Size CEGUI::GarnetRenderer::getSize(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return Size( (float)dd.width, (float)dd.height );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Rect CEGUI::GarnetRenderer::getRect(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return Rect( 0, 0, (float)dd.width, (float)dd.height );

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool CEGUI::GarnetRenderer::onRendererRestore()
{
    // reload textures
    std::vector<GarnetTexture*>::iterator i;
    for( i = mTextures.begin(); i != mTextures.end(); ++i )
    {
        GN_ASSERT( *i );
        if( !(*i)->reload() ) return false;
    }

	// update display size
	EventArgs args;
	fireEvent( EventDisplaySizeChanged, args, EventNamespace );

	// Now we've come back, we MUST ensure a full redraw is done since the
	// textures in the stored quads will have been invalidated.
	System::getSingleton().signalRedraw();

    return true;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::onRendererDispose()
{
    // dispose textures
    std::vector<GarnetTexture*>::iterator i;
    for( i = mTextures.begin(); i != mTextures.end(); ++i )
    {
        GN_ASSERT( *i );
        (*i)->dispose();
    }
}

//
//
// -----------------------------------------------------------------------------
inline void CEGUI::GarnetRenderer::drawQuads( const QuadDesc *, size_t )
{
/*
    GN_GUARD_SLOW;

    GN::scene::QuadRenderer & qr = GN::scene::gQuadRenderer;

    const QuadDesc * start = quads;

    qr.drawBegin(
        start->vertices[0].tex ? ((const GarnetTexture *)start->vertices[0].tex)->getGarnetTexture() : 0 );

    while( count > 0 )
    {
        if( quads->vertices[0].tex == start->vertices[0].tex )
        {
            ++quads;
            --count;
            continue;
        }

        GN_ASSERT( quads > start );

        // set texture
        qr.drawSolid
        r.setTexture(
            0,
            start->vertices[0].tex ? ((const GarnetTexture *)start->vertices[0].tex)->getGarnetTexture() : 0 );

        // draw quads from start to quads (they have same texture)
        r.drawQuads(
            GN::gfx::DQ_WINDOW_SPACE,
            &start->vertices[0].x, sizeof(QuadVertex),
            &start->vertices[0].u, sizeof(QuadVertex),
            &start->vertices[0].bgra, sizeof(QuadVertex),
            quads - start );

        start = quads;
    }

    // draw remaining quads
    if( quads > start )
    {
        r.setTexture(
            0,
            start->vertices[0].tex ? ((const GarnetTexture *)start->vertices[0].tex)->getGarnetTexture() : 0 );

        r.drawQuads(
            GN::gfx::DQ_WINDOW_SPACE,
            &start->vertices[0].x, sizeof(QuadVertex),
            &start->vertices[0].u, sizeof(QuadVertex),
            &start->vertices[0].bgra, sizeof(QuadVertex),
            quads - start );
    }

    GN_UNGUARD_SLOW;
*/
}

#endif
