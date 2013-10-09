#ifndef __GN_D3D_D3D11UTILS_H__
#define __GN_D3D_D3D11UTILS_H__
// *****************************************************************************
/// \file
/// \brief   interface of d3d11 utils
/// \author  chen@@CHENLI-HOMEPC (2007.4.16)
// *****************************************************************************

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#if GN_PLATFORM_HAS_D3D11_1
#include <d3d11_1.h>
#else
#include <d3d11.h>
#endif
#include <vector>
#include <string>

/// HRESULT error check macros
//@{
#define GN_DO_ON_HR_FAILED( func, do_something )                                                    \
    if( true ) {                                                                                    \
        HRESULT __hr = func;                                                                        \
        if (FAILED(__hr))                                                                           \
        {                                                                                           \
            GN_ERROR(::GN::getLogger("GN.d3d11utils"))("HRESULT failed: (0x%X) %S",                 \
                                              __hr, GN::d3d11::hresult2string(__hr).rawptr());      \
            do_something                                                                            \
        }                                                                                           \
    } else void(0)
#if GN_ENABLE_DEBUG
#define GN_CHECK_HR( func )         GN_DO_ON_HR_FAILED( func, )
#else
#define GN_CHECK_HR( func )         func
#endif
#define GN_RETURN_ON_HR_FAILED( func, rval ) GN_DO_ON_HR_FAILED( func, return rval; )
#define GN_RETURN_FALSE_ON_HR_FAILED( func ) GN_DO_ON_HR_FAILED( func, return false; )
#define GN_RETURN_NULL_ON_HR_FAILED( func ) GN_DO_ON_HR_FAILED( func, return nullptr; )
#define GN_RETURN_HR_ON_HR_FAILED( func )    GN_DO_ON_HR_FAILED( func, return __hr; )
#define GN_RETURN_VOID_ON_HR_FAILED( func )  GN_DO_ON_HR_FAILED( func, return; )
//@}

namespace GN { /*namespace for D3D11 utils*/ namespace d3d11
{
    /// \name misc. utils
    //@{

    enum MultiSampleAntiAlias
    {
        MSAA_DISABLE      = 0,
        MSAA_ENABLE       = 1,
    };

    GN::StrW hresult2string(HRESULT hr);

    ///
    /// construct sample descriptor based on MSAA flags
    ///
    GN_API DXGI_SAMPLE_DESC constructDXGISampleDesc( ID3D11Device & device, MultiSampleAntiAlias msaa, DXGI_FORMAT format );

    ///
    /// construct default sampler descriptor
    inline void constructDefaultSamplerDesc( D3D11_SAMPLER_DESC & desc )
    {
        memset( &desc, 0, sizeof(desc) );
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = 0.0f;
        desc.BorderColor[1] = 0.0f;
        desc.BorderColor[2] = 0.0f;
        desc.BorderColor[3] = 0.0f;
        desc.MinLOD = 0.0f;
        desc.MaxLOD = 3.402823466e+38F; // FLT_MAX
    }


    inline void setDefaultBlandStateDesc( D3D11_BLEND_DESC & desc )
    {
        desc.AlphaToCoverageEnable                 = FALSE;
        desc.IndependentBlendEnable                = FALSE;
        desc.RenderTarget[0].BlendEnable           = FALSE;
        desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlend             = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        for(size_t i = 1; i < ARRAYSIZE(desc.RenderTarget); ++i)
        {
            memcpy( &desc.RenderTarget[i], &desc.RenderTarget[0], sizeof(desc.RenderTarget[i]) );
        }
    }

    inline void setDefaultDepthStencilDesc( D3D11_DEPTH_STENCIL_DESC & desc )
    {
        desc.DepthEnable                  = TRUE;
        desc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc                    = D3D11_COMPARISON_LESS;
        desc.StencilEnable                = FALSE;
        desc.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
        desc.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
        desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
        desc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    }

