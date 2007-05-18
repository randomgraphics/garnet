#ifndef __GN_SCENE_QUAD_H__
#define __GN_SCENE_QUAD_H__
// *****************************************************************************
//! \file    scene/quad.h
//! \brief   Quad renderer
//! \author  chen@@CHENLI-HOMEPC (2007.3.13)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// screen aligned quad renderer
    ///    
    class QuadRenderer : public StdClass
    {
        GN_DECLARE_STDCLASS( QuadRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        QuadRenderer( engine::RenderEngine & eng ) : mRenderEngine(eng) { clear(); }
        virtual ~QuadRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() { mActiveVB = 0; mDrawBegun = false; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        enum
        {
            OPT_USER_CONTEXT = 1, ///< using user defined renderer context. This option will overwrite other options.
            OPT_OPAQUE       = 2, ///< draw opaque quad. ignored when OPT_USER_CONTEXT is set.
            OPT_DEPTH_TEST   = 4, ///< enable depth test. ignored when OPT_USER_CONTEXT is set.
            OPT_DEPTH_WRITE  = 8, ///< enable depth write. ignored when OPT_USER_CONTEXT is set.
        };

        ///
        /// get the render engine
        ///
        engine::RenderEngine & renderEngine() const { return mRenderEngine; }

        ///
        /// \note set texture to NULL, to draw solid colored quads
        ///
        void drawBegin( engine::GraphicsResource * texture, BitFields options = 0 );
        void drawSolidBegin( BitFields options = 0 ) { drawBegin( 0, options ); }
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

            if( mNumQuads == MAX_QUADS )
            {
                drawEnd();
                drawBegin( mTexture, mOptions );
            }

            GN_ASSERT( mNumQuads < MAX_QUADS && mNextVtx );

            // fill vertex buffer

            mNextVtx[0].pos.set( x1, y1, z );
            mNextVtx[0].tex.set( u1, v1 );

            mNextVtx[1].pos.set( x1, y2, z );
            mNextVtx[1].tex.set( u1, v2 );

            mNextVtx[2].pos.set( x2, y2, z );
            mNextVtx[2].tex.set( u2, v2 );

            mNextVtx[3].pos.set( x2, y1, z );
            mNextVtx[3].tex.set( u2, v1 );

            // prepare for next quad
            mNextVtx += 4;
            ++mNumQuads;
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

            if( mNumQuads == MAX_QUADS )
            {
                drawEnd();
                drawBegin( mTexture, mOptions );
            }

            GN_ASSERT( mNumQuads < MAX_QUADS && mNextVtx );

            // fill vertex buffer

            mNextVtx[0].pos.set( x1, y1, z );
            mNextVtx[0].clr = rgba;

            mNextVtx[1].pos.set( x1, y2, z );
            mNextVtx[1].clr = rgba;

            mNextVtx[2].pos.set( x2, y2, z );
            mNextVtx[2].clr = rgba;

            mNextVtx[3].pos.set( x2, y1, z );
            mNextVtx[3].clr = rgba;

            // prepare for next quad
            mNextVtx += 4;
            ++mNumQuads;
        }

        //@}

        /// \name helpers to ease drawing of single quad.
        //@{

        void drawSingleTexturedQuad(
            engine::GraphicsResource * tex,
            BitFields options,
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
            drawBegin( tex, options );
            drawTextured( z, x1, y1, x2, y2, u1, v1, u2, v2 );
            drawEnd();
        }

        void drawSingleSolidQuad(
            UInt32    rgba, // color in R-G-B-A format
            BitFields options,
            float z  = 0.0f,
            float x1 = 0.0f,
            float y1 = 0.0f,
            float x2 = 1.0f,
            float y2 = 1.0f )
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

        struct QuadVertex
        {
            GN::Vector3f pos;
            UInt32       clr; // color in R-G-B-A format
            GN::Vector2f tex;
            float        _[2]; // padding to 32 bytes
        };
        GN_CASSERT( sizeof(QuadVertex) == 32 );

        enum
        {
            NUM_VTXBUFS = 16,
            MAX_QUADS   = 256,
        };

        struct QuadMesh
        {
            engine::AutoGraphicsResource vf; ///< vertex format
            engine::AutoGraphicsResource vb[NUM_VTXBUFS];
            engine::AutoGraphicsResource ib;
            engine::AutoGraphicsResource vs;
            engine::AutoGraphicsResource pstexed;
            engine::AutoGraphicsResource pssolid;
        };

        engine::RenderEngine     & mRenderEngine;
        QuadMesh                   mMesh;
        engine::DrawContext        mContext;
        size_t                     mNumQuads;
        size_t                     mActiveVB;
        engine::GraphicsResource * mTexture;
        BitFields                  mOptions;
        bool                       mDrawBegun;

        QuadVertex *               mNextVtx;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of quad.h
// *****************************************************************************
#endif // __GN_SCENE_QUAD_H__
