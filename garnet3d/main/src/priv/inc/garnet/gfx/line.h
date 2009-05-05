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
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        void drawLines(
            const void *      positions,
            size_t            stride,
            size_t            numpoints,
            UInt32            colorInRgba,
            const Matrix44f & model,
            const Matrix44f & view,
            const Matrix44f & proj );

        // ********************************
        // private variables
        // ********************************
    private:

        Renderer        & mRenderer;
        RendererContext   mContext;

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