    inline void setDefaultRasterizerDesc( D3D11_RASTERIZER_DESC & desc )
    {
        desc.FillMode              = D3D11_FILL_SOLID;
        desc.CullMode              = D3D11_CULL_BACK;
        desc.FrontCounterClockwise = FALSE;
        desc.DepthBias             = 0;
        desc.SlopeScaledDepthBias  = 0.0f;
        desc.DepthBiasClamp        = 0.0f;
        desc.DepthClipEnable       = TRUE;
        desc.ScissorEnable         = FALSE;
        desc.MultisampleEnable     = FALSE;
        desc.AntialiasedLineEnable = FALSE;
    }

    inline void setDefaultSamplerDesc( D3D11_SAMPLER_DESC & desc )
    {
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = 16;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
        desc.MinLOD = 0.0f;
        desc.MaxLOD = FLT_MAX;
    }

    inline ID3D11Buffer *
    createBuffer(
        ID3D11Device & dev,
        size_t bytes,
        D3D11_USAGE usage,
        UINT bind,
        UINT cpuAccess = 0,
        UINT misc = 0,
        const void * data = 0,
        size_t rowPitch = 0,
        size_t slicePitch = 0 )
    {
        D3D11_BUFFER_DESC bd = { (UINT)bytes, usage, bind, cpuAccess, misc };
        D3D11_SUBRESOURCE_DATA sd = { data, (UINT)rowPitch, (UINT)slicePitch };

        ID3D11Buffer * buf;

        if( FAILED( dev.CreateBuffer( &bd, data ? &sd : 0, &buf ) ) )
        {
            return NULL;
        }

        return buf;
    }

    //@}

    /// \name state dumper
    //@{

    GN_API ID3D11VertexShader   * createDumpableVS( ID3D11Device & device, const void * binary, size_t bytes );
    GN_API ID3D11GeometryShader * createDumpableGS( ID3D11Device & device, const void * binary, size_t bytes );
    GN_API ID3D11PixelShader    * createDumpablePS( ID3D11Device & device, const void * binary, size_t bytes );
    GN_API ID3D11InputLayout    * createDumpableIL(
        ID3D11Device                   & device,
        const D3D11_INPUT_ELEMENT_DESC * elements,
        size_t                           count,
        const void                     * signature,
        size_t                           bytes );

    GN_API void setDumpFilePrefix( const StrA & );
    GN_API void dumpDraw( ID3D11DeviceContext & devcxt, uint32 vertexCount, uint32 startVertex );
    GN_API void dumpDrawIndexed( ID3D11DeviceContext & devcxt, uint32 indexCount, uint32 startIndex, uint32 startVertex );

    //@}

    /// \name shader utils
    //@{

    enum ShaderType
    {
        SHADER_TYPE_VS = 0,
        SHADER_TYPE_PS,
        SHADER_TYPE_GS,
        SHADER_TYPE_HS,
        SHADER_TYPE_DS,
        SHADER_TYPE_CS,
        SHADER_TYPE_COUNT,
    };

    ID3DBlob * compileShader(
        const char   * profile,
        const char   * code,
        size_t         len = 0,
        UINT           flags = 0,
        const char   * entry = "main" );

    ID3D11DeviceChild * createShader(
        ID3D11Device & dev,
        ShaderType     type,
        const char   * code,
        size_t         len = 0,
        UINT           flags = 0,
        const char   * entry = "main",
        const char   * profile = nullptr, // null means using default profile
        ID3DBlob    ** signature = 0 );

    ID3D11DeviceChild * loadShaderFromFile(
        ID3D11Device  & dev,
        ShaderType      type,
        const wchar_t * fileName,
        UINT            flags = 0,
        const char    * entry = "main",
        const char    * profile = nullptr, // null means using default profile
        ID3DBlob     ** signature = 0 );

    inline ID3D11VertexShader * createVS(
        ID3D11Device & dev,
        const char   * code,
        size_t         len = 0,
        UINT           flags = 0,
        const char   * entry = "main",
        const char   * profile = nullptr,
        ID3DBlob    ** signature = 0 )
    {
        return (ID3D11VertexShader*)createShader(dev, SHADER_TYPE_VS, code, len, flags, entry, profile, signature);
    }

