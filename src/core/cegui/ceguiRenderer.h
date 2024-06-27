#ifndef __GN_GUI_CEGUIRENDERER_H__
#define __GN_GUI_CEGUIRENDERER_H__
// *****************************************************************************
/// \file
/// \brief   cegui renderer
/// \author  chenlee (2006.2.25)
// *****************************************************************************

#ifdef HAS_CEGUI

    #include "ceguiHeaders.h"

namespace CEGUI {
class GarnetTexture;

///
/// CEGUI renderer adapter for garnet platform
///
class GarnetRenderer : public Renderer {
    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    GarnetRenderer(): mQueueEnabled(true) {
        gSigRendererRestore.connect(this, &GarnetRenderer::onRendererRestore);
        gSigRendererDispose.connect(this, &GarnetRenderer::onRendererDispose);
    }
    virtual ~GarnetRenderer() {
        gSigRendererRestore.disconnect(this);
        gSigRendererDispose.disconnect(this);
        destroyAllTextures();
    }
    //@}

    // *****************************
    // from Renderer
    // *****************************
public:
    virtual const String & getIdentifierString() const {
        static const String id("GarnetRenderer");
        return id;
    }
    virtual void      addQuad(const Rect & dest_rect, float z, const Texture * tex, const Rect & texture_rect, const ColourRect & colours,
                              QuadSplitMode quad_split_mode);
    virtual void      doRender(void);
    virtual void      clearRenderList(void) { mQuads.clear(); }
    virtual void      setQueueingEnabled(bool setting) { mQueueEnabled = setting; }
    virtual Texture * createTexture(void);
    virtual Texture * createTexture(const String & filename, const String & resourceGroup);
    virtual Texture * createTexture(float size);
    virtual void      destroyTexture(Texture * texture);
    virtual void      destroyAllTextures(void);
    virtual bool      isQueueingEnabled(void) const { return mQueueEnabled; }
    virtual float     getWidth(void) const;
    virtual float     getHeight(void) const;
    virtual Size      getSize(void) const;
    virtual Rect      getRect(void) const;
    virtual uint      getMaxTextureSize(void) const { return gRenderer.getCaps(GN::gfx::CAPS_MAX_2D_TEXTURE_SIZE); }
    virtual uint      getHorzScreenDPI(void) const { return 96; }
    virtual uint      getVertScreenDPI(void) const { return 96; }

    // ********************************
    // private variables
    // ********************************
private:
    struct QuadDesc {
        const GarnetTexture * tex;
        float                 z;
        float                 x0;
        float                 y0;
        float                 x1;
        float                 y1;
        float                 u0;
        float                 v0;
        float                 u1;
        float                 v1;
        uint32_t              cx0;
        uint32_t              cx1;
        uint32_t              cy0;
        uint32_t              cy1;
    };

    bool                       mQueueEnabled;
    DynaArray<QuadDesc>        mQuads;
    DynaArray<GarnetTexture *> mTextures;

    // ********************************
    // private functions
    // ********************************
private:
    bool onRendererRestore();
    void onRendererDispose();

    inline void drawQuads(const QuadDesc * quads, size_t count);
};
} // namespace CEGUI

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GUI_CEGUIRENDERER_H__
