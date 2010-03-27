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
        LineRenderer( Gpu & r ) : mGpu(r) { Clear(); }
        virtual ~LineRenderer() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init();
        void Quit();
    private:
        void Clear() { mLines = 0; mBatchingModeEnabled = true; }
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
        void DrawLines(
            const void *      positions, // 3D positions
            size_t            stride,    // position buffer stride. 0 for default stride (3 floats).
            size_t            numpoints,
            UInt32            colorInRgba,
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
            UInt32    colorInRGBA;
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
