#include "pch.h"
#include "ceguiRenderer.h"
#include "ceguiTexture.h"

#ifdef HAS_CEGUI

    // link to CEGUI libraries
    #if GN_MSVC
        #pragma comment(lib, "CEGUIbase.lib")
    #endif

// *****************************************************************************
//  from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::addQuad(const Rect & dest_rect, float z, const Texture * tex, const Rect & texture_rect, const ColourRect & colours,
                                    QuadSplitMode) {
    GN_GUARD_SLOW;

    // get screen size
    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();

    float  x0  = dest_rect.d_left / dd.width;
    float  y0  = dest_rect.d_top / dd.height;
    float  x1  = dest_rect.d_right / dd.width;
    float  y1  = dest_rect.d_bottom / dd.height;
    float  u0  = texture_rect.d_left;
    float  v0  = texture_rect.d_top;
    float  u1  = texture_rect.d_right;
    float  v1  = texture_rect.d_bottom;
    uint32 cx0 = colours.d_top_left.getARGB();
    uint32 cx1 = colours.d_top_right.getARGB();
    uint32 cy0 = colours.d_bottom_right.getARGB();
    uint32 cy1 = colours.d_bottom_left.getARGB();

    QuadDesc qd = {(const GarnetTexture *) tex, z, x0, y0, x1, y1, u0, v0, u1, v1, cx0, cx1, cy0, cy1};
    if (mQueueEnabled)
        mQuads.append(qd);
    else
        drawQuads(&qd, 1);
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::doRender(void) {
    GN_GUARD_SLOW;

    if (!mQuads.empty()) drawQuads(&mQuads[0], mQuads.size());

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture * CEGUI::GarnetRenderer::createTexture(void) {
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p(new GarnetTexture(this));
    if (!p) return 0;
    mTextures.append(p);
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture * CEGUI::GarnetRenderer::createTexture(const String & filename, const String & resourceGroup) {
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p(new GarnetTexture(this));
    if (!p) return 0;
    p->loadFromFile(filename, resourceGroup);
    mTextures.append(p);
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Texture * CEGUI::GarnetRenderer::createTexture(float size) {
    GN_GUARD;

    GN::AutoObjPtr<GarnetTexture> p(new GarnetTexture(this));
    if (!p) return 0;
    p->loadFromMemory(0, (uint) size, (uint) size);
    mTextures.append(p);
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::destroyTexture(Texture * texture) {
    GN_GUARD;

    GN::DynaArray<GarnetTexture *>::iterator i = std::find(mTextures.begin(), mTextures.end(), texture);

    if (i != mTextures.end()) {
        GN_ASSERT(*i);
        delete *i;
        mTextures.erase(i);
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::destroyAllTextures(void) {
    GN_GUARD;

    GN::DynaArray<GarnetTexture *>::iterator i;
    for (i = mTextures.begin(); i != mTextures.end(); ++i) {
        GN_ASSERT(*i);
        delete *i;
    }
    mTextures.clear();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
float CEGUI::GarnetRenderer::getWidth(void) const {
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return (float) dd.width;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
float CEGUI::GarnetRenderer::getHeight(void) const {
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return (float) dd.height;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Size CEGUI::GarnetRenderer::getSize(void) const {
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return Size((float) dd.width, (float) dd.height);

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
CEGUI::Rect CEGUI::GarnetRenderer::getRect(void) const {
    GN_GUARD;

    const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
    return Rect(0, 0, (float) dd.width, (float) dd.height);

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool CEGUI::GarnetRenderer::onRendererRestore() {
    // reload textures
    GN::DynaArray<GarnetTexture *>::iterator i;
    for (i = mTextures.begin(); i != mTextures.end(); ++i) {
        GN_ASSERT(*i);
        if (!(*i)->reload()) return false;
    }

    // update display size
    EventArgs args;
    fireEvent(EventDisplaySizeChanged, args, EventNamespace);

    // Now we've come back, we MUST ensure a full redraw is done since the
    // textures in the stored quads will have been invalidated.
    System::getSingleton().signalRedraw();

    return true;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetRenderer::onRendererDispose() {
    // dispose textures
    GN::DynaArray<GarnetTexture *>::iterator i;
    for (i = mTextures.begin(); i != mTextures.end(); ++i) {
        GN_ASSERT(*i);
        (*i)->dispose();
    }
}

//
//
// -----------------------------------------------------------------------------
inline void CEGUI::GarnetRenderer::drawQuads(const QuadDesc * quads, size_t count) {
    GN_GUARD_SLOW;

    GN::scene::QuadRenderer & qr  = GN::scene::gQuadRenderer;
    const QuadDesc *          q   = quads;
    const QuadDesc *          end = quads + count;

    const GarnetTexture * tex = q->tex;

    qr.drawBegin(tex ? tex->getGarnetTexture() : 0);

    for (; q <= end; ++q) {
        if (q == end || q->tex != tex) {
            // switch texture
            qr.drawEnd();

            if (q < end) {
                qr.drawBegin(q->tex ? q->tex->getGarnetTexture() : 0);
            } else
                break;
        }

        qr.drawTextured(q->z, q->x0, q->y0, q->x1, q->y1, q->u0, q->v0, q->u1, q->v1);
    }

    GN_UNGUARD_SLOW;
}

#endif
