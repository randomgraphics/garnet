#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.LineRenderer");

static const char * glslvscode=
    "varying vec4 color; \n"
    "void main() { \n"
    "   // NOTE: GLSL matrix is colomn major \n"
    "   mat4 pvw = mat4(gl_MultiTexCoord0, gl_MultiTexCoord1, gl_MultiTexCoord2, gl_MultiTexCoord3); \n"
    "   gl_Position = gl_Vertex * pvw; \n"
    "   color       = gl_Color; \n"
    "}";

static const char * glslpscode=
    "varying vec4 color; \n"
    "void main() { \n"
    "   gl_FragColor = color; \n"
    "}";

static const char * hlslvscode=
    "struct VSIN { \n"
    "   float4 pos    : POSITION0; \n"
    "   float4 clr    : COLOR; \n"
    "   float4 pvw_r0 : TEXCOORD0; \n"
    "   float4 pvw_r1 : TEXCOORD1; \n"
    "   float4 pvw_r2 : TEXCOORD2; \n"
    "   float4 pvw_r3 : TEXCOORD3; \n"
    "}; \n"
    "struct VSOUT { \n"
    "   float4 pos    : POSITION; \n"
    "   float4 clr    : COLOR; \n"
    "}; \n"
    "VSOUT main( in VSIN i ) { \n"
    "   float4x4 pvw = { i.pvw_r0, i.pvw_r1, i.pvw_r2, i.pvw_r3 }; \n"
    "   VSOUT o; \n"
    "   o.pos = mul( pvw, i.pos ); \n"
    "   o.clr = i.clr; \n"
    "   return o; \n"
    "}";

