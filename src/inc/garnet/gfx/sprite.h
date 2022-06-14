#ifndef __GN_GFX_SPRITE_H__
#define __GN_GFX_SPRITE_H__
// *****************************************************************************
/// \file
/// \brief   2D sprite rendering utilites
/// \author  chenli@@REDMOND (2009.1.25)
// *****************************************************************************

namespace GN {
namespace gfx {
///
/// 2D sprite renderer.
///
/// 2D sprite coordinate system:
///     - left-top corner is (0,0)
///     - X is leftward
///     - Y is downward
///     - length unit is pixel
///
class GN_API SpriteRenderer : public StdClass {
    GN_DECLARE_STDCLASS(SpriteRenderer, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SpriteRenderer() { clear(); }
    virtual ~SpriteRenderer() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(Gpu &);
    void quit();

private:
    void clear() {
        mGpu     = nullptr;
        mSprites = nullptr;
    }
    //@}

    // ********************************
    // public functions
    // ********************************
public:
    //@{

    /// sprite drawing options
    enum SpriteDrawOptions {
        USE_EXTERNAL_GPU_PROGRAM = 0x00000001,      ///< Use GPU program in current renderer context,
                                                    ///< instead of sprite renderer's default shaders.
        USE_EXTERNAL_TEXTURE_FILTERS = 0x00000002,  ///< Use texture filters defined in current renderer context.
                                                    ///< By default, sprite renderer uses point filters.
        FORCE_ALPHA_BLENDING_ENABLED  = 0x00000010, ///< Always enable alpha blending, disregarding value in current renderer context.
        FORCE_ALPHA_BLENDING_DISABLED = 0x00000020, ///< Always disable alpha blending, disregarding value in current renderer context.

        FORCE_DEPTH_TEST_ENABLED  = 0x00000100, ///< Always enable depth test, disregarding value in current renderer context.
        FORCE_DEPTH_TEST_DISABLED = 0x00000200, ///< Always disable depth test, disregarding value in current renderer context.

        FORCE_DEPTH_WRITE_ENABLED  = 0x00001000, ///< Always enable depth write, disregarding value in current renderer context.
        FORCE_DEPTH_WRITE_DISABLED = 0x00002000, ///< Always disable depth write, disregarding value in current renderer context.

        /// A commonly used set of options that are usually used to render transparent 2D picture with alpha channel.
        TRANSPARENT_2D_IMAGE = FORCE_ALPHA_BLENDING_ENABLED | FORCE_DEPTH_TEST_DISABLED | FORCE_DEPTH_WRITE_DISABLED,

        /// A commonly used set of options to render opaque sprite
        SOLID_2D_IMAGE = FORCE_DEPTH_TEST_DISABLED | FORCE_DEPTH_WRITE_DISABLED,
    };

    /// get underline renderer
    Gpu & getGpu() const {
        GN_ASSERT(mGpu);
        return *mGpu;
    }

    ///
    /// \note set texture to NULL, to draw solid sprite
    ///
    void drawBegin(Texture * texture, uint32 options);
    void drawEnd();

    ///
    /// By default, x/y/w/h are in screen space. (0,0) is upper left corner of the screen.
    ///
    void drawTextured(float x, float y, float w, float h, float u = 0.0f, float v = 0.0f, float tw = 1.0f, float th = 1.0f, float z = 0.0f);

    ///
    /// By default, x/y/w/h are in screen space. (0,0) is upper left corner of the screen.
    ///
    void drawSolid(uint32 rgba, float x, float y, float w, float h, float z);

    //@}

    /// \name helpers to ease drawing of single sprite.
    //@{

    void drawSingleTexturedSprite(Texture * tex, uint32_t options, float x, float y, float w, float h, float u = 0.0f, float v = 0.0f, float tw = 1.0f,
                                  float th = 1.0f, float z = 0.0f) {
        drawBegin(tex, options);
        drawTextured(x, y, w, h, u, v, tw, th, z);
        drawEnd();
    }

    void drawSingleSolidSprite(uint32_t rgba, // color in R-G-B-A format
                               uint32_t options, float x, float y, float w, float h, float z) {
        drawBegin(0, options);
        drawSolid(rgba, x, y, w, h, z);
        drawEnd();
    }

    //@}

    // ********************************
    // private variables
    // ********************************
private:
    struct SpriteVertex {
        GN::Vector3f pos;
        uint32       clr; // color in R-G-B-A format
        GN::Vector2f tex;
        float        _[2]; // padding to 32 bytes
    };
    GN_CASSERT(sizeof(SpriteVertex) == 32);

    struct Sprite {
        SpriteVertex v[4];
    };

    enum {
        MAX_SPRITES = 256,
    };

    Gpu * mGpu = nullptr;

    AutoRef<Texture>    mPureWhiteTexture;
    AutoRef<GpuProgram> mGpuProgram;
    VertexBinding       mVertexBinding;
    AutoRef<VtxBuf>     mVertexBuffer;
    AutoRef<IdxBuf>     mIndexBuffer;

    GpuContext mContext;

    uint32   mOptions;
    float    mVertexShift;
    bool     mDrawBegun;
    Sprite * mSprites;
    Sprite * mNextPendingSprite;
    Sprite * mNextFreeSprite;

    // ********************************
    // private functions
    // ********************************
private:
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SPRITE_H__
