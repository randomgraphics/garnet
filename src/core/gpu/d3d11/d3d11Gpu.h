#ifndef __GN_GFXD3D11_D3D11GPU_H__
#define __GN_GFXD3D11_D3D11GPU_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 GPU class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicGpu.h"

#pragma warning(disable:4100) // unreferenced formal parameters

namespace GN { namespace gfx
{
    // Forward declarations
    class D3D11Resource;
    class D3D11GpuProgram;
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
        bool init( ID3D11Device & dev, const GN::gfx::VertexBinding & vtxbind, const D3D11GpuProgram & gpuProgram );

        /// less operator
        bool operator<( const D3D11VertexLayout & rhs ) const
        {
            return il < rhs.il;
        }
    };

    ///
    /// D3D11 GPU class
    ///
    class D3D11Gpu : public BasicGpu
    {
        GN_DECLARE_STDCLASS(D3D11Gpu, BasicGpu);

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        D3D11Gpu()          { clear(); }
        virtual ~D3D11Gpu() { quit(); }
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

    public :

        virtual void * getD3DDevice() const { return mDevice; }
        virtual void * getOGLRC() const { return 0; }

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

        virtual const GpuCaps & caps() const { return mCaps; }
        virtual bool checkTextureFormatSupport( ColorFormat format, TextureUsage usage ) const;

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
        virtual GpuProgram * createGpuProgram( const void * compiledGpuProgramBinary, uint32 length );
        virtual Uniform    * createUniform( uint32 size );
        virtual Texture    * createTexture( const TextureDesc & desc );
        virtual VtxBuf     * createVtxBuf( const VtxBufDesc & desc );
        virtual IdxBuf     * createIdxBuf( const IdxBufDesc & desc );

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

        virtual bool bindContextImpl( const GpuContext & context, bool skipDirtyCheck );

        void setSampler(
            ShaderStage                shaderStage,
            int                        samplerStage,
            const D3D11_SAMPLER_DESC & ssdesc,
            bool                       skipDirtyCheck );

    private :

        bool contextInit();
        void contextQuit();
        void contextClear() { mContext.clear(); mCurrentVertexLayout = NULL; mSOMgr = 0; mRTMgr = 0; }

        inline bool bindContextRenderTarget( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextShader( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextState( const GpuContext & newContext, bool skipDirtyCheck );
        inline bool bindContextResource( const GpuContext & newContext, bool skipDirtyCheck );

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

        GN::Dictionary<VertexFormatKey,D3D11VertexLayout> mVertexLayouts;
        D3D11VertexLayout                               * mCurrentVertexLayout;
        AutoComPtr<ID3D11SamplerState>                    mDefaultSampler;
        D3D11StateObjectManager                         * mSOMgr;
        D3D11RTMgr                                      * mRTMgr;

        //@}

    // ************************************************************************
    //
    /// \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public: // from Gpu

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
        void drawClear() { mUserVB = mUserIB = 0; }

    private:

        ID3D11Buffer * mUserVB;
        ID3D11Buffer * mUserIB;

        //@}

        // ********************************************************************
        //
        /// \name Debug
        //
        // ********************************************************************

        //@{

    public:

        virtual void debugDumpNextFrame( uint32 startBatchIndex, uint32 numBatches );
        virtual void debugMarkBegin( const char * markerName );
        virtual void debugMarkEnd();
        virtual void debugMarkSet( const char * markerName );

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11GPU_H__
