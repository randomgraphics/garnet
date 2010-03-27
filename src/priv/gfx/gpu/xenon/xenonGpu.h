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
    /// Xenon vertex buffer declaration descriptor
    ///
    struct XenonVtxDeclDesc
    {
        VertexFormat                            format; ///< vertex format description
        AutoComPtr<IDirect3DVertexDeclaration9> decl;   ///< D3D vertex decl
    };

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
        XenonGpu() : mThreadId(GetCurrentThreadIdentifier()) { Clear(); }
        virtual ~XenonGpu() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const GpuOptions & ro );
        void Quit();
    private:
        void Clear()
        {
            dispClear();
            capsClear();
            resourceClear();
            ContextClear();
            drawClear();
            miscClear();
        }
        //@}

    // ************************************************************************
    //
    /// \name                     Display Manager
    //
    // ************************************************************************

        //@{

    public :

        // from Gpu
        virtual void * GetD3DDevice() const { return mDevice; }
        virtual void * GetOGLRC() const { return 0; }

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

        virtual const GpuCaps & GetCaps() const { return mCaps; }
        virtual bool                 CheckTextureFormatSupport( ColorFormat format, TextureUsage usages ) const;
        virtual ColorFormat          GetDefaultTextureFormat( TextureUsage usages ) const;

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

        virtual Blob       * CompileGpuProgram( const GpuProgramDesc & desc );
        virtual GpuProgram * CreateGpuProgram( const void * compiledGpuProgramBinary, size_t length );
        virtual Uniform    * CreateUniform( size_t size );
        virtual Texture    * CreateTexture( const TextureDesc & desc );
        virtual VtxBuf     * CreateVtxBuf( const VtxBufDesc & desc );
        virtual IdxBuf     * CreateIdxBuf( const IdxBufDesc & desc );

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

        virtual bool BindContextImpl( const GpuContext & context, bool skipDirtyCheck );

    public:

        void setD3DRenderState( D3DRENDERSTATETYPE, DWORD );
        void setD3DSamplerState( UINT, D3DSAMPLERSTATETYPE, DWORD );

    private :

        bool contextInit();
        void contextQuit();
        void ContextClear() { mContext.Clear(); mRTMgr = 0; }

        inline bool bindContextRenderTargetsAndViewport( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextRenderStates( const GpuContext & newContext, bool skipDirtyCheck  );
        inline bool bindContextShaders( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextResources( const GpuContext & newContext, bool skipDirtyCheck );

    private:

        XenonRenderTargetManager                                      * mRTMgr;
        GN::Dictionary<VertexFormat,AutoComPtr<IDirect3DVertexDeclaration9> > mVertexFormats;

        //@}

    // ************************************************************************
    //
    /// \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public:

        virtual void Present();
        virtual void ClearScreen( const Vector4f & c, float z, UInt8 s, BitFields flags );
        virtual void DrawIndexed( PrimitiveType prim,
                                  size_t        numidx,
                                  size_t        basevtx,
                                  size_t        startvtx,
                                  size_t        numvtx,
                                  size_t        startidx );
        virtual void Draw( PrimitiveType prim,
                           size_t        numvtx,
                           size_t        startvtx );
        virtual void DrawIndexedUp(
                             PrimitiveType  prim,
                             size_t         numidx,
                             size_t         numvtx,
                             const void *   vertexData,
                             size_t         strideInBytes,
                             const UInt16 * indexData );
        virtual void DrawUp( PrimitiveType prim,
                             size_t        numvtx,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void DrawLines( BitFields         options,
                                const void *      positions,
                                size_t            stride,
                                size_t            numpoints,
                                UInt32            rgba,
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

        virtual void DumpNextFrame( size_t startBatchIndex, size_t numBatches );

    public:

        SInt32 getThreadId() const { return mThreadId; }

    private:

        void miscClear() {}

    private:

        const SInt32 mThreadId; // initialized by constructor

        //@}
    };
}}

//#include "xenonContextMgr.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONGPU_H__
