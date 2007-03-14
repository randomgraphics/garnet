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
    class QuadRenderer : public SlotBase
    {
        struct QuadVertex
        {
            GN::Vector3f pos;
            UInt32       clr; // color in B-G-R-A format
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
            AutoRef<gfx::VtxBuf> vb[NUM_VTXBUFS];
            AutoRef<gfx::IdxBuf> ib;
            AutoRef<gfx::Shader> vs;
            AutoRef<gfx::Shader> pstex;
            AutoRef<gfx::Shader> pssolid;
        };

        QuadMesh             mMesh;
        gfx::RendererContext mContext;
        size_t               mNumQuads;
        size_t               mActiveVB;
        gfx::Texture *       mTexture;
        BitFields            mOptions;
        QuadVertex *         mNextVtx;
        bool                 mDrawBegun;

    public:

        //@{

        QuadRenderer();

        ~QuadRenderer();

        //@}

        //@{

        enum
        {
            OPT_USER_CONTEXT = 1, ///< using user defined renderer context. This option will overwrite other options.
            OPT_OPAQUE       = 2, ///< draw opaque quad. ignored when OPT_USER_CONTEXT is set.
            OPT_DEPTH_TEST   = 4, ///< enable depth test. ignored when OPT_USER_CONTEXT is set.
            OPT_DEPTH_WRITE  = 8, ///< enable depth write. ignored when OPT_USER_CONTEXT is set.
        };

        ///
        /// \note set texture to NULL, to draw solid colored quads
        ///
        void drawBegin( gfx::Texture * tex, BitFields options = 0 );
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
            UInt32 bgra = 0xFFFFFFFF,
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
            mNextVtx[0].clr = bgra;

            mNextVtx[1].pos.set( x1, y2, z );
            mNextVtx[1].clr = bgra;

            mNextVtx[2].pos.set( x2, y2, z );
            mNextVtx[2].clr = bgra;

            mNextVtx[3].pos.set( x2, y1, z );
            mNextVtx[3].clr = bgra;

            // prepare for next quad
            mNextVtx += 4;
            ++mNumQuads;
        }

        //@}

        /// \name helpers to ease drawing of single quad.
        //@{

        void drawSingleTexturedQuad(
            gfx::Texture * tex,
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
            UInt32    color, // color in B-G-R-A format
            BitFields options,
            float z  = 0.0f,
            float x1 = 0.0f,
            float y1 = 0.0f,
            float x2 = 1.0f,
            float y2 = 1.0f )
        {
            drawBegin( 0, options );
            drawSolid( color, z, x1, y1, x2, y2 );
            drawEnd();
        }

        //@}

    private:

        bool onRendererRestore();
        void onRendererDispose();

        void submitPendingQuads()
        {
            GN_GUARD_SLOW;

            using namespace gfx;


            GN_UNGUARD_SLOW;
        }
    };

    ///
    /// global instance of quad renderer
    ///
    extern QuadRenderer gQuadRenderer;
}}

// *****************************************************************************
//                           End of quad.h
// *****************************************************************************
#endif // __GN_SCENE_QUAD_H__
