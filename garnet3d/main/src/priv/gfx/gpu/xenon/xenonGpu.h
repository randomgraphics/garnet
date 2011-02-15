#ifndef __GN_GFX_XENONGPU_H__
#define __GN_GFX_XENONGPU_H__
// *****************************************************************************
/// \file
/// \brief   Xenon GPU class
/// \author  chenlee (2004.4.1) -- happy april fool's day!!!
// *****************************************************************************

#include "../common/basicGpuXenon.h"

namespace GN { namespace gfx
{
    class XenonResource; // Forward declaration of XenonResource.
    class XenonRenderTargetManager;
    class XenonLine;

    ///
    /// Xenon GPU class
    ///
    class XenonGpu : public BasicGpuXenon
    {
        GN_DECLARE_STDCLASS(XenonGpu, BasicGpuXenon);

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        XenonGpu() { clear(); }
        virtual ~XenonGpu() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuOptions & ro );
        void quit();
    private:
        void clear()
        {
            dispClear();
            capsClear();
            resourceClear();
            contextClear();
            drawClear();
            miscClear();
            mThread = NULL;
        }
        const Thread * mThread;
        bool isGpuThread() const { return NULL == mThread || mThread->isCurrentThread(); }
        //@}

    // ************************************************************************
    //
    /// \name                     Display Manager
    //
    // ************************************************************************

        //@{

    public :

        // from Gpu
        virtual void * getD3DDevice() const { return mDevice; }
        virtual void * getOGLRC() const { return 0; }

    public :

        //
        // Access to D3D interfaces
        //
        IDirect3D9                  & getD3D() const { GN_ASSERT(mD3D); return *mD3D; }
        IDirect3DDevice9            & getDeviceInlined() const { GN_ASSERT(mDevice); return *mDevice; }
        UINT                          getBehavior() const { return mD3DBehavior; }
        const D3DPRESENT_PARAMETERS & getPresentParameters() const { return mD3DPresentParameters; }

    private :
        bool dispInit();
        void dispQuit();
        void dispClear()
        {
            mD3D = 0;
            mDevice = 0;
        }

        bool dispDeviceCreate();
        bool dispDeviceRestore();
        void dispDeviceDispose();
        void dispDeviceDestroy();

    private :

        UINT                    mD3DBehavior;
        D3DPRESENT_PARAMETERS   mD3DPresentParameters;
        IDirect3D9            * mD3D;
        IDirect3DDevice9      * mDevice;

        //@}

    // ************************************************************************
    //
    /// \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual const GpuCaps & caps() const { return mCaps; }
        virtual bool                 checkTextureFormatSupport( ColorFormat format, TextureUsage usages ) const;
        virtual ColorFormat          getDefaultTextureFormat( TextureUsage usages ) const;

    private :

        bool capsInit();
        void capsQuit() {}
        void capsClear() {}

    private :

        GpuCaps mCaps;

        //@}

    // ************************************************************************
    //
    /// \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual Blob       * compileGpuProgram( const GpuProgramDesc & desc );
        virtual GpuProgram * createGpuProgram( const void * compiledGpuProgramBinary, size_t length );
        virtual Uniform    * createUniform( size_t size );
        virtual Texture    * createTexture( const TextureDesc & desc );
        virtual VtxBuf     * createVtxBuf( const VtxBufDesc & desc );
        virtual IdxBuf     * createIdxBuf( const IdxBufDesc & desc );

    public :

        ///
        /// Insert resource into resource list. Can be only called by
        /// constructor of XenonResource.
        ///
        void insertResource( XenonResource * p )
        {
            mResourceList.push_back(p);
        }

        ///
        /// Remove resource from resource list. Can be only called by
        /// destructor of XenonResource.
        ///
        void removeResource( XenonResource * p )
        {
            mResourceList.remove(p);
        }

    private:

        bool resourceInit();
        void resourceQuit();
        void resourceClear() { mLine = 0; }

    private :

        std::list<XenonResource*> mResourceList;

        //@}


    // ********************************************************************
    //
    /// \name Context manager
    //
    // ********************************************************************

        //@{

    public:

        virtual bool bindContextImpl( const GpuContext & context, bool skipDirtyCheck );

    public:

        void setD3DRenderState( D3DRENDERSTATETYPE, DWORD );
        void setD3DSamplerState( UINT, D3DSAMPLERSTATETYPE, DWORD );

    private :

        bool contextInit();
        void contextQuit();
        void contextClear() { mContext.clear(); mRTMgr = 0; mCurrentDecl = 0; }

        inline bool bindContextRenderTargetsAndViewport( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextRenderStates( const GpuContext & newContext, bool skipDirtyCheck  );
        inline bool bindContextShaders( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextResources( const GpuContext & newContext, bool skipDirtyCheck );

    private:

        struct XenonVertexDeclDesc
        {
            uint32        shaderID;
            VertexBinding vtxbind;

            bool operator<( const XenonVertexDeclDesc & rhs ) const
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

        //typedef GN::HashMap<
        //    XenonVertexDeclDesc,
        //    IDirect3DVertexDeclaration9*,
        //    HashMapUtils::HashFunc_MemoryHash<XenonVertexDeclDesc> > XenonVertexDeclarationDict;
        typedef GN::Dictionary<XenonVertexDeclDesc,IDirect3DVertexDeclaration9*> XenonVertexDeclarationDict;

        XenonRenderTargetManager    * mRTMgr;
        XenonVertexDeclarationDict    mVertexFormats;
        IDirect3DVertexDeclaration9 * mCurrentDecl;

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
                                  size_t        numidx,
                                  size_t        basevtx,
                                  size_t        startvtx,
                                  size_t        numvtx,
                                  size_t        startidx );
        virtual void draw( PrimitiveType prim,
                           size_t        numvtx,
                           size_t        startvtx );
        virtual void drawIndexedUp(
                             PrimitiveType  prim,
                             size_t         numidx,
                             size_t         numvtx,
                             const void *   vertexData,
                             size_t         strideInBytes,
                             const uint16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             size_t        numvtx,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void drawLines( uint32         options,
                                const void *      positions,
                                size_t            stride,
                                size_t            numpoints,
                                uint32            rgba,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );

        // private functions
    private:
        bool drawInit();
        void drawQuit();
        void drawClear()
        {
            mLine = NULL;
            mFrameCounter = 0;
            mDrawCounter = 0;
            mSceneBegun = false;
        }

    private:

        XenonLine * mLine; // line renderer
        size_t mFrameCounter;
        size_t mDrawCounter;
        bool mSceneBegun;

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

    public:

        virtual void dumpNextFrame( size_t startBatchIndex, size_t numBatches );

    private:

        void miscClear() {}

    private:

        //@}
    };
}}

//#include "xenonContextMgr.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONGPU_H__
