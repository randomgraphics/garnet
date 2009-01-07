#ifndef __GN_D3D10_H__
#define __GN_D3D10_H__
// *****************************************************************************
/// \file
/// \brief   interface of d3d10 utils
/// \author  chen@@CHENLI-HOMEPC (2007.4.16)
// *****************************************************************************

#include "garnet/GNbase.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <d3d9.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <map>

namespace GN { /*namespace for D3D10 utils*/ namespace d3d10
{
    enum MultiSampleAntiAlias
    {
        MSAA_DISABLE      = 0,
        MSAA_ENABLE       = 1,
    };

    ///
    /// construct sample descriptor based on MSAA flags
    ///
    DXGI_SAMPLE_DESC constructSampleDesc( ID3D10Device * device, GN::d3d10::MultiSampleAntiAlias msaa, DXGI_FORMAT format );

    ///
    /// scoped PIX event
    ///
    struct PixPerfScopeEvent
    {
        //@{
        PixPerfScopeEvent( D3DCOLOR color, const wchar_t * name )
        {
            D3DPERF_BeginEvent( color, name );
        }
        ~PixPerfScopeEvent()
        {
            D3DPERF_EndEvent();
        }
        //@}
    };

    /// \name state dumper
    //@{

    ID3D10VertexShader   * createDumpableVS( ID3D10Device & device, const void * binary, size_t bytes );
    ID3D10GeometryShader * createDumpableGS( ID3D10Device & device, const void * binary, size_t bytes );
    ID3D10PixelShader    * createDumpablePS( ID3D10Device & device, const void * binary, size_t bytes );
    ID3D10InputLayout    * createDumpableIL(
        ID3D10Device                   & device,
        const D3D10_INPUT_ELEMENT_DESC * elements,
        size_t                           count,
        const void                     * signature,
        size_t                           bytes );

    void setDumpFilePrefix( const StrA & );
    void dumpDraw( ID3D10Device & device, UInt32 vertexCount, UInt32 startVertex );
    void dumpDrawIndexed( ID3D10Device & device, UInt32 indexCount, UInt32 startIndex, UInt32 startVertex );

    //@}

    /// \name shader utils
    //@{

    ID3D10VertexShader * compileVS(
        ID3D10Device & dev,
        const char   * code,
        size_t         len = 0,
        UInt32         flags = 0,
        const char   * entry = "main",
        const char   * profile = "vs_4_0",
        ID3D10Blob  ** signature = 0 );

    ID3D10PixelShader * compilePS(
        ID3D10Device & dev,
        const char   * code,
        size_t         len = 0,
        UInt32         flags = 0,
        const char   * entry = "main",
        const char   * profile = "ps_4_0",
        ID3D10Blob  ** signature = 0 );

    //@}