    inline ID3D11GeometryShader * createGS(
        ID3D11Device & dev,
        const char   * code,
        size_t         len = 0,
        UINT           flags = 0,
        const char   * entry = "main",
        const char   * profile = nullptr,
        ID3DBlob    ** signature = 0 )
    {
        return (ID3D11GeometryShader*)createShader(dev, SHADER_TYPE_GS, code, len, flags, entry, profile, signature);
    }

    inline ID3D11PixelShader * createPS(
        ID3D11Device & dev,
        const char   * code,
        size_t         len = 0,
        UINT           flags = 0,
        const char   * entry = "main",
        const char   * profile = nullptr,
        ID3DBlob    ** signature = 0 )
    {
        return (ID3D11PixelShader*)createShader(dev, SHADER_TYPE_PS, code, len, flags, entry, profile, signature);
    }

    inline ID3D11VertexShader * LoadVSFromFile(
        ID3D11Device  & dev,
        const wchar_t * fileName,
        const char    * entry,
        const char    * profile = nullptr,
        ID3DBlob     ** signature = 0 )
    {
        return (ID3D11VertexShader*)loadShaderFromFile(dev, SHADER_TYPE_VS, fileName, 0, entry, profile, signature);
    }

    inline ID3D11PixelShader * LoadPSFromFile(
        ID3D11Device  & dev,
        const wchar_t * fileName,
        const char    * entry,
        const char    * profile = nullptr,
        ID3DBlob     ** signature = 0 )
    {
        return (ID3D11PixelShader*)loadShaderFromFile(dev, SHADER_TYPE_PS, fileName, 0, entry, profile, signature);
    }
    //@}

    /*

    /// D3D11 resource pool
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
        bool init( ID3D11Device * pDevice );
        void quit();
    private:
        void clear() { m_device = NULL; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// create buffer resource
        ID3D11Buffer * createBuffer( const D3D11_BUFFER_DESC & desc, const D3D11_SUBRESOURCE_DATA * data = NULL );

        /// create buffer resource
        ID3D11Texture2D * createTexture2D( const D3D11_TEXTURE2D_DESC & desc, const D3D11_SUBRESOURCE_DATA * data = NULL );

        /// return resource back to pool
        void returnResource( ID3D11Resource * resource );

        // ********************************
        // private types
        // ********************************
    private:

        /// D3D11 pooled resource description
        struct PooledResourceDesc
        {
            D3D11_RESOURCE_DIMENSION dim;
            union
            {
                D3D11_BUFFER_DESC    buf;
                D3D11_TEXTURE1D_DESC tex1d;
                D3D11_TEXTURE2D_DESC tex2d;
                D3D11_TEXTURE3D_DESC tex3d;
            };

            /// default ctor. Does nothing
            PooledResourceDesc() {}

            /// Construct from D3D11_BUFFER_DESC
            PooledResourceDesc( const D3D11_BUFFER_DESC & desc )
            {
                ::memset( this, 0, sizeof(*this) );
                dim = D3D11_RESOURCE_DIMENSION_BUFFER;
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

        typedef Dictionary<PooledResourceDesc,ID3D11Resource*> ResourceMap;

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D11Device * m_device;

        ResourceMap m_resources;

        // ********************************
        // private functions
        // ********************************
    private:

        /// find or create new resource from pool
        ID3D11Resource * findOrCreateResource( const PooledResourceDesc & desc, const D3D11_SUBRESOURCE_DATA * data );

        /// create new D3D11 resource instance
        ID3D11Resource * createResource( const PooledResourceDesc & desc, const D3D11_SUBRESOURCE_DATA * data );
    };
    */

///
    /// simple D3D mesh
    ///
    class SimpleMesh
    {
    public:

        /// mesh vertex
        struct Vertex
        {
            //@{
            float pos[3];
            float normal[3];
            float tex[2];
            float color[4];
            float user[4];   ///< user defined data
            //@}
        };

