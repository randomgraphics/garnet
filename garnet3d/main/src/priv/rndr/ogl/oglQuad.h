#ifndef __GN_GFXOGL_OGLQUAD_H__
#define __GN_GFXOGL_OGLQUAD_H__
// *****************************************************************************
//! \file    ogl/oglQuad.h
//! \brief   OGL quad renderer
//! \author  chenlee (2005.12.7)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer;
    
    //!
    //! OGL quad renderer
    //!
    class OGLQuad : public StdClass
    {
        GN_DECLARE_STDCLASS( OGLQuad, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLQuad( OGLRenderer & r ) : mRenderer(r) { clear(); }
        virtual ~OGLQuad() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mVtxBuf = 0;
            mNextQuad = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Draw quads on screen
        //!
        //! \sa Renderer::drawQuads
        //!
        void drawQuads(
            BitField options,
            const float * positions, size_t posStride,
            const float * texCoords, size_t texStride,
            const uint32_t * colors, size_t clrStride,
            size_t count );

        // ********************************
        // private variables
        // ********************************
    private:

        enum { MAX_QUADS = 4096 };

        struct QuadVertex
        {
            Vector2f t;
            uint32_t c;
            Vector3f p;
            Vector2f _; // padding to 32 bytes
        };
        GN_CASSERT( 32 == sizeof(QuadVertex) );

        QuadVertex * mVtxBuf;

        OGLRenderer & mRenderer;

        size_t   mNextQuad;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of oglQuad.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLQUAD_H__
