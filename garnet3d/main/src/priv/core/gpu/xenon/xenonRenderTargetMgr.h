#ifndef __GN_GFX_XENONRENDERTARGETMGR_H__
#define __GN_GFX_XENONRENDERTARGETMGR_H__
// *****************************************************************************
/// \file
/// \brief   Xenon render target manager
/// \author  chen@@CHENLI-HOMEPC (2007.1.11)
// *****************************************************************************

namespace GN { namespace gfx
{
    class XenonGpu;

    ///
    /// Xenon render target description
    ///
    struct XenonRenderTargetDesc
    {
        RenderTargetTexture * crts; ///< color render targets.
        RenderTargetTexture * dsrt; ///< depth stencil render targets.
    };

    ///
    /// Xenon render target manager
    ///
    class XenonRenderTargetManager : public StdClass
    {
        GN_DECLARE_STDCLASS( XenonRenderTargetManager, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonRenderTargetManager( XenonGpu & r ) : mGpu(r) { clear(); }
        virtual ~XenonRenderTargetManager() { quit(); }
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

        ///
        /// bind render target to device
        ///
        void bind(
            const XenonRenderTargetDesc & oldDesc,
            const XenonRenderTargetDesc & newDesc,
            bool                          skipDirtyCheck,
            bool                        & needRebindViewport );

        // ********************************
        // private variables
        // ********************************
    private:

        XenonGpu & mGpu;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONRENDERTARGETMGR_H__
