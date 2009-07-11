#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.util");

static const char * hlslvscode =
    "uniform float4x4 pvw; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   o.hpos      = mul( pvw, i.position ); \n"
    "   return o; \n"
    "}";

static const char * hlslpscode =
    "uniform float4 color; \n"
    "float4 main() : COLOR0 { \n"
    "   return color; \n"
    "}";

static const char * glslvscode =
    "uniform mat4 pvw; \n"
    "void main() { \n"
    "   gl_Position = pvw * gl_Vertex; \n"
    "}";

static const char * glslpscode =
    "uniform vec4 color; \n"
    "void main() { \n"
    "   gl_FragColor = color; \n"
    "}";

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::util::SimpleWireframeEffect::init( Renderer & r )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::util::SimpleWireframeEffect, () );

    EffectDesc ed;
    ed.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);
    ed.uniforms["COLOR"].size = sizeof(Vector4f);

    ed.shaders["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.shaders["glsl"].gpd.vs.source = glslvscode;
    ed.shaders["glsl"].gpd.ps.source = glslpscode;
    ed.shaders["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["glsl"].uniforms["color"] = "COLOR";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";

    ed.shaders["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.shaders["hlsl"].gpd.vs.source = hlslvscode;
    ed.shaders["hlsl"].gpd.vs.entry  = "main";
    ed.shaders["hlsl"].gpd.ps.source = hlslpscode;
    ed.shaders["hlsl"].gpd.ps.entry  = "main";
    ed.shaders["hlsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["hlsl"].uniforms["color"] = "COLOR";
    ed.techniques["hlsl"].passes.resize( 1 );
    ed.techniques["hlsl"].passes[0].shader = "hlsl";

    mEffect = new Effect( r );
    if( !mEffect->init( ed ) ) return failure();

#define INIT_UNIFORM( x, name, defval ) \
    GN_ASSERT( mEffect->uniforms.contains( name ) ); \
    x = &mEffect->uniforms[name]; \
    GN_ASSERT( x ); \
    (*x)->update( defval );

    // initialize uniforms
    INIT_UNIFORM( mMatrixPvw     , "MATRIX_PVW"      , Matrix44f::sIdentity() );
    INIT_UNIFORM( mColor         , "COLOR"           , Vector4f(1,1,1,1) );

    // setup render targets
    GN_ASSERT( mEffect->rendertargets.contains( "color0" ) );
    GN_ASSERT( mEffect->rendertargets.contains( "depth" ) );
    mColorTarget = &mEffect->rendertargets["color0"];
    mDepthTarget = &mEffect->rendertargets["depth"];

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWireframeEffect::quit()
{
    GN_GUARD;

    safeDelete( mEffect );
    mDrawable.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWireframeEffect::setTransformation(
    const Matrix44f & proj,
    const Matrix44f & view,
    const Matrix44f & world )
{
    Matrix44f pvw = proj * view * world;
    (*mMatrixPvw)->update( pvw );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWireframeEffect::setColor( const Vector4f & clr )
{
    (*mColor)->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWireframeEffect::setMesh( const gfx::Mesh & mesh, const gfx::MeshSubset * subset )
{
    mesh.applyToDrawable( mDrawable, subset );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWireframeEffect::setRenderTarget(
    const gfx::RenderTargetTexture * color,
    const gfx::RenderTargetTexture * depth )
{
    if( color )
        *mColorTarget = *color;
    else
        mColorTarget->clear();

    if( depth )
        *mDepthTarget = *depth;
    else
        mDepthTarget->clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWireframeEffect::draw()
{
    mEffect->applyToDrawable( mDrawable, 0 );
    mDrawable.draw();
}
