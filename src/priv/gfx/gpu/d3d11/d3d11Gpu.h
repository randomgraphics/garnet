#ifndef __GN_GFXD3D11_D3D11GPU_H__
#define __GN_GFXD3D11_D3D11GPU_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 GPU class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicGpuMsw.h"

#pragma warning(disable:4100) // unreferenced formal parameters

namespace GN { namespace gfx
{
    // Forward declarations
    class D3D11Resource;
    class D3D11RTMgr;
    class D3D11StateObjectManager;

    ///
    /// D3D11 vertex format structure
    ///
    class D3D11VertexLayout
    {
    public:

        AutoComPtr<ID3D11InputLayout> il; ///< D3D11 input layout

    public:

        /// initialize the layout
        bool Init( ID3D11Device & dev, const GN::gfx::VertexFormat & format );

        /// less operator
        bool operator<( const D3D11VertexLayout & rhs ) const
        {
            return il < rhs.il;
        }
    };

    ///
    /// D3D11 GPU class
    ///
    class D3D11Gpu : public BasicGpuMsw
    {
        GN_DECLARE_STDCLASS(D3D11Gpu, BasicGpuMsw);

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        D3D11Gpu()          { Clear(); }
        virtual ~D3D11Gpu() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const GpuOptions & );
        void Quit();
    private:
        void Clear()
        {
            dispClear();
            capsClear();
            resourceClear();
            ContextClear();
            drawClear();
        }
        //@}

    // ************************************************************************
    //
    /// \name                     Display Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual void * GetD3DDevice() const { return mDevice; }
        virtual void * GetOGLRC() const { return 0; }

    public :

        ID3D11Device        & getDeviceRefInlined() const { GN_ASSERT(mDevice); return *mDevice; }
        ID3D11DeviceContext & getDeviceContextRefInlined() const { GN_ASSERT(mDeviceContext); return *mDeviceContext; }
        IDXGISwapChain      & getSwapChainRef() const { GN_ASSERT(mSwapChain); return *mSwapChain; }

        void                  ReportLiveDeviceObjects();

    private :

        bool dispInit();
        void dispQuit();
        void dispClear()
        {
            mAdapter = 0;
            mSwapChain = 0;
            mDevice = 0;
            mDeviceContext = 0;
            mD3D11Debug = 0;
        }

    private :

        IDXGIAdapter        * mAdapter;
        IDXGISwapChain      * mSwapChain;
        ID3D11Device        * mDevice;
        ID3D11DeviceContext * mDeviceContext;
        ID3D11Debug         * mD3D11Debug;

        //@}

    // ************************************************************************
    //
    /// \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual const GpuCaps & GetCaps() const { return mCaps; }
        virtual bool CheckTextureFormatSupport( ColorFormat format, TextureUsage usage ) const;
        virtual ColorFormat GetDefaultTextureFormat( TextureUsage usage ) const;

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
        /// constructor of D3D9Resource.
        ///
        void insertResource( D3D11Resource * p )
        {
            mResourceList.push_back(p);
        }

        ///
        /// Remove resource from resource list. Can be only called by
        /// destructor of D3D9Resource.
        ///
        void removeResource( D3D11Resource * p )
        {
            mResourceList.remove(p);
        }

    private:

        bool resourceInit();
        void resourceQuit();
        void resourceClear() {}

    private :

        std::list<D3D11Resource*> mResourceList;

        //@}


    // ********************************************************************
    //
    /// \name Context manager
    //
    // ********************************************************************

        //@{

    public:

        virtual bool BindContextImpl( const GpuContext & context, bool skipDirtyCheck );

        void setSampler(
            int                        shaderStage,  // 0: VS, 1: GS: 2: PS
            int                        samplerStage, // sampler stage
            const D3D11_SAMPLER_DESC & ssdesc,
            bool                       skipDirtyCheck );

    private :

        bool contextInit();
        void contextQuit();
        void ContextClear() { mContext.Clear(); mSOMgr = 0; mRTMgr = 0; }

        inline bool bindContextRenderTarget( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextShader( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextState( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextResource( const GpuContext & newContext, bool skipDirtyCheck );

    private:

        GN::Dictionary<VertexFormat,D3D11VertexLayout> mVertexLayouts;
        AutoComPtr<ID3D11SamplerState>           mDefaultSampler;
        D3D11StateObjectManager * mSOMgr;
        D3D11RTMgr              * mRTMgr;

        //@}

    // ************************************************************************
    //
    /// \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public: // from Gpu

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

    private:

        bool drawInit();
        void drawQuit();
        void drawClear() { mFrameCounter = 0; mDrawCounter = 0; mUserVB = mUserIB = 0; }

    private:

        size_t mFrameCounter;
        size_t mDrawCounter;

        ID3D11Buffer * mUserVB;
        ID3D11Buffer * mUserIB;

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

    public:

        virtual void DumpNextFrame( size_t startBatchIndex, size_t numBatches )
        {
            GN_UNUSED_PARAM( startBatchIndex );
            GN_UNUSED_PARAM( numBatches );
            GN_TODO( "D3D11 frame dump is not implemented." );
        }

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11GPU_H__