    public:

        SimpleMesh()          { clear(); }

        virtual ~SimpleMesh() { quit(); }

        bool init( ID3D11DeviceContext & cxt );

        void quit();

        ID3D11Device & device() const { GN_ASSERT(mDevice); return *mDevice; }
        ID3D11DeviceContext & context() const { GN_ASSERT(mContext); return *mContext; }

        void beginVertices( size_t reservedVertexCount = 0 );
        void    pos( float x, float y, float z );
        void    normal( float x, float y, float z );
        void    tex( float x, float y );
        void    color( float r, float g, float b, float a );
        void    user(float x, float y, float z, float w);
        void endVertices();
        void setVertices( const Vertex * vertices, size_t count );

        void beginTopology( D3D11_PRIMITIVE_TOPOLOGY topology, size_t reservedIndexCount = 0 );
        void beginTriangle(size_t reservedIndexCount) { return beginTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, reservedIndexCount = 0); }
        void    triangle( size_t i0, size_t i1, size_t i2 );
        void    line( size_t i0, size_t i1 );
        void endTopology();
        void setTriangles( const USHORT * triangles, size_t triangleCount );

        void draw() const;
        void drawIndexed() const;

    private:

        void clear()
        {
            mDevice = 0;
            mContext = 0;
            mLayout = 0;
            mVtxBuf = 0;
            mIdxBuf = 0;
            mVtxBufCapacity = 0;
            mNumVertices = 0;
            mIdxBufCapacity = 0;
            mNumIndices = 0;
        }

        ID3D11Device           * mDevice;
        ID3D11DeviceContext    * mContext;
        ID3D11InputLayout      * mLayout;
        ID3D11Buffer           * mVtxBuf;
        ID3D11Buffer           * mIdxBuf;

        std::vector<Vertex>      mVertices;
        Vertex                   mNewVertex;
        size_t                   mVtxBufCapacity;
        size_t                   mNumVertices;

        std::vector<USHORT>      mIndices;
        size_t                   mIdxBufCapacity;
        size_t                   mNumIndices;
        D3D11_PRIMITIVE_TOPOLOGY mTopology;
    };

    struct SimpleTriangle
    {
        SimpleMesh                     mesh;
        AutoComPtr<ID3D11VertexShader> vs;
        AutoComPtr<ID3D11PixelShader>  ps;

        bool create(
            ID3D11DeviceContext & cxt,
            const char * vscode = NULL,
            const char * pscode = NULL )
        {
            // create shaders
            static const char * default_vscode =
                "struct VSI { float4 pos : POSITION0; }; \n"
                "struct VSO { float4 pos : SV_POSITION; }; \n"
                "VSO main( VSI i ) { \n"
                "   VSO o; \n"
                "   o.pos = i.pos; \n"
                "   return o; \n"
                "}";

            static const char * default_pscode =
                "struct VSO { float4 pos : SV_POSITION; }; \n"
                "float4 main( VSO i ) : SV_TARGET { \n"
                "   return 1; \n"
                "}";

            vscode = vscode ? vscode : default_vscode;
            pscode = pscode ? pscode : default_pscode;

            AutoComPtr<ID3D11Device> dev;
            cxt.GetDevice( &dev );

            vs.attach(createVS(*dev, vscode));
            ps.attach(createPS(*dev, pscode));
            if( !vs || !ps ) return false;

            // create mesh
            if( !mesh.init( cxt ) ) return false;
            mesh.beginVertices( 3 );
            mesh.pos( -0.5f, -0.5f, 0.0f );
            mesh.pos(  0.0f,  0.5f, 0.0f );
            mesh.pos(  0.5f, -0.5f, 0.0f );
            mesh.endVertices();

            // success
            return true;
        };

        void cleanup()
        {
            vs.clear();
            ps.clear();
            mesh.quit();
        }

