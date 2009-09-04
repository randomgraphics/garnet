#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

static const char * hlslvscode =
    "uniform float4x4 pvw; \n"
    "uniform float4x4 world; \n"
    "uniform float4x4 wit; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 pos_world : POS_WORLD;    // vertex position in world space \n"
    "   float3 nml_world : NORMAL_WORLD; // vertex normal in world space \n"
    "   float3 tan_world : TANGENT_WORLD; // vertex tangent in world space \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "   float3 normal    : NORMAL; \n"
    "   float3 tangent   : TANGENT; \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   o.hpos      = mul( pvw, i.position ); \n"
    "   o.pos_world = mul( world, i.position ); \n"
    "   o.nml_world = mul( wit, float4(i.normal,0) ).xyz; \n"
    "   o.tan_world = mul( wit, float4(i.tangent,0) ).xyz; \n"
    "   o.texcoords = i.texcoords; \n"
    "   return o; \n"
    "}";

static const char * hlslpscode =
    "uniform float4 lightpos; // light positin in world space \n"
    "uniform float4 lightColor; \n"
    "uniform float4 albedoColor; \n"
    "sampler s0; \n"
    "Texture2D<float4> t0; // albedo texture \n"
    "Texture2D<float2> t1; // normal texture \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float4 pos_world : POS_WORLD;    // vertex position in world space \n"
    "   float3 nml_world : NORMAL_WORLD; // vertex normal in world space \n"
    "   float3 tan_world : TANGENT_WORLD; // vertex tangent in world space \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "float4 main( in VSOUTPUT i ) : COLOR0 { \n"
    "   float3  L    = normalize( (lightpos - i.pos_world).xyz ); \n"
    "   float3  N    = normalize( i.nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   float4  tex  = t0.Sample( s0, i.texcoords ) + t1.Sample( s0, i.texcoords ).xyyy; \n"
    "   return float4( diff, diff, diff, 1.0 ) * lightColor * albedoColor * tex; \n"
    "}";

static const char * glslvscode =
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

static const char * glslpscode =
    "uniform vec4 lightpos; // light positin in world space \n"
    "uniform vec4 lightColor; \n"
    "uniform vec4 albedoColor; \n"
    "uniform sampler2D t0; // albedo texture \n"
    "uniform sampler2D t1; // normal texture \n"
    "varying vec4 pos_world; // position in world space \n"
    "varying vec3 nml_world; // normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   vec3  L      = normalize( (lightpos - pos_world).xyz ); \n"
    "   vec3  N      = normalize( nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   vec4  tex    = texture2D( t0, texcoords ) + texture2D( t1, texcoords ); \n"
    "   gl_FragColor = vec4( diff, diff, diff, 1.0 ) * lightColor * albedoColor * tex; \n"
    "}";

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SimpleNormalMapModel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SimpleNormalMapModel, () );

    // create a white 2x2 texture as default albedo texture
    mDefaultAlbedoTexture.attach( mDatabase.gpu().create2DTexture( 2, 2, 0, ColorFormat::RGBA32 ) );
    if( NULL == mDefaultAlbedoTexture ) return failure();
    UInt32 white[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
    mDefaultAlbedoTexture->updateMipmap( 0, 0, NULL, sizeof(UInt32)*2, sizeof(UInt32)*4, white, SurfaceUpdateFlag::DEFAULT );

    // create default normal texture
    mDefaultNormalTexture.attach(  mDatabase.gpu().create2DTexture( 2, 2, 0, ColorFormat::RG_16_16_UNORM ) );
    if( NULL == mDefaultNormalTexture ) return failure();
    UInt32 up[4] = { 0x80008000, 0x80008000, 0x80008000, 0x80008000 };
    mDefaultNormalTexture->updateMipmap( 0, 0, NULL, sizeof(UInt32)*2, sizeof(UInt32)*4, up, SurfaceUpdateFlag::DEFAULT );

    ModelResourceDesc md;
    md.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);
    md.uniforms["MATRIX_WORLD"].size = sizeof(Matrix44f);
    md.uniforms["MATRIX_WORLD_IT"].size = sizeof(Matrix44f); // used to translate normal from local space into world space
    md.uniforms["LIGHT0_POSITION"].size = sizeof(Vector4f);
    md.uniforms["LIGHT0_COLOR"].size = sizeof(Vector4f);
    md.uniforms["ALBEDO_COLOR"].size = sizeof(Vector4f);

    // TODO: should not creat texture at all.
    md.textures["ALBEDO_TEXTURE"].desc = mDefaultAlbedoTexture->getDesc();
    md.textures["NORMAL_TEXTURE"].desc = mDefaultNormalTexture->getDesc();

    EffectResourceDesc & ed = md.effectResourceDesc;
    ed.uniforms["MATRIX_PVW"];
    ed.uniforms["MATRIX_WORLD"];
    ed.uniforms["MATRIX_WORLD_IT"];
    ed.uniforms["LIGHT0_POSITION"];
    ed.uniforms["LIGHT0_COLOR"];
    ed.uniforms["ALBEDO_COLOR"];
    ed.textures["ALBEDO_TEXTURE"];
    ed.textures["NORMAL_TEXTURE"];

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
    ed.shaders["glsl"].textures["t1"] = "NORMAL_TEXTURE";
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
    ed.shaders["hlsl"].textures["t1"] = "NORMAL_TEXTURE";
    ed.techniques["hlsl"].passes.resize( 1 );
    ed.techniques["hlsl"].passes[0].shader = "hlsl";

    GpuResourceHandle h = mDatabase.createResource( ModelResource::guid(), NULL );
    if( 0 == h ) return failure();
    mModel = GpuResource::castTo<ModelResource>( mDatabase.getResource(h) );
    if( !mModel || !mModel->reset( &md ) ) return failure();

