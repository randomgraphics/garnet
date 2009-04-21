#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.util");

const char * hlslvscode =
    "uniform float4x4 pvw; \n"
    "uniform float4x4 world; \n"
    "uniform float4x4 wit; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 pos_world : POS_WORLD;    // vertex position in world space \n"
    "   float3 nml_world : NORMAL_WORLD; // vertex normal in world space \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "   float3 normal    : NORMAL; \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   o.hpos      = mul( pvw, i.position ); \n"
    "   o.pos_world = mul( world, i.position ); \n"
    "   o.nml_world = mul( wit, float4(i.normal,0) ).xyz; \n"
    "   o.texcoords = i.texcoords; \n"
    "   return o; \n"
    "}";

const char * hlslpscode =
    "uniform float4 lightpos; // light positin in world space \n"
    "uniform float4 lightColor; \n"
    "uniform float4 albedoColor; \n"
    "sampler s0; \n"
    "Texture2D<float4> t0; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 pos_world : POS_WORLD;    // vertex position in world space \n"
    "   float3 nml_world : NORMAL_WORLD; // vertex normal in world space \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "float4 main( in VSOUTPUT i ) : COLOR0 { \n"
    "   float3  L    = normalize( (lightpos - i.pos_world).xyz ); \n"
    "   float3  N    = normalize( i.nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   float4  tex  = t0.Sample( s0, i.texcoords ); \n"
    "   return diff * lightColor * albedoColor * tex; \n"
    "}";

const char * glslvscode =
    "uniform mat4 pvw; \n"
    "uniform mat4 world; \n"
    "uniform mat4 wit; \n"
    "varying vec4 pos_world; // vertex position in world space \n"
    "varying vec3 nml_world; // vertex normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_Position = pvw * gl_Vertex; \n"
    "   pos_world   = world * gl_Vertex; \n"
    "   nml_world   = (wit * vec4(gl_Normal,0)).xyz; \n"
    "   texcoords   = gl_MultiTexCoord0.xy; \n"
    "}";

const char * glslpscode =
    "uniform vec4 lightpos; // light positin in world space \n"
    "uniform vec4 lightColor; \n"
    "uniform vec4 albedoColor; \n"
    "uniform sampler2D t0; \n"
    "varying vec4 pos_world; // position in world space \n"
    "varying vec3 nml_world; // normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   vec3  L      = normalize( (lightpos - pos_world).xyz ); \n"
    "   vec3  N      = normalize( nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   vec4  tex    = texture2D( t0, texcoords ); \n"
    "   gl_FragColor = diff * lightColor * albedoColor * tex; \n"
    "}";

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::util::SimpleDiffuseEffect::init( Renderer & r )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::util::SimpleDiffuseEffect, () );

    // create a pure white 2x2 texture
    mDefaultTexture = r.create2DTexture( 2, 2, 0, ColorFormat::RGBA32 );
    if( NULL == mDefaultTexture ) return failure();
    UInt32 white[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
    mDefaultTexture->updateMipmap( 0, 0, NULL, sizeof(UInt32)*2, sizeof(UInt32)*4, white, SurfaceUpdateFlag::DEFAULT );

    EffectDesc ed;
    ed.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);
    ed.uniforms["MATRIX_WORLD"].size = sizeof(Matrix44f);
    ed.uniforms["MATRIX_WORLD_IT"].size = sizeof(Matrix44f); // used to translate normal from local space into world space
    ed.uniforms["LIGHT0_POSITION"].size = sizeof(Vector4f);
    ed.uniforms["LIGHT0_COLOR"].size = sizeof(Vector4f);
    ed.uniforms["ALBEDO_COLOR"].size = sizeof(Vector4f);
    ed.textures["ALBEDO_TEXTURE"]; // create a texture parameter named "ALBEDO_TEXTURE"

    ed.shaders["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.shaders["glsl"].gpd.vs.source = glslvscode;
    ed.shaders["glsl"].gpd.ps.source = glslpscode;
    ed.shaders["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["glsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.shaders["glsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.shaders["glsl"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.shaders["glsl"].uniforms["lightColor"] = "LIGHT0_COLOR";
    ed.shaders["glsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.shaders["glsl"].textures["t0"] = "ALBEDO_TEXTURE";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";

    ed.shaders["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.shaders["hlsl"].gpd.vs.source = hlslvscode;
    ed.shaders["hlsl"].gpd.vs.entry  = "main";
    ed.shaders["hlsl"].gpd.ps.source = hlslpscode;
    ed.shaders["hlsl"].gpd.ps.entry  = "main";
    ed.shaders["hlsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["hlsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.shaders["hlsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.shaders["hlsl"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.shaders["hlsl"].uniforms["lightColor"] = "LIGHT0_COLOR";
    ed.shaders["hlsl"].uniforms["albedoColor"] = "ALBEDO_COLOR";
    ed.shaders["hlsl"].textures["t0"] = "ALBEDO_TEXTURE";
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
    INIT_UNIFORM( mMatrixWorld   , "MATRIX_WORLD"    , Matrix44f::sIdentity() );
    INIT_UNIFORM( mMatrixWorldIT , "MATRIX_WORLD_IT" , Matrix44f::sIdentity() );
    INIT_UNIFORM( mLightPos      , "LIGHT0_POSITION" , Vector4f(0,0,0,0) );
    INIT_UNIFORM( mLightColor    , "LIGHT0_COLOR"    , Vector4f(1,1,1,1) );
    INIT_UNIFORM( mAlbedoColor   , "ALBEDO_COLOR"    , Vector4f(1,1,1,1) );

    // setup default texture
    mAlbedoTexture = &mEffect->textures["ALBEDO_TEXTURE"];
    mAlbedoTexture->set( mDefaultTexture );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::quit()
{
    GN_GUARD;

    safeDecref( mDefaultTexture );
    safeDelete( mEffect );
    mDrawable.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setTransformation(
    const Matrix44f & proj,
    const Matrix44f & view,
    const Matrix44f & world )
{
    Matrix44f pvw = proj * view * world;
    Matrix44f wit = Matrix44f::sInverse( Matrix44f::sTranspose( world ) );
    (*mMatrixPvw)->update( pvw );
    (*mMatrixWorld)->update( world );
    (*mMatrixWorldIT)->update( wit );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setLightPos( const Vector4f & pos )
{
    (*mLightPos)->update( pos );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setLightColor( const Vector4f & clr )
{
    (*mLightColor)->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setAlbedoColor( const Vector4f & clr )
{
    (*mAlbedoColor)->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setAlbedoTexture( gfx::Texture * tex )
{
    mAlbedoTexture->set( tex ? tex : mDefaultTexture );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setMesh( const gfx::Mesh & mesh, const gfx::MeshSubset * subset )
{
    mesh.applyToDrawable( mDrawable, subset );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::draw()
{
    mEffect->applyToDrawable( mDrawable, 0 );
    mDrawable.draw();
}
