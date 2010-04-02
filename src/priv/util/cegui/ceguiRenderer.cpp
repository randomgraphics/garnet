#include "pch.h"
#include "ceguiRenderer.h"
#include "ceguiTexture.h"

#ifdef HAS_CEGUI

// link to CEGUI libraries
#if GN_MSVC
#pragma comment( lib, "CEGUIbase.lib" )
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

    // get screen size
    const GN::gfx::DispDesc & dd = gRenderer.GetDispDesc();

    float x0 = dest_rect.d_left / dd.width;
    float y0 = dest_rect.d_top / dd.height;
    float x1 = dest_rect.d_right / dd.width;
    float y1 = dest_rect.d_bottom / dd.height;
    float u0 = texture_rect.d_left;
    float v0 = texture_rect.d_top;
    float u1 = texture_rect.d_right;
    float v1 = texture_rect.d_bottom;
    UInt32 cx0 = colours.d_top_left.getARGB();
    UInt32 cx1 = colours.d_top_right.getARGB();
    UInt32 cy0 = colours.d_bottom_right.getARGB();
    UInt32 cy1 = colours.d_bottom_left.getARGB();

    QuadDesc qd =
    {
        (const GarnetTexture*)tex,
        z,
        x0 , y0 , x1 , y1 ,
        u0 , v0 , u1 , v1 ,
        cx0, cx1, cy0, cy1
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

    if( !mQuads.empty() ) drawQuads( &mQuads[0], mQuads.Size() );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture* CEGUI::GarnetRenderer::CreateTexture(void)
{
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p( new GarnetTexture(this) );
    if( !p ) return 0;
    mTextures.push_back( p );
    return p.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture* CEGUI::GarnetRenderer::CreateTexture(
    const String& filename, const String& resourceGroup)
{
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p( new GarnetTexture(this) );
    if( !p ) return 0;
    p->LoadFromFile( filename, resourceGroup );
    mTextures.push_back( p );
    return p.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture* CEGUI::GarnetRenderer::CreateTexture(float size)
{
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p( new GarnetTexture(this) );
    if( !p ) return 0;
    p->loadFromMemory( 0, (uint)size, (uint)size );
    mTextures.push_back( p );
    return p.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::destroyTexture(Texture* texture)
{
    GN_GUARD;

    DynaArray<GarnetTexture*>::iterator i = std::find(
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

    DynaArray<GarnetTexture*>::iterator i;
    for( i = mTextures.begin(); i != mTextures.end(); ++i )
    {
        GN_ASSERT( *i );
        delete *i;
    }
    mTextures.Clear();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
float CEGUI::GarnetRenderer::getWidth(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.GetDispDesc();
    return (float)dd.width;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
float CEGUI::GarnetRenderer::getHeight(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.GetDispDesc();
    return (float)dd.height;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Size CEGUI::GarnetRenderer::getSize(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.GetDispDesc();
    return Size( (float)dd.width, (float)dd.height );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Rect CEGUI::GarnetRenderer::getRect(void) const
{
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.GetDispDesc();
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
    DynaArray<GarnetTexture*>::iterator i;
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
    DynaArray<GarnetTexture*>::iterator i;
    for( i = mTextures.begin(); i != mTextures.end(); ++i )
    {
        GN_ASSERT( *i );
        (*i)->dispose();
    }
}

//
//
// -----------------------------------------------------------------------------
inline void CEGUI::GarnetRenderer::drawQuads( const QuadDesc * quads, size_t count )
{
    GN_GUARD_SLOW;

    GN::scene::QuadRenderer & qr = GN::scene::gQuadRenderer;
    const QuadDesc * q   = quads;
    const QuadDesc * end = quads + count;

    const GarnetTexture * tex = q->tex;

    qr.DrawBegin( tex ? tex->getGarnetTexture() : 0 );

    for( ; q <= end; ++q )
    {
        if( q == end || q->tex != tex )
        {
            // switch texture
            qr.DrawEnd();

            if( q < end )
            {
                qr.DrawBegin( q->tex ? q->tex->getGarnetTexture() : 0 );
            }
            else break;
        }

        qr.DrawTextured(
            q->z,
            q->x0, q->y0, q->x1, q->y1,
            q->u0, q->v0, q->u1, q->v1 );
    }

    GN_UNGUARD_SLOW;
}

#endif
