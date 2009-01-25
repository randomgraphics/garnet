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
        void clear() { mDrawBegun = false; }
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
            float v2 = 1.0f )
        {
            GN_ASSERT( mDrawBegun && mNextVtx );

            if( mNumPendingSprites == MAX_SPRITES_PER_BATCH )
            {
                drawEnd();
                drawBegin( mContext.textures[0].get(), mOptions );
            }

            GN_ASSERT( mNumPendingSprites < MAX_SPRITES_PER_BATCH && mNextVtx );

            // fill vertex buffer

            mNextVtx[0].pos.set( x1, y1, z );
            mNextVtx[0].tex.set( u1, v1 );

            mNextVtx[1].pos.set( x1, y2, z );
            mNextVtx[1].tex.set( u1, v2 );

            mNextVtx[2].pos.set( x2, y2, z );
            mNextVtx[2].tex.set( u2, v2 );

            mNextVtx[3].pos.set( x2, y1, z );
            mNextVtx[3].tex.set( u2, v1 );

            // prepare for next sprite
            mNextVtx += 4;
            ++mNumPendingSprites;
        }

        ///
        /// Note that [0,0] is upper left corner of the screen.
        ///
        void drawSolid(
            UInt32 rgba = 0xFFFFFFFF,
            float z     = 0.0f,
            float x1    = 0.0f,
            float y1    = 0.0f,
            float x2    = 1.0f,
            float y2    = 1.0f )
        {
            GN_ASSERT( mDrawBegun && mNextVtx );

            if( mNumPendingSprites == MAX_SPRITES_PER_BATCH )
            {
                drawEnd();
                drawBegin( mContext.textures[0].get(), mOptions );
            }

            GN_ASSERT( mNumPendingSprites < MAX_SPRITES_PER_BATCH && mNextVtx );

            // fill vertex buffer

            mNextVtx[0].pos.set( x1, y1, z );
            mNextVtx[0].clr = rgba;

            mNextVtx[1].pos.set( x1, y2, z );
            mNextVtx[1].clr = rgba;

            mNextVtx[2].pos.set( x2, y2, z );
            mNextVtx[2].clr = rgba;

            mNextVtx[3].pos.set( x2, y1, z );
            mNextVtx[3].clr = rgba;

            // prepare for next Sprite
            mNextVtx += 4;
            ++mNumPendingSprites;
        }

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

        enum { MAX_SPRITES_PER_BATCH = 256 };

        Renderer                 & mRenderer;
        RendererContext            mContext;
        BitFields                  mOptions;
        bool                       mDrawBegun;
        size_t                     mNumPendingSprites;
        SpriteVertex             * mNextVtx;

        // ********************************
        // private functions
        // ********************************
    private:
    };}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SPRITE_H__
