#ifndef __GN_GFX_OGLRENDERTARGETMGR_H__
#define __GN_GFX_OGLRENDERTARGETMGR_H__
// *****************************************************************************
//! \file    ogl/oglRenderTargetMgr.h
//! \brief   OGL render target manager
//! \author  chen@@CHENLI-HOMEPC (2007.1.11)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer;
    
    ///
    /// Basic render target manager
    ///
    class OGLBasicRTMgr
    {
    protected:

        OGLRenderer & mRenderer; ///< reference to renderer instance;

        UInt32 mWidth;  ///< color buffer width
        UInt32 mHeight; ///< color buffer height

        ///
        /// protected ctor.
        ///
        OGLBasicRTMgr( OGLRenderer & r ) : mRenderer( r ) {}

    public:

        ///
        /// initialize
        ///
        virtual bool init() = 0;

        ///
        /// Get render target size
        ///
        void getRTSize( UInt32 & w, UInt32 & h ) const { w = mWidth; h = mHeight; };

        ///
        /// bind render target to device.
        ///
        virtual void bind(
            const RenderTargetDesc & oldRT,
            const RenderTargetDesc & newRT,
            bool forceRebind,
            bool & needRebindViewport ) = 0;
    };

    ///
    /// implement render target by copying data from frame buffer.
    /// No extension required, but only supports single RT texture.
    ///
    class OGLRTMgrCopyFrame : public OGLBasicRTMgr
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLRTMgrCopyFrame( OGLRenderer & r ) : OGLBasicRTMgr(r) { clear(); }
        virtual ~OGLRTMgrCopyFrame() { quit(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        virtual bool init() { return true; }
        virtual void bind( const RenderTargetDesc &, const RenderTargetDesc &, bool, bool &);

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:

        void clear() { mWidth = 0; mHeight = 0; }
        void quit() { clear(); }

    };
}}

// *****************************************************************************
//                           End of oglRenderTargetMgr.h
// *****************************************************************************
#endif // __GN_GFX_OGLRENDERTARGETMGR_H__