static const char * hlslpscode=
    "struct VSIO { \n"
    "   float4 pos : POSITION; \n"
    "   float4 clr : COLOR; \n"
    "}; \n"
    "float4 main( in VSIO i ) : COLOR0 { \n"
    "   return i.clr; \n"
    "}";

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::LineRenderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::LineRenderer, () );

    // create vertex format
    mContext.vtxbind.resize( 6 );
    mContext.vtxbind[0].stream = 0;
    mContext.vtxbind[0].offset = GN_FIELD_OFFSET( LineVertex, pos );
    mContext.vtxbind[0].format = ColorFormat::FLOAT3;
    mContext.vtxbind[1].stream = 0;
    mContext.vtxbind[1].offset = GN_FIELD_OFFSET( LineVertex, colorInRGBA );
    mContext.vtxbind[1].format = ColorFormat::RGBA32;
    mContext.vtxbind[2].stream = 0;
    mContext.vtxbind[2].offset = GN_FIELD_OFFSET( LineVertex, transform );
    mContext.vtxbind[2].format = ColorFormat::FLOAT4;
    mContext.vtxbind[3].stream = 0;
    mContext.vtxbind[3].offset = GN_FIELD_OFFSET( LineVertex, transform ) + sizeof(Vector4f);
    mContext.vtxbind[3].format = ColorFormat::FLOAT4;
    mContext.vtxbind[4].stream = 0;
    mContext.vtxbind[4].offset = GN_FIELD_OFFSET( LineVertex, transform ) + sizeof(Vector4f) * 2;
    mContext.vtxbind[4].format = ColorFormat::FLOAT4;
    mContext.vtxbind[5].stream = 0;
    mContext.vtxbind[5].offset = GN_FIELD_OFFSET( LineVertex, transform ) + sizeof(Vector4f) * 3;
    mContext.vtxbind[5].format = ColorFormat::FLOAT4;

    // create GPU program
    const GpuCaps & caps = mGpu.caps();
    GpuProgramDesc gpd;
    if( caps.shaderModels & ShaderModel::GLSL_1_00 )
    {
        gpd.lang = GpuProgramLanguage::GLSL;
        gpd.shaderModels = ShaderModel::GLSL_1_00;
        gpd.vs.source = glslvscode;
        gpd.ps.source = glslpscode;

        mContext.gpuProgram.attach( mGpu.createGpuProgram( gpd ) );
        if( !mContext.gpuProgram ) return failure();

        const GpuProgramParameterDesc & gppd = mContext.gpuProgram->getParameterDesc();

        mContext.vtxbind[0].attribute = gppd.attributes["gl_Vertex"];
        mContext.vtxbind[1].attribute = gppd.attributes["gl_Color"];
        mContext.vtxbind[2].attribute = gppd.attributes["gl_MultiTexCoord0"];
        mContext.vtxbind[3].attribute = gppd.attributes["gl_MultiTexCoord1"];
        mContext.vtxbind[4].attribute = gppd.attributes["gl_MultiTexCoord2"];
        mContext.vtxbind[5].attribute = gppd.attributes["gl_MultiTexCoord3"];
    }
    else if( caps.shaderModels & ShaderModel::SM_2_0 )
    {
        gpd.lang = GpuProgramLanguage::HLSL9;
        gpd.shaderModels = ShaderModel::SM_2_0;
        gpd.vs.source = hlslvscode;
        gpd.vs.entry = "main";
        gpd.ps.source = hlslpscode;
        gpd.ps.entry = "main";

        mContext.gpuProgram.attach( mGpu.createGpuProgram( gpd ) );
        if( !mContext.gpuProgram ) return failure();

        const GpuProgramParameterDesc & gppd = mContext.gpuProgram->getParameterDesc();

        mContext.vtxbind[0].attribute = gppd.attributes["POSITION0"];
        mContext.vtxbind[1].attribute = gppd.attributes["COLOR0"];
        mContext.vtxbind[2].attribute = gppd.attributes["TEXCOORD0"];
        mContext.vtxbind[3].attribute = gppd.attributes["TEXCOORD1"];
        mContext.vtxbind[4].attribute = gppd.attributes["TEXCOORD2"];
        mContext.vtxbind[5].attribute = gppd.attributes["TEXCOORD3"];
    }
    else
    {
        GN_ERROR(sLogger)( "Sprite renderer requires either GLSL or HLSL support from graphics hardware." );
        return failure();
    }

    // create vertex buffer
    mContext.vtxbufs[0].vtxbuf.attach( mGpu.createVtxBuf( MAX_LINES * sizeof(Line), true ) );
    if( !mContext.vtxbufs[0].vtxbuf ) return failure();
    mContext.vtxbufs[0].stride = sizeof(LineVertex);

    // create line buffer
    mLines = (Line*)HeapMemory::alloc( MAX_LINES * sizeof(Line) );
    if( NULL == mLines ) return failure();
    mNextPendingLine = mLines;
    mNextFreeLine = mLines;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::LineRenderer::quit()
{
    GN_GUARD;

    HeapMemory::dealloc( mLines ); mLines = NULL;
    mContext.clear();

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
void GN::gfx::LineRenderer::drawLines(
    const void *      positions,
    size_t            stride,
    size_t            numpoints,
    uint32            colorInRgba,
    const Matrix44f & transform )
{
    if( 0 == stride ) stride = sizeof(float)*3;

    size_t numNewLines = numpoints / 2;

    const uint8 * positionsU8 = (const uint8*)positions;

    if( numNewLines > MAX_LINES )
    {
        // handle line buffer longer than maxinum length.
        for( size_t i = 0; i < numNewLines / MAX_LINES; ++i )
        {
            drawLines(
                positionsU8,
                stride,
                MAX_LINES * 2,
                colorInRgba,
                transform );

            positionsU8 += MAX_LINES * 2 * stride;
        }
        numNewLines %= MAX_LINES;
    }

    GN_ASSERT( numNewLines <= MAX_LINES );

    if( numNewLines + mNextFreeLine > mLines + MAX_LINES )
    {
        // there's no enough space to hold all incoming lines. So flush first.
        flush();
    }

    GN_ASSERT( numNewLines + mNextFreeLine <= mLines + MAX_LINES );

    for( size_t i = 0; i < numNewLines; ++i )
    {
        GN_ASSERT( mLines <= mNextFreeLine && mNextFreeLine < (mLines + MAX_LINES) );
        mNextFreeLine->v0.pos = *(const Vector3f*)positionsU8;
        mNextFreeLine->v0.colorInRGBA = colorInRgba;
        mNextFreeLine->v0.transform = transform;

        mNextFreeLine->v1.pos = *(const Vector3f*)(positionsU8 + stride);
        mNextFreeLine->v1.colorInRGBA = colorInRgba;
        mNextFreeLine->v1.transform = transform;

        // next line segement
        positionsU8 += stride * 2;
        ++mNextFreeLine;
    }

    if( !mBatchingModeEnabled ) flush();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::LineRenderer::drawBox(
    const Boxf      & box,
    uint32            colorInRgba,
    const Matrix44f & transform )
{
    Vector3f points[] =
    {
        box.corner( 0 ), box.corner( 1 ),
        box.corner( 1 ), box.corner( 2 ),
        box.corner( 2 ), box.corner( 3 ),
        box.corner( 3 ), box.corner( 0 ),

        box.corner( 0 ), box.corner( 4 ),
        box.corner( 1 ), box.corner( 5 ),
        box.corner( 2 ), box.corner( 6 ),
        box.corner( 3 ), box.corner( 7 ),

        box.corner( 4 ), box.corner( 5 ),
        box.corner( 5 ), box.corner( 6 ),
        box.corner( 6 ), box.corner( 7 ),
        box.corner( 7 ), box.corner( 4 ),
    };

    drawLines( points, sizeof(Vector3f), GN_ARRAY_COUNT(points), colorInRgba, transform );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::LineRenderer::flush()
{
    size_t numPendingLines = mNextFreeLine - mNextPendingLine;
    if( 0 == numPendingLines ) return;

    size_t firstPendingLineOffset = mNextPendingLine - mLines;

    GN_ASSERT( firstPendingLineOffset + numPendingLines <= MAX_LINES );

    // copy render targets from current context
    mContext.colortargets = mGpu.getContext().colortargets;
    mContext.depthstencil = mGpu.getContext().depthstencil;

    mContext.vtxbufs[0].vtxbuf->update(
        (uint32)(firstPendingLineOffset * sizeof(Line)),
        (uint32)(numPendingLines * sizeof(Line)),
        mNextPendingLine,
        mLines == mNextPendingLine ? SurfaceUpdateFlag::DISCARD : SurfaceUpdateFlag::NO_OVERWRITE );

    mGpu.bindContext( mContext );

    mGpu.draw(
        PrimitiveType::LINE_LIST,
        (uint32)(numPendingLines * 2),       // numvtx
        (uint32)(firstPendingLineOffset * 2) // startvtx,
        );

    // rewind all pointers
    mNextFreeLine = mLines;
    mNextPendingLine = mLines;
}
