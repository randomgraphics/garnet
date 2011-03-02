#ifndef __GN_GFX_LINE_H__
#define __GN_GFX_LINE_H__
// *****************************************************************************
/// \file
/// \brief   Colored line renderer
/// \author  chenli@@REDMOND (2009.5.4)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Colored line renderer
    ///
    class LineRenderer : public StdClass
    {
        GN_DECLARE_STDCLASS( LineRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        LineRenderer( Gpu & r ) : mGpu(r) { clear(); }
        virtual ~LineRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() { mLines = 0; mBatchingModeEnabled = true; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Enable/Disable batch rendering mode. It is enabled by default.
        ///
        void setBatchRenderingEnable( bool enabled ) { flush(); mBatchingModeEnabled = enabled; }

        ///
        /// draw line list
        ///
        void drawLines(
            const void *      positions, // 3D positions
            size_t            stride,    // position buffer stride. 0 for default stride (3 floats).
            size_t            numpoints,
            uint32            colorInRgba,
            const Matrix44f & transform );

        ///
        /// draw a wireframe box
        ///
        void drawBox(
            const Boxf      & box,
            uint32            colorInRgba,
            const Matrix44f & transform );

        ///
        /// submit any pending line drawing requests to renderer
        ///
        void flush();

        // ********************************
        // private variables
        // ********************************
    private:

        struct LineVertex
        {
            Vector3f  pos;
            uint32    colorInRGBA;
            Matrix44f transform;
        };

        struct Line
        {
            LineVertex v0, v1;
        };

        enum
        {
            MAX_LINES = 256,
        };

        Gpu        & mGpu;
        GpuContext   mContext;
        Line            * mLines;
        Line            * mNextPendingLine;
        Line            * mNextFreeLine;
        bool              mBatchingModeEnabled;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_LINE_H__
