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
    /// Colored line Renderer
    ///
    class LineRenderer : public StdClass
    {
        GN_DECLARE_STDCLASS( LineRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        LineRenderer( Renderer & r ) : mRenderer(r) { clear(); }
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
            const void *      positions,
            size_t            stride,
            size_t            numpoints,
            UInt32            colorInRgba,
            const Matrix44f & projViewWorld );

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
            Matrix44f pvw;
        };

        struct Line
        {
            LineVertex v0, v1;
        };

        enum
        {
            MAX_LINES = 256,
        };

        Renderer        & mRenderer;
        RendererContext   mContext;
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
