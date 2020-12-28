#ifndef __GN_GFX_OGLGPU_H__
#define __GN_GFX_OGLGPU_H__
// *****************************************************************************
/// \file
/// \brief   OGL GPU class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicGpu.h"
#include "garnet/GNogl.h"

namespace GN { namespace gfx
{
    class OGLResource;
    class OGLGpuProgram;
    class OGLVtxFmt;
    class OGLBasicRTMgr;

    ///
    /// OGL specific caps
    ///
    struct OGLGpuCaps : public GpuCaps
    {
        uint32 maxVertexAttributes; ///< query GL_MAX_VERTEX_ATTRIBS_ARB
    };

    ///
    /// OGL GPU class
    ///
    class OGLGpu : public BasicGpu
    {
        GN_DECLARE_STDCLASS(OGLGpu, BasicGpu);

        static Logger * sLogger;

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        OGLGpu() { clear(); }
        virtual ~OGLGpu() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{

    public:

        bool init( const GpuOptions & );
        void quit();

    private:

        void clear()
        {
            dispClear();
            capsClear();
            resourceClear();
            contextClear();
            drawClear();
        }

        //@}

    // ************************************************************************
    //
    /// \name                     Display Manager
    //
    // ************************************************************************

        //@{

    public:

        virtual void * getD3DDevice() const { return 0; }
        virtual void * getOGLRC() const { return mRenderContext; }

#if GN_WINPC
    private :
        bool dispInit();
        void dispQuit();
        void dispClear()
        {
            mDeviceContext = 0;
            mRenderContext = 0;
            mHook = 0;
            mDisplayModeActivated = false;
            mIgnoreMsgHook = false;
        }

        bool activateDisplayMode();
        void restoreDisplayMode();
        void msgHook( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticHookProc( int code, WPARAM wp, LPARAM lp );

    private:

        typedef Dictionary<intptr_t, OGLGpu*> WindowMap;
        static WindowMap msInstanceMap;

        HDC     mDeviceContext;
        HGLRC   mRenderContext;
        HHOOK   mHook;
        bool    mDisplayModeActivated;
        bool    mIgnoreMsgHook;

#elif GN_POSIX
    private :
        bool dispInit();
        void dispQuit();
        void dispClear()
        {
            mRenderContext = 0;
        }

    private :
        GLXContext mRenderContext;
#endif

        //@}

    // ************************************************************************
    //
    /// \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual const GpuCaps & caps() const { return mCaps; }
        virtual bool            checkTextureFormatSupport( ColorFormat format, TextureUsage usages ) const;

    public :

        const OGLGpuCaps & getOGLCaps() const { return mCaps; }

    private :
        bool capsInit();
        void capsQuit() {}
        void capsClear() { memset( &mCaps, 0, sizeof(mCaps) ); }

    private:

        OGLGpuCaps mCaps;

        //@}

    // ************************************************************************
    //
    /// \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual GpuProgram * createGpuProgram( const GpuProgramDesc & desc );
        virtual Uniform    * createUniform( uint32 size );
        virtual Texture    * createTexture( const TextureDesc & desc );
        virtual VtxBuf     * createVtxBuf( const VtxBufDesc & desc );
        virtual IdxBuf     * createIdxBuf( const IdxBufDesc & desc );

    public:

        ///
        /// Insert resource into resource list. Can be only called by
        /// constructor of OGLResource.
        ///
        void insertResource( OGLResource * p ) { mResourceList.push_back(p); }

        ///
        /// Remove resource from resource list. Can be only called by
        /// destructor of OGLResource.
        ///
        void removeResource( OGLResource * p ) { mResourceList.remove(p); }

    private :

        bool resourceInit();
        void resourceQuit();
        void resourceClear() {}

    private:

        std::list<OGLResource*> mResourceList;

        //@}

    // ************************************************************************
    //
    /// \name                   Context Manager
    //
    // ************************************************************************

        //@{

    public:

        virtual bool bindContextImpl( const GpuContext & context, bool skipDirtyCheck );

    public:

        void chooseTextureStage( size_t ) const; ///< Choose one texture stage as active stage
        void disableTextureStage( size_t ) const; ///< Disable one texture stage

    private:

        bool contextInit();
        void contextQuit();
        void contextClear() { mContext.clear(); mCurrentOGLVtxFmt = NULL; mRTMgr = NULL; }

        inline OGLVtxFmt * findOrCreateOGLVtxFmt( const VertexBinding & vtxbind, const OGLGpuProgram * gpuProgram );
        inline bool bindContextShaders( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextRenderStates( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextRenderTargets( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextResources( const GpuContext & newContext, bool skipDirtyCheck );

    private:

        struct VertexFormatKey
        {
            VertexBinding vtxbind;
            uint64        shaderID;

            bool operator<( const VertexFormatKey & rhs ) const
            {
                if( this == &rhs ) return false;
                if( shaderID < rhs.shaderID ) return true;
                if( shaderID > rhs.shaderID ) return false;
                if( vtxbind.size() < rhs.vtxbind.size() ) return true;
                if( vtxbind.size() > rhs.vtxbind.size() ) return false;

                for( size_t i = 0; i < vtxbind.size(); ++i )
                {
                    const VertexElement & b1 = vtxbind[i];
                    const VertexElement & b2 = rhs.vtxbind[i];

                    if( b1 < b2 ) return true;
                    if( b1 > b2 ) return false;
                }

                return false;
            }
        };

        Dictionary<VertexFormatKey,OGLVtxFmt*> mVertexFormats;
        OGLVtxFmt                            * mCurrentOGLVtxFmt;
        OGLBasicRTMgr                        * mRTMgr;

        //@}

    // ************************************************************************
    //
    /// \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public:

        virtual void present();
        virtual void clearScreen( const Vector4f & c, float z, uint8 s, uint32 flags );
        virtual void drawIndexed( PrimitiveType prim,
                                  uint32        numidx,
                                  uint32        basevtx,
                                  uint32        startvtx,
                                  uint32        numvtx,
                                  uint32        startidx );
        virtual void draw( PrimitiveType prim,
                           uint32        numvtx,
                           uint32        startvtx );
        virtual void drawIndexedUp(
                             PrimitiveType  prim,
                             uint32         numidx,
                             uint32         numvtx,
                             const void *   vertexData,
                             uint32         strideInBytes,
                             const uint16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             uint32        numvtx,
                             const void *  vertexData,
                             uint32        strideInBytes );

    private:

        bool drawInit();
        void drawQuit();
        void drawClear()
        {
            mCurrentStartVtx = (size_t)-1;
            mFrameCounter = 0;
            mDrawCounter = 0;
            mUserVBO.cleanup();
            mUserIBO.cleanup();
        }

    private:

        size_t    mCurrentStartVtx;
        size_t    mFrameCounter;
        size_t    mDrawCounter;
        ogl::BufferObject<GL_ARRAY_BUFFER> mUserVBO;
        ogl::BufferObject<GL_ELEMENT_ARRAY_BUFFER> mUserIBO;

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

    public:

        virtual void debugDumpNextFrame( uint32 startBatchIndex, uint32 numBatches )
        {
            GN_UNUSED_PARAM( startBatchIndex );
            GN_UNUSED_PARAM( numBatches );
            GN_TODO( "OpenGL frame dump is not implemented." );
        }

        //@}
    };
}}

#include "oglContextMgr.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_OGLGPU_H__