        void draw()
        {
            ID3D11DeviceContext & cxt = mesh.context();
            cxt.VSSetShader( vs, NULL, 0 );
            cxt.GSSetShader( 0, NULL, 0 );
            cxt.PSSetShader( ps, NULL, 0 );
            mesh.draw();
        }
    };

    ///
    /// A helper class to create D3D11 resources and views.
    ///
    struct D3D11Resource
    {
        AutoComPtr<ID3D11Resource>             resource;
        AutoComPtr<ID3D11Buffer>               buffer;
        AutoComPtr<ID3D11Texture1D>            tex1d;
        AutoComPtr<ID3D11Texture2D>            tex2d;
        AutoComPtr<ID3D11Texture3D>            tex3d;
        AutoComPtr<ID3D11ShaderResourceView>   srv1;
        AutoComPtr<ID3D11ShaderResourceView>   srv2;
        AutoComPtr<ID3D11RenderTargetView>     rtv;
        AutoComPtr<ID3D11DepthStencilView>     dsv;
        AutoComPtr<ID3D11UnorderedAccessView>  uav;

    public:

        struct ViewDescriptors
        {
            const D3D11_SHADER_RESOURCE_VIEW_DESC  * srv1;
            const D3D11_SHADER_RESOURCE_VIEW_DESC  * srv2;
            const D3D11_RENDER_TARGET_VIEW_DESC    * rtv;
            const D3D11_DEPTH_STENCIL_VIEW_DESC    * dsv;
            const D3D11_UNORDERED_ACCESS_VIEW_DESC * uav;
        };

        void clear(); ///< Clear everything.

        HRESULT createBuffer(
            ID3D11Device * dev,
            const D3D11_BUFFER_DESC & bufdesc,
            const D3D11_SUBRESOURCE_DATA * data = nullptr,
            const D3D11_SHADER_RESOURCE_VIEW_DESC * srvDesc = nullptr,
            const D3D11_UNORDERED_ACCESS_VIEW_DESC * uavDesc = nullptr);

        HRESULT createTexture(
            ID3D11Device * dev,
            const D3D11_TEXTURE1D_DESC & texdesc,
            const ViewDescriptors & viewDesc);

        HRESULT createTexture(
            ID3D11Device * dev,
            const D3D11_TEXTURE2D_DESC & texdesc,
            const ViewDescriptors & viewDesc);

        HRESULT createTexture(
            ID3D11Device * dev,
            const D3D11_TEXTURE3D_DESC & texdesc,
            const ViewDescriptors & viewDesc);

        HRESULT createVB(ID3D11Device * dev, UINT sizeBytes, const void * data = nullptr);
        HRESULT createIB(ID3D11Device * dev, UINT sizeBytes, const void * data = nullptr);
        HRESULT createCB(ID3D11Device * dev, UINT sizeBytes, const void * data = nullptr);
        HRESULT create1D(ID3D11Device * dev, UINT width, DXGI_FORMAT resourceFormat, DXGI_FORMAT srvFormat);
        HRESULT create2D(ID3D11Device * dev, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT srvFormat);
        HRESULT create3D(ID3D11Device * dev, UINT width, UINT height, UINT depth, DXGI_FORMAT format, DXGI_FORMAT srvFormat);
        HRESULT createRT(ID3D11Device * dev, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT rtvFormat, DXGI_FORMAT srvFormat = DXGI_FORMAT_UNKNOWN);
        HRESULT createDS(ID3D11Device * dev, UINT width, UINT height, DXGI_FORMAT resourceFormat, DXGI_FORMAT dsvFormat, DXGI_FORMAT srv1Format = DXGI_FORMAT_UNKNOWN, DXGI_FORMAT srv2Format = DXGI_FORMAT_UNKNOWN);

    private:

        HRESULT createViews(ID3D11Device * dev, const ViewDescriptors & viewDesc);
    };


    /// 2D sprite renderer.
    ///
    /// 2D sprite coordinate system:
    ///     - left-top corner is (0,0)
    ///     - X is leftward
    ///     - Y is downward
    ///     - length unit is pixel
    class SpriteRenderer
    {
        class Impl;
        Impl * _impl;

