#ifndef __GN_GFX_D3D9RENDERTARGETMGR_H__
#define __GN_GFX_D3D9RENDERTARGETMGR_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 render target manager
/// \author  chen@@CHENLI-HOMEPC (2007.1.11)
// *****************************************************************************

namespace GN { namespace gfx
{
    class D3D9Renderer;

#if GN_XENON

    ///
    /// D3D9 render target manager for PC
    ///
    class D3D9RTMgrXenon : public StdClass
    {
        GN_DECLARE_STDCLASS( D3D9RTMgrPC, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9RTMgrXenon( D3D9Renderer & r ) : mRenderer(r) { clear(); }
        virtual ~D3D9RTMgrXenon() { quit(); }
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
            const RenderTargetDesc & oldDesc,
            const RenderTargetDesc & newDesc,
            bool forceRebind,
            bool & needRebindViewport );

        // ********************************
        // private variables
        // ********************************
    private:

        D3D9Renderer & mRenderer;

        // ********************************
        // private functions
        // ********************************
    private:
    };

#else

    ///
    /// D3D9 render target manager for PC
    ///
    class D3D9RTMgrPC : public StdClass
    {
        GN_DECLARE_STDCLASS( D3D9RTMgrPC, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9RTMgrPC( D3D9Renderer & r ) : mRenderer(r) { clear(); }
        virtual ~D3D9RTMgrPC() { quit(); }
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
            const RenderTargetDesc & oldDesc,
            const RenderTargetDesc & newDesc,
            bool forceRebind,
            bool & needRebindViewport );

        // ********************************
        // private variables
        // ********************************
    private:

        D3D9Renderer & mRenderer;

        AutoComPtr<IDirect3DSurface9> mAutoColor0, mAutoDepth;

        // ********************************
        // private functions
        // ********************************
    private:

    };

#endif
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_D3D9RENDERTARGETMGR_H__