    /// D3D10 resource pool
    class ResourcePool : public StdClass
    {
        GN_DECLARE_STDCLASS( ResourcePool, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ResourcePool()          { clear(); }
        virtual ~ResourcePool() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( ID3D10Device * pDevice );
        void quit();
    private:
        void clear() { m_device = NULL; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// create buffer resource
        ID3D10Buffer * createBuffer( const D3D10_BUFFER_DESC & desc, const D3D10_SUBRESOURCE_DATA * data = NULL );

        /// create buffer resource
        ID3D10Texture2D * createTexture2D( const D3D10_TEXTURE2D_DESC & desc, const D3D10_SUBRESOURCE_DATA * data = NULL );

        /// return resource back to pool
        void returnResource( ID3D10Resource * resource );

        // ********************************
        // private types
        // ********************************
    private:

        /// D3D10 pooled resource description
        struct PooledResourceDesc
        {
            D3D10_RESOURCE_DIMENSION dim;
            union
            {
                D3D10_BUFFER_DESC    buf;
                D3D10_TEXTURE1D_DESC tex1d;
                D3D10_TEXTURE2D_DESC tex2d;
                D3D10_TEXTURE3D_DESC tex3d;
            };

            /// default ctor. Does nothing
            PooledResourceDesc() {}

            /// Construct from D3D10_BUFFER_DESC
            PooledResourceDesc( const D3D10_BUFFER_DESC & desc )
            {
                ::memset( this, 0, sizeof(*this) );
                dim = D3D10_RESOURCE_DIMENSION_BUFFER;
                buf = desc;
            }

            /// Less operator
            bool operator<( const PooledResourceDesc & rhs ) const
            {
                return ::memcmp( this, &rhs, sizeof(*this) ) < 0;
            }

            /// if the resource is immutable
            inline bool isImmutable() const;
        };

        typedef std::map<PooledResourceDesc,ID3D10Resource*> ResourceMap;

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10Device * m_device;

        ResourceMap m_resources;

        // ********************************
        // private functions
        // ********************************
    private:

        /// find or create new resource from pool
        ID3D10Resource * findOrCreateResource( const PooledResourceDesc & desc, const D3D10_SUBRESOURCE_DATA * data );

        /// create new D3D10 resource instance
        ID3D10Resource * createResource( const PooledResourceDesc & desc, const D3D10_SUBRESOURCE_DATA * data );
    };

    ///
    /// simple D3D mesh
    ///
    class SimpleMesh : public StdClass
    {
        GN_DECLARE_STDCLASS( SimpleMesh, StdClass );

    public:

        ///
        /// mesh vertex
        ///
        struct Vertex
        {
            //@{
            Vector3f pos;
            Vector3f normal;
            Vector2f tex;
            Vector4f color;
            Vector4f user;   ///< user defined data
            //@}
        };

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SimpleMesh()          { clear(); }
        virtual ~SimpleMesh() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( ID3D10Device * dev );
        void quit();
    private:
        void clear()
        {
            mDevice = 0;
            mLayout = 0;
            mVtxBuf = 0;
            mIdxBuf = 0;
            mVtxBufCapacity = 0;
            mNumVertices = 0;
            mIdxBufCapacity = 0;
            mNumIndices = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{

        void beginVertices();
        void    pos( float x, float y, float z );
        void    normal( float x, float y, float z );
        void    tex( float x, float y );
        void    color( float r, float g, float b, float a );
        void endVertices();
        void setVertices( const Vertex * vertices, size_t count );

        void beginTriangles();
        void    triangle( size_t i0, size_t i1, size_t i2 );
        void endTriangles();
        void setTriangles( const UInt16 * triangles, size_t triangleCount );

        void draw() const;
        void drawIndexed() const;

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10Device           * mDevice;
        ID3D10InputLayout      * mLayout;
        ID3D10Buffer           * mVtxBuf;
        ID3D10Buffer           * mIdxBuf;

        DynaArray<Vertex>        mVertices;
        Vertex                   mNewVertex;
        size_t                   mVtxBufCapacity;
        size_t                   mNumVertices;

        DynaArray<UInt16>        mIndices;
        size_t                   mIdxBufCapacity;
        size_t                   mNumIndices;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// structure to represent a render-target-texture
    ///
    struct RenderTargetTexture
    {
        //@{
        ID3D10Resource           * res;
        ID3D10RenderTargetView   * rtv;
        ID3D10DepthStencilView   * dsv;
        ID3D10ShaderResourceView * srv;
        //@}

        //@{

        RenderTargetTexture() : res(0), rtv(0), dsv(0), srv(0)
        {
        }

        void clear()
        {
            safeRelease( res );
            safeRelease( rtv );
            safeRelease( dsv );
            safeRelease( srv );
        }

        //@}
    };

    ///
    /// render-to-texture options
    ///
    struct RenderToTextureOption
    {
        //@{
        UInt32               width;
        UInt32               height;
        DXGI_FORMAT          format;
        UInt32               count;
        MultiSampleAntiAlias msaa;
        bool                 stencil;
        //@}
    };

    ///
    /// utility class to simplify render-to-texture
    ///
    class RenderToTexture : public StdClass
    {
        GN_DECLARE_STDCLASS( RenderToTexture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        RenderToTexture()          { clear(); }
        virtual ~RenderToTexture() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( ID3D10Device * device, const RenderToTextureOption & options );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        const RenderTargetTexture & getColorBuffer( size_t index ) const { GN_ASSERT( index < mColors.size() ); return mColors[index]; }
        const RenderTargetTexture & getDepthBuffer() const { return mDepth; }

        void bindNoDepth() const { mDevice->OMSetRenderTargets( (UINT)mColors.size(), mColorViews.cptr(), 0 ); }
        void bindWithDepth() const { mDevice->OMSetRenderTargets( (UINT)mColors.size(), mColorViews.cptr(), mDepth.dsv ); }

        void clearScreen( float r, float g, float b, float a, float d, UInt8 s );

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10Device                        * mDevice;
        StackArray<RenderTargetTexture,8>     mColors;
        FixedArray<ID3D10RenderTargetView*,8> mColorViews;
        RenderTargetTexture                   mDepth;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// screen aligned quad descriptor
    ///
    struct ScreenAlignedQuadDesc
    {
        float x1, y1;
        float x2, y2;
        float u1, v1;
        float u2, v2;
        float z;
        D3D10_DEPTH_WRITE_MASK depthWriteMask;
        D3D10_COMPARISON_FUNC  depthFunc;

        void makeDefault()
        {
            x1 = y1 = -1.0f;
            x2 = y2 = 1.0f;
            u1 = 0.0f;
            v1 = 1.0f;
            u2 = 1.0f;
            v2 = 0.0f;
            z = .0f;
            depthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;
            depthFunc = D3D10_COMPARISON_ALWAYS;
        }
    };

    ///
    /// sceeen aligned quad mesh
    ///
    class ScreenAlignedQuad : public StdClass
    {
        GN_DECLARE_STDCLASS( ScreenAlignedQuad, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ScreenAlignedQuad()          { clear(); }
        virtual ~ScreenAlignedQuad() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( ID3D10Device * device, const ScreenAlignedQuadDesc & desc );
        void quit();
    private:
        void clear() { mDevice = 0; mDepthStencilState = 0; mVs = 0; mPsTexed = 0; mPsSolid = 0; }
        //@}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        ///  \note Does not support multisampled texture
        ///
        void drawTexed( ID3D10ShaderResourceView * srv );

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D10Device            * mDevice;
        ID3D10DepthStencilState * mDepthStencilState;
        ID3D10VertexShader      * mVs;
        ID3D10PixelShader       * mPsTexed;
        ID3D10PixelShader       * mPsSolid;
        SimpleMesh                mMesh;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// D3D10 application framework
    ///
    struct D3D10AppOption
    {
        ///
        /// ctor
        ///
        D3D10AppOption()
            : parent(0)
            , monitor(0)
            , ref(false)
            , debug(false)
            , vsync(false)
            , fullscreen(false)
            , width(640)
            , height(480)
            , depth(0)
            , refrate(0)
            , msaa(MSAA_DISABLE)
        {
        }

        ///
        /// Handle of parent window. Default is zero, means a top-level window.
        ///
        HWND parent;

        ///
        /// Monitor handle.
        ///
        /// - 0 means using the monitor where parent window stays in.
        ///   If monitor and parent are both zero, primary monitor will be used.
        /// - Default is 0.
        ///
        HMONITOR monitor;

        ///
        /// use reference device
        ///
        bool ref;

        ///
        /// use debug device
        ///
        bool debug;

        ///
        /// V-SYNC enable or not. Default is false.
        ///
        bool vsync;

        ///
        /// fullscreen or windowed mode.
        /// Default is false.
        ///
        bool fullscreen;

        UInt32 width;   ///< Screen width.
        UInt32 height;  ///< Screen height.
        UInt32 depth;   ///< Color depth. Ignored for windowed mode.
        UInt32 refrate; ///< Referesh rate. Ignored for windowed mode.

        ///
        /// MSAA.
        ///
        MultiSampleAntiAlias msaa;
    };

    ///
    /// D3D10 application framework
    ///
    class D3D10Application
    {
    public:

        //@{

        D3D10Application();
        ~D3D10Application();

        ID3D10Device & device() const { GN_ASSERT( mDevice ); return *mDevice; }
        IDXGISwapChain & swapChain() const { GN_ASSERT( mSwapChain ); return *mSwapChain; }

        ID3D10Texture2D        * backbuf() const { return mBackBuf; }
        ID3D10RenderTargetView * backrtv() const { return mBackRTV; }
        ID3D10DepthStencilView * depthrtv() const { return mDepthDSV; }

        int run( const D3D10AppOption & );

        bool changeOption( const D3D10AppOption & );

        const D3D10AppOption & getOption() const { return mOption; }

        void clearScreen( float r, float g, float b, float a, float d, UInt8 s );

        void resetToDefaultRenderTargets() { mDevice->OMSetRenderTargets( 1, &mBackRTV, mDepthDSV ); }

        //@}

    protected:

        //@{

        virtual bool onInit( D3D10AppOption & ) { return true; }
        virtual bool onCreate() { return true; }
        virtual void onDestroy() {}
        virtual void onQuit() {}

        virtual void onDraw() {}
        virtual void onUpdate() {}

        //@}

    private:

        bool init();
        void quit();

        bool createDevice();
        void destroyDevice();

    private:

        D3D10AppOption           mOption;
        HWND                     mWindow;
        IDXGIAdapter           * mAdapter;
        ID3D10Device           * mDevice;
        IDXGISwapChain         * mSwapChain;
        ID3D10Texture2D        * mBackBuf;  // default back buffer
        ID3D10RenderTargetView * mBackRTV;
        ID3D10Texture2D        * mDepthBuf; // default depth buffer
        ID3D10DepthStencilView * mDepthDSV;
        ID3D10Debug            * mDebug;
        ID3D10InfoQueue        * mInfoQueue;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D10_H__
