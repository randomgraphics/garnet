#ifndef __GN_GFXOGL_OGLQUAD_H__
#define __GN_GFXOGL_OGLQUAD_H__
// *****************************************************************************
//! \file    ogl/oglQuad.h
//! \brief   OGL quad renderer
//! \author  chenlee (2005.12.7)
// *****************************************************************************

namespace GN { namespace gfx
{
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
        OGLQuad( Renderer & r ) : mRenderer(r) { clear(); }
        virtual ~OGLQuad() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const
        {
            return MyParent::ok()
                && 0 != mRsb
                && 0 != mVtxBinding
                && 0 != mVtxBuf
                && 0 != mIdxBuf;
        }
    private:
        void clear()
        {
            mRsb = 0;
            mVtxBinding = 0;
            mVtxBuf = 0;
            mIdxBuf = 0;
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
            const Vector2f * positions, size_t posStride,
            const Vector2f * texCoords, size_t texStride,
            size_t count, uint32_t options );

        // ********************************
        // private variables
        // ********************************
    private:

        enum { MAX_QUADS = 4096 };

        Renderer & mRenderer;

        uint32_t mRsb;
        uint32_t mVtxBinding;
        VtxBuf * mVtxBuf;
        IdxBuf * mIdxBuf;

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