    public:

        enum SpriteDrawOptions
        {
            USE_EXTERNAL_GPU_PROGRAM      = 0x00000001, ///< Use GPU program in current renderer context,
                                                        ///< instead of sprite renderer's default shaders.
            USE_EXTERNAL_TEXTURE_FILTERS  = 0x00000002, ///< Use texture filters defined in current renderer context.
                                                        ///< By default, sprite renderer uses point filters.
            FORCE_ALPHA_BLENDING_ENABLED  = 0x00000010, ///< Always enable alpha blending, disregarding value in current renderer context.
            FORCE_ALPHA_BLENDING_DISABLED = 0x00000020, ///< Always disable alpha blending, disregarding value in current renderer context.

            FORCE_DEPTH_TEST_ENABLED      = 0x00000100, ///< Always enable depth test, disregarding value in current renderer context.
            FORCE_DEPTH_TEST_DISABLED     = 0x00000200, ///< Always disable depth test, disregarding value in current renderer context.

            FORCE_DEPTH_WRITE_ENABLED     = 0x00001000, ///< Always enable depth write, disregarding value in current renderer context.
            FORCE_DEPTH_WRITE_DISABLED    = 0x00002000, ///< Always disable depth write, disregarding value in current renderer context.

            // A commonly used set of options that are usually used to render transparent 2D picture with alpha channel.
            TRANSPARENT_2D_IMAGE          = FORCE_ALPHA_BLENDING_ENABLED | FORCE_DEPTH_TEST_DISABLED | FORCE_DEPTH_WRITE_DISABLED,
        };

        struct TexturedSprite
        {
            float x;
            float y;
            float w;
            float h;
            float u;
            float v;
            float tw;
            float th;
            float z;
        };

        struct SolidSprite
        {
            UINT   color; // color in R-G-B-A (byte order)
            float  x;
            float  y;
            float  w;
            float  h;
            float  z;
        };

        SpriteRenderer() : _impl(nullptr) {}
        ~SpriteRenderer() { shutdown(); }
        bool initialize(ID3D11Device & dev);
        void shutdown();
        void drawBegin(ID3D11DeviceContext & cxt, ID3D11ShaderResourceView * texture, UINT options);
        void drawEnd();
        void drawTexturedSprite(const TexturedSprite &);
        void drawSolidSprite(const SolidSprite &);
        void drawSingleTexturedSprite(
            ID3D11DeviceContext & cxt,
            ID3D11ShaderResourceView * tex,
            UINT      options,
            float     x,
            float     y,
            float     w,
            float     h,
            float     u  = 0.0f,
            float     v  = 0.0f,
            float     tw = 1.0f,
            float     th = 1.0f,
            float     z  = 0.0f )
        {
            drawBegin(cxt, tex, options);
            TexturedSprite ts;
            ts.x = x;
            ts.y = y;
            ts.w = w;
            ts.h = h;
            ts.u = u;
            ts.v = v;
            ts.tw = tw;
            ts.th = th;
            ts.z = z;
            drawTexturedSprite(ts);
            drawEnd();
        }
        void DrawSingleSolidSprite(
            ID3D11DeviceContext & cxt,
            UINT      rgba, // color in R-G-B-A format
            UINT      options,
            float     x,
            float     y,
            float     w,
            float     h,
            float     z )
        {
            drawBegin(cxt, 0, options);
            SolidSprite ss;
            ss.color = rgba;
            ss.x = x;
            ss.y = y;
            ss.w = w;
            ss.h = h;
            ss.z = z;
            drawSolidSprite(ss);
            drawEnd();
        }
    };

    class TextRenderer
    {
        class Impl;

        Impl * _impl;

    public:

        struct FontDesc
        {
            const wchar_t * face;
            float           width;  // font width in pixels.
            float           height; // font height in pixels.
        };