#define INIT_UNIFORM( x, name, defval ) \
    h = mModel->getUniform( name ); \
    GN_ASSERT( h ); \
    x = GpuResource::castTo<UniformResource>( mDatabase.getResource(h) ); \
    GN_ASSERT( x ); \
    x->getUniform()->update( defval );

    // initialize uniforms
    INIT_UNIFORM( mMatrixPvw     , "MATRIX_PVW"      , Matrix44f::sIdentity() );
    INIT_UNIFORM( mMatrixWorld   , "MATRIX_WORLD"    , Matrix44f::sIdentity() );
    INIT_UNIFORM( mMatrixWorldIT , "MATRIX_WORLD_IT" , Matrix44f::sIdentity() );
    INIT_UNIFORM( mLightPos      , "LIGHT0_POSITION" , Vector4f(0,0,0,0) );
    INIT_UNIFORM( mLightColor    , "LIGHT0_COLOR"    , Vector4f(1,1,1,1) );
    INIT_UNIFORM( mAlbedoColor   , "ALBEDO_COLOR"    , Vector4f(1,1,1,1) );

    // setup default texture
    mAlbedoTexture = GpuResource::castTo<TextureResource>( mDatabase.getResource( mModel->getTexture("ALBEDO_TEXTURE") ) );
    mAlbedoTexture->setTexture( mDefaultAlbedoTexture );
    mNormalTexture = GpuResource::castTo<TextureResource>( mDatabase.getResource( mModel->getTexture("NORMAL_TEXTURE") ) );
    mNormalTexture->setTexture( mDefaultNormalTexture );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::quit()
{
    GN_GUARD;

    mDefaultAlbedoTexture.clear();
    mDefaultNormalTexture.clear();

    if( mModel )
    {
        mDatabase.deleteResource( mModel->handle() );
        mModel = 0;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setTransformation(
    const Matrix44f & proj,
    const Matrix44f & view,
    const Matrix44f & world )
{
    Matrix44f pvw = proj * view * world;
    Matrix44f wit = Matrix44f::sInverse( Matrix44f::sTranspose( world ) );
    mMatrixPvw->getUniform()->update( pvw );
    mMatrixWorld->getUniform()->update( world );
    mMatrixWorldIT->getUniform()->update( wit );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setLightPos( const Vector4f & pos )
{
    mLightPos->getUniform()->update( pos );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setLightColor( const Vector4f & clr )
{
    mLightColor->getUniform()->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setAlbedoColor( const Vector4f & clr )
{
    mAlbedoColor->getUniform()->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setAlbedoTexture( const AutoRef<Texture> & tex )
{
    mAlbedoTexture->setTexture( tex ? tex : mDefaultAlbedoTexture );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setNormalTexture( const AutoRef<Texture> & tex )
{
    mNormalTexture->setTexture( tex ? tex : mDefaultNormalTexture );
}
