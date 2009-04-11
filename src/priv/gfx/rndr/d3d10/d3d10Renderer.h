#ifndef __GN_GFXD3D10_D3D10RENDERER_H__
#define __GN_GFXD3D10_D3D10RENDERER_H__
// *****************************************************************************
/// \file
/// \brief   D3D10 renderer class
/// \author  chenlee (2005.10.2)
// *****************************************************************************

#include "../common/basicRendererMsw.h"

#pragma warning(disable:4100)

namespace GN { namespace gfx
{
    // Forward declarations
    class D3D10Resource;
    class D3D10RTMgr;
    class D3D10StateObjectManager;

    ///
    /// D3D9 vertex buffer layout descriptor
    ///
    struct D3D10VtxLayoutDesc
    {
        VertexFormat                  format; ///< vertex format
        AutoComPtr<ID3D10InputLayout> layout; ///< D3D layout
    };

    ///
    /// D3D10 renderer class
    ///
    class D3D10Renderer : public BasicRendererMsw
    {
        GN_DECLARE_STDCLASS(D3D10Renderer, BasicRendererMsw);

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public :
        D3D10Renderer()          { clear(); }
        virtual ~D3D10Renderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const RendererOptions & );
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

        ID3D10Device   & getDeviceRefInlined() const { GN_ASSERT(mDevice); return *mDevice; }
        IDXGISwapChain & getSwapChainRef() const { GN_ASSERT(mSwapChain); return *mSwapChain; }

    private :

        bool dispInit();
        void dispQuit();
        void dispClear()
        {
            mAdapter = 0;
            mDevice = 0;
            mSwapChain = 0;
        }

    private :

        IDXGIAdapter   * mAdapter;
        ID3D10Device   * mDevice;
        IDXGISwapChain * mSwapChain;

        //@}

    // ************************************************************************
    //
    /// \name                 Capability Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual const RendererCaps & getCaps() const { return mCaps; }
        virtual bool checkTextureFormatSupport( ColorFormat format, TextureUsages usages ) const;
        virtual ColorFormat getDefaultTextureFormat( TextureUsages usages ) const;

    private :

        bool capsInit();
        void capsQuit() {}
        void capsClear() {}

    private :

        RendererCaps mCaps;

        //@}

    // ************************************************************************
    //
    /// \name                     Resource Manager
    //
    // ************************************************************************

        //@{

    public :

        virtual CompiledGpuProgram * compileGpuProgram( const GpuProgramDesc & desc );
        virtual GpuProgram         * createGpuProgram( const void * compiledGpuProgramBinary, size_t length );
        virtual Uniform            * createUniform( size_t size );
        virtual Texture            * createTexture( const TextureDesc & desc );
        virtual VtxBuf             * createVtxBuf( const VtxBufDesc & desc );
        virtual IdxBuf             * createIdxBuf( const IdxBufDesc & desc );

    public :

        ///
        /// Insert resource into resource list. Can be only called by
        /// constructor of D3D9Resource.
        ///
        void insertResource( D3D10Resource * p )
        {
            mResourceList.push_back(p);
        }

        ///
        /// Remove resource from resource list. Can be only called by
        /// destructor of D3D9Resource.
        ///
        void removeResource( D3D10Resource * p )
        {
            mResourceList.remove(p);
        }

    private:

        bool resourceInit();
        void resourceQuit();
        void resourceClear() {}

    private :

        std::list<D3D10Resource*> mResourceList;

        //@}


    // ********************************************************************
    //
    /// \name Context manager
    //
    // ********************************************************************

        //@{

    public:

        virtual bool bindContextImpl( const RendererContext & context, bool skipDirtyCheck );

    private :

        bool contextInit();
        void contextQuit();
        void contextClear() { mContext.clear(); mSOMgr = 0; mRTMgr = 0; }

        inline bool bindContextRenderTarget( const RendererContext & newContext, bool skipDirtyCheck );
        inline bool bindContextShader( const RendererContext & newContext, bool skipDirtyCheck );
        inline bool bindContextState( const RendererContext & newContext, bool skipDirtyCheck );
        inline bool bindContextResource( const RendererContext & newContext, bool skipDirtyCheck );

    private:

        struct VertexFormatKey
        {
            VertexFormat vtxfmt;
            UInt64       shaderID;

            bool operator<( const VertexFormatKey & rhs ) const
            {
                if( shaderID < rhs.shaderID ) return true;
                if( shaderID > rhs.shaderID ) return false;
                return vtxfmt < rhs.vtxfmt;
            }
        };

        RendererContext           mContext;
        D3D10StateObjectManager * mSOMgr;
        D3D10RTMgr              * mRTMgr;

        //@}

    // ************************************************************************
    //
    /// \name                     Drawing Manager
    //
    // ************************************************************************

        //@{

    public: // from Renderer

        virtual void present();
        virtual void clearScreen( const Vector4f & c, float z, UInt8 s, BitFields flags );
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
                             const UInt16 * indexData );
        virtual void drawUp( PrimitiveType prim,
                             size_t        numvtx,
                             const void *  vertexData,
                             size_t        strideInBytes );
        virtual void drawLines( BitFields         options,
                                const void *      positions,
                                size_t            stride,
                                size_t            numpoints,
                                UInt32            rgba,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj );

    private:

        bool drawInit() { return true; }
        void drawQuit() {}
        void drawClear() { mFrameCounter = 0; mDrawCounter = 0; }

    private:

        size_t mFrameCounter;
        size_t mDrawCounter;

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
        //
        // ********************************************************************

        //@{

    public:

        virtual void dumpNextFrame( size_t startBatchIndex, size_t numBatches )
        {
            GN_UNUSED_PARAM( startBatchIndex );
            GN_UNUSED_PARAM( numBatches );
            GN_TODO( "D3D10 frame dump is not implemented." );
        }

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10RENDERER_H__