        struct TextDesc
        {
            const wchar_t * text;
            UINT            len;   // could be 0 for zero-terminated string.
            float           x;     // X coordinate of left bottom corner of the first character, in screen space [0, resolution_x) rightward in pixels.
            float           y;     // Y coordinate of left bottom corner of the first character, in screen space [0, resolution_y) downward in pixels.
            float           z;     // Z coordinate of the text, in clip space [0,1] outward.
            DWORD           color; // text color, R-G-B-A in byte order, ABGR in DWORD order.
        };

        TextRenderer();
        ~TextRenderer();
        bool initialize(ID3D11Device & dev, FontDesc & font);
        void shutdown();
        void draw2DText(ID3D11DeviceContext & cxt, const TextDesc & text) const;
        void draw2DText(ID3D11DeviceContext & cxt, const wchar_t * text, float x, float y, DWORD color) const
        {
            TextDesc td = {0};
            td.text = text;
            td.len = 0;
            td.x = x;
            td.y = y;
            td.z = 0;
            td.color = color;
            draw2DText(cxt, td);
        }
    };

    /*
    /// structure that represents a render-target-texture
    struct RenderTargetTexture
    {
        //@{
        ID3D11Resource           * res;
        ID3D11RenderTargetView   * rtv;
        ID3D11DepthStencilView   * dsv;
        ID3D11ShaderResourceView * srv;
        ID3D11ShaderResourceView * srv2;
        //@}

        //@{

        RenderTargetTexture() : res(0), rtv(0), dsv(0), srv(0), srv2(0)
        {
        }

        void Cleanup()
        {
            SAFE_RELEASE( res );
            SAFE_RELEASE( rtv );
            SAFE_RELEASE( dsv );
            SAFE_RELEASE( srv );
            SAFE_RELEASE( srv2 );
        }

        bool create2D(
            ID3D11Device & dev,
            UINT width,
            UINT height,
            DXGI_FORMAT texfmt,
            DXGI_FORMAT rtvfmt,
            DXGI_FORMAT srvfmt,
            UINT samples = 1,
            UINT quality = 0 );

        bool create2D(
            ID3D11Device & dev,
            UINT width,
            UINT height,
            DXGI_FORMAT format,
            UINT samples = 1,
            UINT quality = 0 )
        {
            return create2D( dev, width, height, format, format, format, samples, quality );
        }

        bool CreateDepth(
            ID3D11Device & dev,
            UINT width,
            UINT height,
            DXGI_FORMAT texfmt,
            DXGI_FORMAT dsvfmt,
            DXGI_FORMAT srv1fmt,
            DXGI_FORMAT srv2fmt,
            UINT samples = 1,
            UINT quality = 0 );

        bool CreateDepth(
            ID3D11Device & dev,
            UINT width,
            UINT height,
            UINT samples = 1,
            UINT quality = 0 )
        {
            return CreateDepth(
                dev,
                width,
                height,
                DXGI_FORMAT_R32G8X24_TYPELESS,
                DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
                DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
                DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
                samples,
                quality );
        }

        //@}
    };

    ///
    /// render-to-texture options
    ///
    struct RenderToTextureOption
    {
        //@{
        UINT                 width;
        UINT                 height;
        DXGI_FORMAT          texfmt;
        DXGI_FORMAT          rtvfmt;
        DXGI_FORMAT          srvfmt;
        UINT                 count;
        UINT                 multiSampleCount;
        bool                 floatDepth;
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
        bool init( ID3D11DeviceContext & context, const RenderToTextureOption & options );
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

        void bindNoDepth() const { mContext->OMSetRenderTargets( (UINT)mColors.size(), mColorViews.cptr(), 0 ); }
        void bindWithDepth() const { mContext->OMSetRenderTargets( (UINT)mColors.size(), mColorViews.cptr(), mDepth.dsv ); }
        void bindDepthOnly() const
        {
            ID3D11RenderTargetView * rtv[8];
            memset( rtv, 0, sizeof(rtv) );
            mContext->OMSetRenderTargets( 8, rtv, mDepth.dsv );
        }

        void clearScreen( float r, float g, float b, float a, float d, UInt8 s );

        // ********************************
        // private variables
        // ********************************
    private:

        ID3D11Device                        * mDevice;
        ID3D11DeviceContext                 * mContext;
        StackArray<RenderTargetTexture,8>     mColors;
        FixedArray<ID3D11RenderTargetView*,8> mColorViews;
        RenderTargetTexture                   mDepth;

        // ********************************
        // private functions
        // ********************************
    private:
    };*/

