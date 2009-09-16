#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

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
bool GN::gfx::SimpleWireframeModel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SimpleWireframeModel, () );

    ModelResourceDesc md;

    md.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);
    md.uniforms["COLOR"].size = sizeof(Vector4f);

    EffectResourceDesc & ed = md.effectResourceDesc;
    ed.uniforms["MATRIX_PVW"];
    ed.uniforms["COLOR"];

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.vs.source = glslvscode;
    ed.gpuprograms["glsl"].gpd.ps.source = glslpscode;
    ed.gpuprograms["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["glsl"].uniforms["color"] = "COLOR";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].gpuprogram = "glsl";

    ed.gpuprograms["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.source = hlslvscode;
    ed.gpuprograms["hlsl"].gpd.vs.entry  = "main";
    ed.gpuprograms["hlsl"].gpd.ps.source = hlslpscode;
    ed.gpuprograms["hlsl"].gpd.ps.entry  = "main";
    ed.gpuprograms["hlsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["hlsl"].uniforms["color"] = "COLOR";
    ed.techniques["hlsl"].passes.resize( 1 );
    ed.techniques["hlsl"].passes[0].gpuprogram = "hlsl";

    mModel = mDatabase.createResource<ModelResource>( NULL );
    if( 0 == mModel || !mModel->reset(&md) ) return failure();

#define INIT_UNIFORM( x, name, defval ) \
    x = mModel->getUniformResource( name ); \
    GN_ASSERT( x ); \
    x->getUniform()->update( defval );

    // initialize uniforms
    INIT_UNIFORM( mMatrixPvw     , "MATRIX_PVW"      , Matrix44f::sIdentity() );
    INIT_UNIFORM( mColor         , "COLOR"           , Vector4f(1,1,1,1) );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleWireframeModel::quit()
{
    GN_GUARD;

    mModel.clear();
    mMatrixPvw.clear();
    mColor.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleWireframeModel::setTransformation(
    const Matrix44f & proj,
    const Matrix44f & view,
    const Matrix44f & world )
{
    Matrix44f pvw = proj * view * world;
    mMatrixPvw->getUniform()->update( pvw );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleWireframeModel::setColor( const Vector4f & clr )
{
    mColor->getUniform()->update( clr );
}
