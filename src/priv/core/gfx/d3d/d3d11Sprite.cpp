#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.d3d11utils.SpriteRenderer");

static const char * hlslvscode=
    "struct VSOUT { \n"
    "   float4 position  : SV_POSITION; \n"
    "   float4 color     : COLOR; \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "struct VSIN { \n"
    "   float4 position  : POSITION; \n"
    "   float4 color     : COLOR; \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "VSOUT main( VSIN i ) { \n"
    "   VSOUT o; \n"
    "   o.position.x  = i.position.x * 2.0 - 1.0; \n"
    "   o.position.y  = i.position.y * -2.0 + 1.0; \n"
    "   o.position.zw = i.position.zw; \n"
    "   o.color       = i.color; \n"
    "   o.texcoords   = i.texcoords; \n"
    "   return o; \n"
    "}";

static const char * hlslpscode=
    "Texture2D t0; \n"
    "sampler s0; \n"
    "struct VSOUT { \n"
    "   float4 position  : SV_POSITION; \n"
    "   float4 color     : COLOR; \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "float4 main( VSOUT i ) : SV_TARGET { \n"
    "   return i.color * t0.Sample(s0, i.texcoords); \n"
    "}";

// *****************************************************************************
// Impl class
// *****************************************************************************

class GN::d3d11::SpriteRenderer::Impl
{
    struct Float2
    {
        float x, y;

        void Set(float x_, float y_)
        {
            x = x_;
            y = y_;
        }
    };

    struct Float3
    {
        float x, y, z;

        void Set(float x_, float y_, float z_)
        {
            x = x_;
            y = y_;
            z = z_;
        }
    };

    struct SpriteVertex
    {
        Float3 pos;
        UINT   clr;    // color in R-G-B-A format
        Float2 tex;
        float _[2];   // padding to 32 bytes
    };
    static_assert(sizeof(SpriteVertex) == 32, "");

    struct Sprite
    {
        SpriteVertex v[4];
    };

    enum
    {
        MAX_SPRITES  = 256,
    };

    ShaderResource                 mPureWhiteTexture;
    AutoComPtr<ID3D11VertexShader> mVs;
    AutoComPtr<ID3D11PixelShader>  mPs;
    AutoComPtr<ID3D11InputLayout>  mVertexBinding;
    AutoComPtr<ID3D11Buffer>       mVertexBuffer;
    AutoComPtr<ID3D11Buffer>       mIndexBuffer;

    ID3D11DeviceContext      * mCxt;
    ID3D11ShaderResourceView * mTexture;
    UINT                       mOptions;
    float                      mVertexShift;
    bool                       mDrawBegun;
    Sprite                   * mSprites;
    Sprite                   * mNextPendingSprite;
    Sprite                   * mNextFreeSprite;

public:

    // -----------------------------------------------------------------------------
    bool Initialize(ID3D11Device & dev)
    {
        enum
        {
            MAX_VERTICES = MAX_SPRITES * 4,
            MAX_INDICES  = MAX_SPRITES * 6,
            VTXBUF_SIZE  = MAX_VERTICES * sizeof(SpriteVertex)
        };

        AutoComPtr<ID3D11DeviceContext> cxt;
        dev.GetImmediateContext(&cxt);

        // create a 2x2 pure white texture
        if (!mPureWhiteTexture.create2D(dev, 2, 2, DXGI_FORMAT_R8G8B8A8_UNORM)) return false;
        const UINT PURE_WHITE[] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF } ;
        cxt->UpdateSubresource(
            mPureWhiteTexture.res,
            0,
            nullptr,
            PURE_WHITE,
            sizeof(UINT)*2,
            sizeof(UINT)*4);

        // create GPU programs
        AutoComPtr<ID3DBlob> signature;
        mVs.attach(createVS(dev, hlslvscode, 0, 0, "main", nullptr, &signature));
        mPs.attach(createPS(dev, hlslpscode));
        if(!mVs || !mPs) return false;

        // create input layout
        D3D11_INPUT_ELEMENT_DESC elements[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                                0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  FIELD_OFFSET(SpriteVertex, clr) , 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    FIELD_OFFSET(SpriteVertex, tex) , 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        GN_RETURN_FALSE_ON_HR_FAILED(dev.CreateInputLayout(elements, _countof(elements), signature->GetBufferPointer(), signature->GetBufferSize(), &mVertexBinding));

        // create vertex buffer
        D3D11_BUFFER_DESC bufdesc =
        {
            VTXBUF_SIZE,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_VERTEX_BUFFER,
            0, // cpu access flags
            0, // misc flags
            sizeof(SpriteVertex),
        };
        GN_RETURN_FALSE_ON_HR_FAILED(dev.CreateBuffer(&bufdesc, nullptr, &mVertexBuffer));

        // create index buffer
        bufdesc.ByteWidth = MAX_INDICES * sizeof(SHORT);
        bufdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufdesc.StructureByteStride = sizeof(SHORT);
        std::vector<UINT16> indices( MAX_INDICES );
        for (UINT16 i = 0; i < MAX_SPRITES; ++i)
        {
            indices[i*6+0] = i * 4 + 0;
            indices[i*6+1] = i * 4 + 1;
            indices[i*6+2] = i * 4 + 2;
            indices[i*6+3] = i * 4 + 0;
            indices[i*6+4] = i * 4 + 2;
            indices[i*6+5] = i * 4 + 3;
        }
        D3D11_SUBRESOURCE_DATA indicesData = { &indices[0], 0, 0 };
        GN_RETURN_FALSE_ON_HR_FAILED(dev.CreateBuffer(&bufdesc, &indicesData, &mIndexBuffer));

        // create pending vertex buffer
        mSprites = (Sprite*)malloc( VTXBUF_SIZE );
        if( NULL == mSprites ) { GN_ERROR(sLogger)("Out of memory."); return false; }

        // initialize other member variables
        mDrawBegun = false;
        mNextPendingSprite = mSprites;
        mNextFreeSprite = mSprites;

        // success
        return true;
    }

    // -----------------------------------------------------------------------------
    void Shutdown()
    {
        free(mSprites), mSprites = nullptr;
        mIndexBuffer.clear();
        mVertexBuffer.clear();
        mVertexBinding.clear();
        mVs.clear();
        mPs.clear();
        mPureWhiteTexture.cleanup();
    }

    // -----------------------------------------------------------------------------
    void DrawBegin(ID3D11DeviceContext & cxt, ID3D11ShaderResourceView * texture, UINT options)
    {
        if( mDrawBegun )
        {
            GN_ERROR(sLogger)( "SpriteRenderer::drawBegin() can not be called consequtively w/o DrawEnd() in between" );
            return;
        }

        // use pure white texture, if input texture is NULL
        if( NULL == texture ) texture = mPureWhiteTexture.srv;

        // setup parameters that are not affected by options
        UINT stride = sizeof(SpriteVertex);
        UINT offset = 0;
        cxt.PSSetShaderResources(0, 1, &mPureWhiteTexture.srv);
        cxt.IASetInputLayout(mVertexBinding);
        cxt.IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
        cxt.IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        cxt.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // setup shaders
        if( 0 == (options & USE_EXTERNAL_GPU_PROGRAM) )
        {
            cxt.VSSetShader(mVs, nullptr, 0);
            cxt.HSSetShader(nullptr, nullptr, 0);
            cxt.DSSetShader(nullptr, nullptr, 0);
            cxt.GSSetShader(nullptr, nullptr, 0);
            cxt.PSSetShader(mPs, nullptr, 0);
        }
        if( 0 == (options & USE_EXTERNAL_TEXTURE_FILTERS) )
        {
            // TODO: use point sampling by default
            //mContext.textures[0].sampler.filterMin = SamplerDesc::FILTER_POINT;
            //mContext.textures[0].sampler.filterMip = SamplerDesc::FILTER_POINT;
            //mContext.textures[0].sampler.filterMag = SamplerDesc::FILTER_POINT;
        }

        // setup alpha blending
        if( options & FORCE_ALPHA_BLENDING_ENABLED )
        {
            // TODO: setup alpha blending state
            if (options & FORCE_ALPHA_BLENDING_DISABLED)
            {
                GN_WARN(sLogger)( "FORCE_ALPHA_BLENDING_ENABLED and FORCE_ALPHA_BLENDING_DISABLED should not be specifed together!" );
            }

            //mContext.rs.alphaBlend[0].blendEnabled  = true;
            //mContext.rs.alphaBlend[0].blendSrc      = GpuContext::BLEND_SRC_ALPHA;
            //mContext.rs.alphaBlend[0].blendDst      = GpuContext::BLEND_INV_SRC_ALPHA;
            //mContext.rs.alphaBlend[0].blendOp       = GpuContext::BLEND_OP_ADD;
            //mContext.rs.alphaBlend[0].blendAlphaSrc = GpuContext::BLEND_SRC_ALPHA;
            //mContext.rs.alphaBlend[0].blendAlphaDst = GpuContext::BLEND_INV_SRC_ALPHA;
            //mContext.rs.alphaBlend[0].blendAlphaOp  = GpuContext::BLEND_OP_ADD;
        }
        else if( options & FORCE_ALPHA_BLENDING_DISABLED )
        {
            // TODO: setup alpha blending state
            //mContext.rs.alphaBlend[0].blendEnabled = false;
        }

        // setup depth stencil state
        if( options & FORCE_DEPTH_TEST_ENABLED )
        {
            if( options & FORCE_DEPTH_TEST_DISABLED )
            {
                GN_WARN(sLogger)( "FORCE_DEPTH_TEST_ENABLED and FORCE_DEPTH_TEST_DISABLED should not be specifed together!" );
            }

            //mContext.rs.depthTestEnabled = true;
        }
        else if( options & FORCE_DEPTH_TEST_DISABLED )
        {
            //mContext.rs.depthTestEnabled = false;
        }

        // setup depth write
        if( options & FORCE_DEPTH_WRITE_ENABLED )
        {
            if( options & FORCE_DEPTH_WRITE_DISABLED )
            {
                GN_WARN(sLogger)( "FORCE_DEPTH_WRITE_ENABLED and FORCE_DEPTH_WRITE_DISABLED should not be specifed together!" );
            }

            //mContext.rs.depthWriteEnabled = true;
        }
        else if( options & FORCE_DEPTH_WRITE_DISABLED )
        {
            //mContext.rs.depthWriteEnabled = false;
        }

        // no shift on D3D11 device
        mVertexShift = 0;

        mDrawBegun = true;
        mOptions   = options;
        mCxt       = &cxt;
        mTexture   = texture;
    }

    //
    //
    // -----------------------------------------------------------------------------
    void DrawEnd()
    {
        if( !mDrawBegun )
        {
            GN_ERROR(sLogger)( "SpriteRenderer::drawEnd() can not be called consequtively w/o drawBegin() in between" );
            return;
        }

        size_t numPendingSprites = mNextFreeSprite - mNextPendingSprite;
        if( numPendingSprites > 0 )
        {
            size_t firstPendingSpriteOffset = mNextPendingSprite - mSprites;

            D3D11_BOX box =
            {
                (UINT)(firstPendingSpriteOffset * sizeof(Sprite)), 0, 0,
                (UINT)(numPendingSprites * sizeof(Sprite)), 1, 1
            };
            mCxt->UpdateSubresource(mVertexBuffer, 0, &box, mNextPendingSprite, 0, 0);

            mCxt->DrawIndexed(
                (UINT)(numPendingSprites * 6), // index count
                0, // start index location
                (UINT)(firstPendingSpriteOffset * 4) // base vertex location
                );
        }

        mDrawBegun = false;
        if( mNextFreeSprite == mSprites + MAX_SPRITES ) mNextFreeSprite = mSprites; // rewind next-free pointer if needed.
        mNextPendingSprite = mNextFreeSprite;
    }

    // -----------------------------------------------------------------------------
    void DrawTexturedSprite(const TexturedSprite & ts)
    {
        if( !mDrawBegun )
        {
            GN_ERROR(sLogger)( "Must be called between drawBegin() and drawEnd()." );
            return;
        }

        if( mNextFreeSprite == mSprites + MAX_SPRITES )
        {
            DrawEnd();
            DrawBegin(*mCxt, mTexture, mOptions );
        }

        GN_ASSERT( mNextFreeSprite < mSprites + MAX_SPRITES );

        // TODO: get screen size based on current context
        UINT screenWidth = 1920, screenHeight = 1080;

        float x1 = ( ts.x + mVertexShift ) / screenWidth;
        float y1 = ( ts.y + mVertexShift ) / screenHeight;
        float x2 = x1 + ts.w / screenWidth;
        float y2 = y1 + ts.h / screenHeight;
        float u2 = ts.u + ts.tw;
        float v2 = ts.v + ts.th;

        // fill vertex buffer
        mNextFreeSprite->v[0].pos.Set( x1, y1, ts.z );
        mNextFreeSprite->v[0].clr = 0xFFFFFFFF;
        mNextFreeSprite->v[0].tex.Set( ts.u, ts.v );

        mNextFreeSprite->v[1].pos.Set( x1, y2, ts.z );
        mNextFreeSprite->v[1].clr = 0xFFFFFFFF;
        mNextFreeSprite->v[1].tex.Set( ts.u, v2 );

        mNextFreeSprite->v[2].pos.Set( x2, y2, ts.z );
        mNextFreeSprite->v[2].clr = 0xFFFFFFFF;
        mNextFreeSprite->v[2].tex.Set( u2, v2 );

        mNextFreeSprite->v[3].pos.Set( x2, y1, ts.z );
        mNextFreeSprite->v[3].clr = 0xFFFFFFFF;
        mNextFreeSprite->v[3].tex.Set( u2, ts.v );

        // prepare for next sprite
        ++mNextFreeSprite;
    }

    // -----------------------------------------------------------------------------
    void DrawSolidSprite(const SolidSprite & ss)
    {
        if( !mDrawBegun )
        {
            GN_ERROR(sLogger)( "Must be called between drawBegin() and drawEnd()." );
            return;
        }

        if( mNextFreeSprite == mSprites + MAX_SPRITES )
        {
            DrawEnd();
            DrawBegin(*mCxt, mTexture, mOptions);
        }

        GN_ASSERT( mNextFreeSprite < mSprites + MAX_SPRITES );

        // TODO: get screen size based on current context
        UINT screenWidth = 1920, screenHeight = 1080;

        float x1 = ss.x / screenWidth;
        float y1 = ss.y / screenHeight;
        float x2 = (ss.x + ss.w) / screenWidth;
        float y2 = (ss.y + ss.h) / screenHeight;

        mNextFreeSprite->v[0].pos.Set( x1, y1, ss.z );
        mNextFreeSprite->v[0].clr = ss.color;
        mNextFreeSprite->v[0].tex.Set( 0, 0 );

        mNextFreeSprite->v[1].pos.Set( x1, y2, ss.z );
        mNextFreeSprite->v[1].clr = ss.color;
        mNextFreeSprite->v[1].tex.Set( 0, 0 );

        mNextFreeSprite->v[2].pos.Set( x2, y2, ss.z );
        mNextFreeSprite->v[2].clr = ss.color;
        mNextFreeSprite->v[2].tex.Set( 0, 0 );

        mNextFreeSprite->v[3].pos.Set( x2, y1, ss.z );
        mNextFreeSprite->v[3].clr = ss.color;
        mNextFreeSprite->v[3].tex.Set( 0, 0 );

        // prepare for next Sprite
        ++mNextFreeSprite;
    }
};


// -----------------------------------------------------------------------------
bool GN::d3d11::SpriteRenderer::initialize(ID3D11Device & dev)
{
    shutdown();
    std::unique_ptr<Impl> p(new Impl);
    if (nullptr == p) { GN_ERROR(sLogger)("out of memory"); return false; }
    if (!p->Initialize(dev)) return false;
    _impl = p.release();
    return true;
}

// -----------------------------------------------------------------------------
void GN::d3d11::SpriteRenderer::shutdown()
{
    delete _impl;
    _impl = nullptr;
}

// -----------------------------------------------------------------------------
void GN::d3d11::SpriteRenderer::drawBegin(ID3D11DeviceContext & cxt, ID3D11ShaderResourceView * texture, UINT options)
{
    if(nullptr != _impl)
    {
        _impl->DrawBegin(cxt, texture, options);
    }
}

// -----------------------------------------------------------------------------
void GN::d3d11::SpriteRenderer::drawEnd()
{
    if(nullptr != _impl)
    {
        _impl->DrawEnd();
    }
}

// -----------------------------------------------------------------------------
void GN::d3d11::SpriteRenderer::drawTexturedSprite(const TexturedSprite & ts)
{
    if(nullptr != _impl)
    {
        _impl->DrawTexturedSprite(ts);
    }
}

// -----------------------------------------------------------------------------
void GN::d3d11::SpriteRenderer::drawSolidSprite(const SolidSprite & ss)
{
    if(nullptr != _impl)
    {
        _impl->DrawSolidSprite(ss);
    }
}
