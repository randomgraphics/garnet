#include "pch.h"
#include "ceguiRenderer.h"
#include "ceguiTexture.h"

#ifdef HAS_CEGUI

// *****************************************************************************
//  from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::addQuad(
    const Rect& dest_rect,
    float z,
    const Texture* tex,
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
    uint32_t c0 = colours.d_top_left.getARGB();
    uint32_t c1 = colours.d_top_right.getARGB();
    uint32_t c2 = colours.d_bottom_right.getARGB();
    uint32_t c3 = colours.d_bottom_left.getARGB();

    uint32_t texid = tex ? GN::safeCast<const GarnetTexture*>(tex)->getHandle() : 0;

    QuadDesc qd =
    {
        {
            { x0, y0, z, u0, v0, c0, texid },
            { x1, y0, z, u1, v0, c1, texid },
            { x1, y1, z, u1, v1, c2, texid },
            { x0, y1, z, u0, v1, c3, texid },
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
    if( 0 == p ) return 0;
    mTextures.push_back( p.get() );
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
    if( 0 == p ) return 0;
    p->loadFromFile( filename, resourceGroup );
    mTextures.push_back( p.get() );
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
    if( 0 == p ) return 0;
    p->loadFromMemory( 0, (uint)size, (uint)size );
    mTextures.push_back( p.get() );
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
inline void CEGUI::GarnetRenderer::drawQuads( const QuadDesc * quads, size_t count )
{
    GN_GUARD_SLOW;

    GN::gfx::Renderer & r = gRenderer;

    /*
    for( ; count > 0; --count, ++quads )
    {
        r.bindTextureHandle( 0, quads->vertices[0].tex );
        r.drawQuads(
            GN::gfx::DQ_WINDOW_SPACE,
            &quads->vertices[0].x, sizeof(QuadVertex),
            &quads->vertices[0].u, sizeof(QuadVertex),
            1 );
    }
    /*/
    const QuadDesc * start = quads;

    while( count > 0 )
    {
        if( quads->vertices[0].tex == start->vertices[0].tex )
        {
            ++quads;
            --count;
            continue;
        }

        GN_ASSERT( quads > start );

        // draw quads from start to quads (they have same texture)
        r.bindTextureHandle( 0, start->vertices[0].tex );
        r.drawQuads(
            GN::gfx::DQ_WINDOW_SPACE,
            &start->vertices[0].x, sizeof(QuadVertex),
            &start->vertices[0].u, sizeof(QuadVertex),
            quads - start );

        start = quads;
    }

    // draw remaining quads
    if( quads > start )
    {
        r.bindTextureHandle( 0, start->vertices[0].tex );
        r.drawQuads(
            GN::gfx::DQ_WINDOW_SPACE,
            &start->vertices[0].x, sizeof(QuadVertex),
            &start->vertices[0].u, sizeof(QuadVertex),
            quads - start );
    }//*/

    GN_UNGUARD_SLOW;
}

#endif
