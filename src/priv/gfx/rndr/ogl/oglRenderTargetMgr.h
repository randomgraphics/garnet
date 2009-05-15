#ifndef __GN_GFX_OGLRENDERTARGETMGR_H__
#define __GN_GFX_OGLRENDERTARGETMGR_H__
// *****************************************************************************
/// \file
/// \brief   OGL render target manager
/// \author  chen@@CHENLI-HOMEPC (2007.1.11)
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

        UInt32 mColorWidth;  ///< color buffer width.
        UInt32 mColorHeight; ///< color buffer height.
        UInt32 mDepthWidth;  ///< depth buffer width.
        UInt32 mDepthHeight; ///< depth buffer height.

        ///
        /// protected ctor.
        ///
        OGLBasicRTMgr( OGLRenderer & r ) : mRenderer( r ) {}

    public:

        ///
        /// dtor
        ///
        virtual ~OGLBasicRTMgr() {}

        ///
        /// initialize
        ///
        virtual bool init() = 0;

        ///
        /// Get color render target size
        ///
        void getColorRenderTargetSize( UInt32 & w, UInt32 & h ) const { w = mColorWidth; h = mColorHeight; };

        ///
        /// Get depth render target size
        ///
        void getDepthStencilSize( UInt32 & w, UInt32 & h ) const { w = mDepthWidth; h = mDepthHeight; }

        ///
        /// bind render target to device.
        ///
        virtual bool bind(
            const RenderTargetDesc & oldRT,
            const RenderTargetDesc & newRT,
            bool                     skipDirtyCheck ) = 0;
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
        virtual bool bind( const RenderTargetDesc &, const RenderTargetDesc &, bool );

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:

        void clear() { mColorWidth = 0; mColorHeight = 0; mDepthWidth = 0; mDepthHeight = 0; }
        void quit() { clear(); }
    };

    ///
    /// render target manager using FBO extension
    ///
    class OGLRTMgrFBO : public OGLBasicRTMgr
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLRTMgrFBO( OGLRenderer & r ) : OGLBasicRTMgr(r) { clear(); }
        virtual ~OGLRTMgrFBO() { quit(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// check where FBO is usable or not.
        ///
        static bool usable()
        {
            return GLEW_EXT_framebuffer_object && GLEW_ARB_draw_buffers;
        };

        virtual bool init();
        virtual bool bind(
            const RenderTargetDesc & oldRT,
            const RenderTargetDesc & newRT,
            bool                     skipDirtyCheck );

        // ********************************
        // private variables
        // ********************************
    private:

        GLuint mFbo;
        GLuint mAutoZ;
        Vector2<UInt32> mAutoZSize;

        // ********************************
        // private functions
        // ********************************
    private:

        void quit();
        void clear() { mFbo = 0; mAutoZ = 0; mAutoZSize.set(0,0); }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_OGLRENDERTARGETMGR_H__
