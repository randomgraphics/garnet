#ifndef __GN_GFX_SPRITE_H__
#define __GN_GFX_SPRITE_H__
// *****************************************************************************
/// \file
/// \brief   2D sprite rendering utilites
/// \author  chenli@@REDMOND (2009.1.25)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// 2D sprite renderer
    ///
    class SpriteRenderer : public StdClass
    {
        GN_DECLARE_STDCLASS( SpriteRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SpriteRenderer( Renderer & r ) : mRenderer(r) { clear(); }
        virtual ~SpriteRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() { mSprites = NULL; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        /// sprite drawing options
        enum SpriteDrawOptions
        {
            USE_COSTOM_CONTEXT = 1, ///< using user defined custom renderer context. This option will overwrite other options.
            OPAQUE_SPRITE      = 2, ///< draw opaque sprite. ignored when USE_COSTOM_CONTEXT is set.
            ENABLE_DEPTH_TEST  = 4, ///< enable depth test. ignored when USE_COSTOM_CONTEXT is set.
            ENABLE_DEPTH_WRITE = 8, ///< enable depth write. ignored when USE_COSTOM_CONTEXT is set.
        };

        ///
        /// \note set texture to NULL, to draw solid sprite
        ///
        void drawBegin( Texture * texture, BitFields options = 0 );
        void drawEnd();

        ///
        /// Note that [0,0] is upper left corner of the screen.
        ///
        void drawTextured(
            float z  = 0.0f,
            float x1 = 0.0f,
            float y1 = 0.0f,
            float x2 = 1.0f,
            float y2 = 1.0f,
            float u1 = 0.0f,
            float v1 = 0.0f,
            float u2 = 1.0f,
            float v2 = 1.0f );

        ///
        /// Note that [0,0] is upper left corner of the screen.
        ///
        void drawSolid(
            UInt32 rgba = 0xFFFFFFFF,
            float z     = 0.0f,
            float x1    = 0.0f,
            float y1    = 0.0f,
            float x2    = 1.0f,
            float y2    = 1.0f );

        //@}

        /// \name helpers to ease drawing of single sprite.
        //@{

        void drawSingleTexturedSprite(
            Texture * tex,
            BitFields options,
            float     z  = 0.0f,
            float     x1 = 0.0f,
            float     y1 = 0.0f,
            float     x2 = 1.0f,
            float     y2 = 1.0f,
            float     u1 = 0.0f,
            float     v1 = 0.0f,
            float     u2 = 1.0f,
            float     v2 = 1.0f )
        {
            drawBegin( tex, options );
            drawTextured( z, x1, y1, x2, y2, u1, v1, u2, v2 );
            drawEnd();
        }

        void drawSingleSolidSprite(
            UInt32    rgba, // color in R-G-B-A format
            BitFields options,
            float     z  = 0.0f,
            float     x1 = 0.0f,
            float     y1 = 0.0f,
            float     x2 = 1.0f,
            float     y2 = 1.0f )
        {
            drawBegin( 0, options );
            drawSolid( rgba, z, x1, y1, x2, y2 );
            drawEnd();
        }

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct SpriteVertex
        {
            GN::Vector3f pos;
            UInt32       clr; // color in R-G-B-A format
            GN::Vector2f tex;
            float        _[2]; // padding to 32 bytes
        };
        GN_CASSERT( sizeof(SpriteVertex) == 32 );

        struct Sprite
        {
            SpriteVertex v[4];
        };

        enum
        {
            MAX_SPRITES  = 256,
        };

        Renderer                 & mRenderer;
        AutoRef<VtxBuf>            mVtxBuf;
        AutoRef<IdxBuf>            mIdxBuf;
        RendererContext            mPrivateContext;
        RendererContext            mEnvironmentContext;
        RendererContext          * mEffectiveContext;
        BitFields                  mOptions;
        bool                       mDrawBegun;
        Sprite                   * mSprites;
        Sprite                   * mNextPendingSprite;
        Sprite                   * mNextFreeSprite;

        // ********************************
        // private functions
        // ********************************
    private:
    };}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SPRITE_H__