    ///
    /// D3D11 application framework
    ///
    struct D3D11AppOption
    {
        /// Construct with default options.
        D3D11AppOption()
            : ref(false)
            , debug(!!GN_ENABLE_DEBUG)
            , vsync(false)
            , fullscreen(false)
            , width(1280)
            , height(720)
            , msaa(0)
        {
        }

        /// use reference device
        bool ref;

        /// use debug device
        bool debug;

        /// V-SYNC enable or not. Default is false.
        bool vsync;

        /// fullscreen or windowed mode.
        /// Default is false.
        bool fullscreen;

        /// Screen width. Ignored for fullscreen mode.
        UINT width;

        /// Screen height. Ignored for fullscreen mode.
        UINT height;

        /// MSAA:
        ///     0 - disabled.
        ///     1 - enabled, low quality
        ///     2 - enabled, high quality
        int msaa;
    };

    ///
    /// D3D11 application framework
    ///
    class D3D11Application
    {
    public:

        //@{

        D3D11Application();
        ~D3D11Application();

        ID3D11Device           & device() const { GN_ASSERT( mDevice ); return *mDevice; }
        ID3D11DeviceContext    & context() const { GN_ASSERT( mContext ); return *mContext; }
#if GN_PLATFORM_HAS_D3D11_1
        ID3D11Device1          & device1() const { GN_ASSERT( mDevice1 ); return *mDevice1; }
        ID3D11DeviceContext1   & context1() const { GN_ASSERT( mContext1 ); return *mContext1; }
#endif
        IDXGISwapChain         & swapChain() const { GN_ASSERT( mSwapChain ); return *mSwapChain; }

        ID3D11Texture2D        * backBuffer() const { return mBackBuf; }
        ID3D11RenderTargetView * backRTV() const { return mBackRTV; }
        ID3D11DepthStencilView * depthRTV() const { return mDepthDSV; }

        int run( const D3D11AppOption * = nullptr );

        bool changeOption( const D3D11AppOption & );

        const D3D11AppOption & getOption() const { return mOption; }

        void clearScreen( float r, float g, float b, float a, float d, BYTE s );

        void resetToDefaultRenderTargets() { mContext->OMSetRenderTargets( 1, &mBackRTV, mDepthDSV ); }

        //@}

    protected:

        //@{

        virtual bool onInit( D3D11AppOption & ) { return true; }
        virtual void onCleanup() {}
        virtual bool onCreateDevice() { return true; }
        virtual void onDestroyDevice() {}
        virtual void onDraw() {}
        virtual void onUpdate() {}

        //@}

    private:

        bool init();
        void cleanup();
        bool createDevice();
        void destroyDevice();

    private:

        class RenderWindow;

        D3D11AppOption           mOption;
        RenderWindow           * mWindow;
        IDXGIAdapter           * mAdapter;
        ID3D11Device           * mDevice;
        ID3D11DeviceContext    * mContext;
        IDXGISwapChain         * mSwapChain;
#if GN_PLATFORM_HAS_D3D11_1
        ID3D11Device1          * mDevice1;
        ID3D11DeviceContext1   * mContext1;
        IDXGISwapChain1        * mSwapChain1;
#endif
        ID3D11Texture2D        * mBackBuf;  // default back buffer
        ID3D11RenderTargetView * mBackRTV;
        ID3D11Texture2D        * mDepthBuf; // default depth buffer
        ID3D11DepthStencilView * mDepthDSV;
    };

    //@}
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D_D3D11UTILS_H__
