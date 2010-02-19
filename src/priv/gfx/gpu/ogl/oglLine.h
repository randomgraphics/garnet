#ifndef __GN_GFXOGL_OGLLINE_H__
#define __GN_GFXOGL_OGLLINE_H__
// *****************************************************************************
/// \file
/// \brief   OGL line renderer
/// \author  chenlee (2005.12.7)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLGpu;

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
        OGLLine( OGLGpu & r ) : mGpu(r) { Clear(); }
        virtual ~OGLLine() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init();
        void Quit();
    private:
        void Clear()
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
        /// \sa Gpu::drawLines
        ///
        void drawLines(
            BitFields options,
            const float * positions,
            size_t stride,
            size_t numpoints,
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
            UInt32   c;
            Vector3f p;
        };
        GN_CASSERT( 16 == sizeof(LineVertex) );

        LineVertex * mVtxBuf;

        OGLGpu & mGpu;

        size_t mNextLine;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLLINE_H__
