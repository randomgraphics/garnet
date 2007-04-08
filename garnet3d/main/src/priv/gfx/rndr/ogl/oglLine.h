#ifndef __GN_GFXOGL_OGLLINE_H__
#define __GN_GFXOGL_OGLLINE_H__
// *****************************************************************************
/// \file    ogl/oglLine.h
/// \brief   OGL line renderer
/// \author  chenlee (2005.12.7)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer;
    
    ///
    /// OGL line renderer
    ///
    class OGLLine : public StdClass
    {
        GN_DECLARE_STDCLASS( OGLLine, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLLine( OGLRenderer & r ) : mRenderer(r) { clear(); }
        virtual ~OGLLine() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear()
        {
            mVtxBuf = 0;
            mNextLine = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Draw lines on screen
        ///
        /// \sa Renderer::drawLines
        ///
        void drawLines(
            BitFields options,
            const float * positions,
            size_t stride,
            size_t count,
            UInt32 rgba,
            const Matrix44f & model,
            const Matrix44f & view,
            const Matrix44f & proj );

        // ********************************
        // private variables
        // ********************************
    private:

        enum { MAX_LINES = 4096 };

        struct LineVertex
        {
            UInt32 c;
            Vector3f p;
        };
        GN_CASSERT( 16 == sizeof(LineVertex) );

        LineVertex * mVtxBuf;

        OGLRenderer & mRenderer;

        size_t mNextLine;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of oglLine.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLLINE_H__
