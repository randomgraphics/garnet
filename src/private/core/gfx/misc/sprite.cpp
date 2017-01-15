#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.SpriteRenderer");

static const char * glslvscode=
    "varying vec4 color; \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_Position.x = gl_Vertex.x * 2.0 - 1.0; \n"
    "   gl_Position.y = gl_Vertex.y * -2.0 + 1.0; \n"
    "   gl_Position.zw = gl_Vertex.zw; \n"
    "   color = gl_Color; \n"
    "   texcoords.xy = gl_MultiTexCoord0.xy; \n"
    "}";

static const char * glslpscode=
    "uniform sampler2D t0; \n"
    "varying vec4 color; \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_FragColor = color * texture2D( t0, texcoords ); \n"
    "}";

static const char * hlslvscode=
    "struct VSOUT { \n"
    "   float4 position  : POSITION0; \n"
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
    "sampler t0; \n"
    "struct VSOUT { \n"
    "   float4 position  : POSITION0; \n"
    "   float4 color     : COLOR; \n"
    "   float2 texcoords : TEXCOORD0; \n"
    "}; \n"
    "float4 main( VSOUT i ) : COLOR0 { \n"
    "   return i.color * tex2D( t0, i.texcoords ); \n"
    "}";

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SpriteRenderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SpriteRenderer, () );

    enum
    {
        MAX_VERTICES = MAX_SPRITES * 4,
        MAX_INDICES  = MAX_SPRITES * 6,
        VTXBUF_SIZE  = MAX_VERTICES * sizeof(SpriteVertex)
    };

    // create a 2x2 pure white texture
    mPureWhiteTexture.attach( mGpu.create2DTexture( 2, 2, 0, ColorFormat::RGBA32 ) );
    if( !mPureWhiteTexture ) return failure();
    const uint32 PURE_WHITE[] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF } ;
    mPureWhiteTexture->updateMipmap( 0, 0, NULL, sizeof(uint32)*2, sizeof(uint32)*4, &PURE_WHITE );

    // create GPU program
    const GpuCaps & caps = mGpu.caps();
    GpuProgramDesc gpd;
    if( caps.shaderModels & ShaderModel::GLSL_1_00 )
    {
        gpd.lang = GpuProgramLanguage::GLSL;
        gpd.shaderModels = ShaderModel::GLSL_1_00;
        gpd.vs.source = glslvscode;
        gpd.ps.source = glslpscode;

        mGpuProgram.attach( mGpu.createGpuProgram( gpd ) );
        if( !mGpuProgram ) return failure();

        mVertexBinding.resize( 3 );
        mVertexBinding[0].stream    = 0;
        mVertexBinding[0].offset    = 0;
        mVertexBinding[0].format    = ColorFormat::FLOAT3;
        mVertexBinding[0].attribute = mGpuProgram->getParameterDesc().attributes["gl_Vertex"];
        mVertexBinding[1].stream    = 0;
        mVertexBinding[1].offset    = GN_FIELD_OFFSET( SpriteVertex, clr );
        mVertexBinding[1].format    = ColorFormat::RGBA32;
        mVertexBinding[1].attribute = mGpuProgram->getParameterDesc().attributes["gl_Color"];
        mVertexBinding[2].stream    = 0;
        mVertexBinding[2].offset    = GN_FIELD_OFFSET( SpriteVertex, tex );
        mVertexBinding[2].format    = ColorFormat::FLOAT2;
        mVertexBinding[2].attribute = mGpuProgram->getParameterDesc().attributes["gl_MultiTexCoord0"];
    }
    else if( caps.shaderModels & ShaderModel::SM_2_0 )
    {
        gpd.lang = GpuProgramLanguage::HLSL9;
        gpd.shaderModels = ShaderModel::SM_2_0;
        gpd.vs.source = hlslvscode;
        gpd.vs.entry = "main";
        gpd.ps.source = hlslpscode;
        gpd.ps.entry = "main";

        mGpuProgram.attach( mGpu.createGpuProgram( gpd ) );
        if( !mGpuProgram ) return failure();

        mVertexBinding.resize( 3 );
        mVertexBinding[0].stream    = 0;
        mVertexBinding[0].offset    = 0;
        mVertexBinding[0].format    = ColorFormat::FLOAT3;
        mVertexBinding[0].attribute = mGpuProgram->getParameterDesc().attributes["POSITION0"];
        mVertexBinding[1].stream    = 0;
        mVertexBinding[1].offset    = GN_FIELD_OFFSET( SpriteVertex, clr );
        mVertexBinding[1].format    = ColorFormat::RGBA32;
        mVertexBinding[1].attribute = mGpuProgram->getParameterDesc().attributes["COLOR0"];
        mVertexBinding[2].stream    = 0;
        mVertexBinding[2].offset    = GN_FIELD_OFFSET( SpriteVertex, tex );
        mVertexBinding[2].format    = ColorFormat::FLOAT2;
        mVertexBinding[2].attribute = mGpuProgram->getParameterDesc().attributes["TEXCOORD0"];
    }
    else
    {
        GN_ERROR(sLogger)( "Sprite renderer requires either GLSL or HLSL support from graphics hardware." );
        return failure();
    }

    // create vertex buffer
    mVertexBuffer.attach( mGpu.createVtxBuf( VTXBUF_SIZE, true ) );
    if( !mVertexBuffer ) return failure();

    // create index buffer
    mIndexBuffer.attach( mGpu.createIdxBuf16( MAX_INDICES, false ) );
    if( !mIndexBuffer ) return failure();
    DynaArray<uint16> indices( MAX_INDICES );
    for( uint16 i = 0; i < MAX_SPRITES; ++i )
    {
        indices[i*6+0] = i * 4 + 0;
        indices[i*6+1] = i * 4 + 1;
        indices[i*6+2] = i * 4 + 2;
        indices[i*6+3] = i * 4 + 0;
        indices[i*6+4] = i * 4 + 2;
        indices[i*6+5] = i * 4 + 3;
    }
    mIndexBuffer->update( 0, MAX_INDICES, indices.rawptr() );


    // create pending vertex buffer
    mSprites = (Sprite*)HeapMemory::alloc( VTXBUF_SIZE );
    if( NULL == mSprites ) return failure();

    // initialize other member variables
    mDrawBegun = false;
    mNextPendingSprite = mSprites;
    mNextFreeSprite = mSprites;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SpriteRenderer::quit()
{
    GN_GUARD;

    HeapMemory::dealloc( mSprites );
    mContext.clear();
    mIndexBuffer.clear();
    mVertexBuffer.clear();
    mGpuProgram.clear();
    mPureWhiteTexture.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SpriteRenderer::drawBegin( Texture * texture, uint32 options )
{
    if( mDrawBegun )
    {
        GN_ERROR(sLogger)( "SpriteRenderer::drawBegin() can not be called consequtively w/o drawEnd() in between" );
        return;
    }

    // use pure white texture, if input texture is NULL
    if( NULL == texture ) texture = mPureWhiteTexture;

    // copy render targets from current context
    mContext.colortargets = mGpu.getContext().colortargets;
    mContext.depthstencil = mGpu.getContext().depthstencil;

    // setup parameters that are not affected by options
    mContext.textures[0].texture.set( texture );
    mContext.vtxbind           = mVertexBinding;
    mContext.vtxbufs[0].vtxbuf = mVertexBuffer;
    mContext.vtxbufs[0].stride = sizeof(SpriteVertex);
    mContext.idxbuf            = mIndexBuffer;

    // setup GPU program
    if( 0 == (options & USE_EXTERNAL_GPU_PROGRAM) )
    {
        mContext.gpuProgram = mGpuProgram;

        // default shader has no uniform
        mContext.uniforms.clear();
    }
    if( 0 == (options & USE_EXTERNAL_TEXTURE_FILTERS) )
    {
        // use point sampling by default
        mContext.textures[0].sampler.filterMin = SamplerDesc::FILTER_POINT;
        mContext.textures[0].sampler.filterMip = SamplerDesc::FILTER_POINT;
        mContext.textures[0].sampler.filterMag = SamplerDesc::FILTER_POINT;
    }

    // setup alpha blending
    if( options & FORCE_ALPHA_BLENDING_ENABLED )
    {
        if( options & FORCE_ALPHA_BLENDING_DISABLED )
        {
            GN_WARN(sLogger)( "FORCE_ALPHA_BLENDING_ENABLED and FORCE_ALPHA_BLENDING_DISABLED should not be specifed together!" );
        }

        mContext.rs.alphaBlend[0].blendEnabled  = true;
        mContext.rs.alphaBlend[0].blendSrc      = GpuContext::BLEND_SRC_ALPHA;
        mContext.rs.alphaBlend[0].blendDst      = GpuContext::BLEND_INV_SRC_ALPHA;
        mContext.rs.alphaBlend[0].blendOp       = GpuContext::BLEND_OP_ADD;
        mContext.rs.alphaBlend[0].blendAlphaSrc = GpuContext::BLEND_SRC_ALPHA;
        mContext.rs.alphaBlend[0].blendAlphaDst = GpuContext::BLEND_INV_SRC_ALPHA;
        mContext.rs.alphaBlend[0].blendAlphaOp  = GpuContext::BLEND_OP_ADD;
    }
    else if( options & FORCE_ALPHA_BLENDING_DISABLED )
    {
        mContext.rs.alphaBlend[0].blendEnabled = false;
    }

    // setup depth test
    if( options & FORCE_DEPTH_TEST_ENABLED )
    {
        if( options & FORCE_DEPTH_TEST_DISABLED )
        {
            GN_WARN(sLogger)( "FORCE_DEPTH_TEST_ENABLED and FORCE_DEPTH_TEST_DISABLED should not be specifed together!" );
        }

        mContext.rs.depthTestEnabled = true;
    }
    else if( options & FORCE_DEPTH_TEST_DISABLED )
    {
        mContext.rs.depthTestEnabled = false;
    }

    // setup depth write
    if( options & FORCE_DEPTH_WRITE_ENABLED )
    {
        if( options & FORCE_DEPTH_WRITE_DISABLED )
        {
            GN_WARN(sLogger)( "FORCE_DEPTH_WRITE_ENABLED and FORCE_DEPTH_WRITE_DISABLED should not be specifed together!" );
        }

        mContext.rs.depthWriteEnabled = true;
    }
    else if( options & FORCE_DEPTH_WRITE_DISABLED )
    {
        mContext.rs.depthWriteEnabled = false;
    }

    // Note: D3D9 and Xenon needs -0.5f vertex shift
    if( mGpu.getOptions().api == GpuAPI::XENON )
    {
        mVertexShift = -0.5f;
    }
    else
    {
        mVertexShift = 0;
    }

    mDrawBegun = true;
    mOptions   = options;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SpriteRenderer::drawEnd()
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

        mVertexBuffer->update(
            (uint32)(firstPendingSpriteOffset * sizeof(Sprite)),
            (uint32)(numPendingSprites * sizeof(Sprite)),
            mNextPendingSprite,
            mSprites == mNextPendingSprite ? SurfaceUpdateFlag::DISCARD : SurfaceUpdateFlag::NO_OVERWRITE );

        mGpu.bindContext( mContext );

        mGpu.drawIndexed(
            PrimitiveType::TRIANGLE_LIST,
            (uint32)(numPendingSprites * 6),        // numidx
            (uint32)(firstPendingSpriteOffset * 4), // basevtx,
            0,                                      // startvtx
            (uint32)(numPendingSprites * 4),        // numvtx
            0 );                                    // startidx
    }

    mDrawBegun = false;
    if( mNextFreeSprite == mSprites + MAX_SPRITES ) mNextFreeSprite = mSprites; // rewind next-free pointer if needed.
    mNextPendingSprite = mNextFreeSprite;
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::SpriteRenderer::drawTextured(
    float x,
    float y,
    float w,
    float h,
    float u,
    float v,
    float tw,
    float th,
    float z )
{
    if( !mDrawBegun )
    {
        GN_ERROR(sLogger)( "Must be called between drawBegin() and drawEnd()." );
        return;
    }

    if( mNextFreeSprite == mSprites + MAX_SPRITES )
    {
        drawEnd();
        drawBegin( mContext.textures[0].texture, mOptions );
    }

    GN_ASSERT( mNextFreeSprite < mSprites + MAX_SPRITES );

    // get screen size based on current context
    uint32 screenWidth, screenHeight;
    mGpu.getCurrentRenderTargetSize( &screenWidth, &screenHeight );

    float x1 = ( x + mVertexShift ) / screenWidth;
    float y1 = ( y + mVertexShift ) / screenHeight;
    float x2 = x1 + w / screenWidth;
    float y2 = y1 + h / screenHeight;
    float u2 = u + tw;
    float v2 = v + th;

    // fill vertex buffer
    mNextFreeSprite->v[0].pos.set( x1, y1, z );
    mNextFreeSprite->v[0].clr = 0xFFFFFFFF;
    mNextFreeSprite->v[0].tex.set( u, v );

    mNextFreeSprite->v[1].pos.set( x1, y2, z );
    mNextFreeSprite->v[1].clr = 0xFFFFFFFF;
    mNextFreeSprite->v[1].tex.set( u, v2 );

    mNextFreeSprite->v[2].pos.set( x2, y2, z );
    mNextFreeSprite->v[2].clr = 0xFFFFFFFF;
    mNextFreeSprite->v[2].tex.set( u2, v2 );

    mNextFreeSprite->v[3].pos.set( x2, y1, z );
    mNextFreeSprite->v[3].clr = 0xFFFFFFFF;
    mNextFreeSprite->v[3].tex.set( u2, v );

    // prepare for next sprite
    ++mNextFreeSprite;
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::SpriteRenderer::drawSolid(
    uint32 rgba,
    float  x,
    float  y,
    float  w,
    float  h,
    float  z )
{
    if( !mDrawBegun )
    {
        GN_ERROR(sLogger)( "Must be called between drawBegin() and drawEnd()." );
        return;
    }

    if( mNextFreeSprite == mSprites + MAX_SPRITES )
    {
        drawEnd();
        drawBegin( mContext.textures[0].texture, mOptions );
    }

    GN_ASSERT( mNextFreeSprite < mSprites + MAX_SPRITES );

    // get screen size based on current context
    uint32 screenWidth, screenHeight;
    mGpu.getCurrentRenderTargetSize( &screenWidth, &screenHeight );

    float x1 = x / screenWidth;
    float y1 = y / screenHeight;
    float x2 = (x + w) / screenWidth;
    float y2 = (y + h) / screenHeight;

    mNextFreeSprite->v[0].pos.set( x1, y1, z );
    mNextFreeSprite->v[0].clr = rgba;
    mNextFreeSprite->v[0].tex.set( 0, 0 );

    mNextFreeSprite->v[1].pos.set( x1, y2, z );
    mNextFreeSprite->v[1].clr = rgba;
    mNextFreeSprite->v[1].tex.set( 0, 0 );

    mNextFreeSprite->v[2].pos.set( x2, y2, z );
    mNextFreeSprite->v[2].clr = rgba;
    mNextFreeSprite->v[2].tex.set( 0, 0 );

    mNextFreeSprite->v[3].pos.set( x2, y1, z );
    mNextFreeSprite->v[3].clr = rgba;
    mNextFreeSprite->v[3].tex.set( 0, 0 );

    // prepare for next Sprite
    ++mNextFreeSprite;
}
