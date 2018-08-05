#ifndef __GN_GFX_OGLRENDERTARGETMGR_H__
#define __GN_GFX_OGLRENDERTARGETMGR_H__
// *****************************************************************************
/// \file
/// \brief   OGL render target manager
/// \author  chen@@CHENLI-HOMEPC (2007.1.11)
// *****************************************************************************

#include "../common/basicGpu.h"

namespace GN { namespace gfx
{
    class OGLGpu;

    ///
    /// Basic render target manager
    ///
    class OGLBasicRTMgr
    {
    protected:

        OGLGpu & mGpu; ///< reference to GPU instance;
        Vector2<uint32> mRenderTargetSize;  ///< render target size

        ///
        /// protected ctor.
        ///
        OGLBasicRTMgr( OGLGpu & r ) : mGpu( r ) {}

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
        const Vector2<uint32> & getRenderTargetSize() const { return mRenderTargetSize; }

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
        OGLRTMgrCopyFrame( OGLGpu & r ) : OGLBasicRTMgr(r) { clear(); }
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

        void clear() { mRenderTargetSize.set( 0, 0 ); }
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
        OGLRTMgrFBO( OGLGpu & r ) : OGLBasicRTMgr(r) { clear(); }
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
        Vector2<uint32> mAutoZSize;

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
