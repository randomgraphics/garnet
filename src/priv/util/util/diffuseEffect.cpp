#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.util");

const char * glslvscode =
    "uniform vec4 lightpos; \n"
    "uniform mat4 pvw; \n"
    "uniform mat4 world; \n"
    "uniform mat4 wit; \n"
    "varying vec3 lightdir; // light direction in world space \n"
    "varying vec3 normal; // normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_Position = pvw * gl_Vertex; \n"
    "   posworld    = world * gl_Vertex; \n"
    "   lightdir    = normalize( lightpos - posworld ); \n"
    "   normal      = (wit * gl_Normal).xyz; \n"
    "   texcoords   = gl_Vertex.xy; \n"
    "}";

const char * glslpscode =
    "uniform vec4 lightColor; \n"
    "uniform sampler2D t0; \n"
    "varying vec3 lightdir; // light direction in world space \n"
    "varying vec3 normal; // normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   float3 L    = normalize( lightdir ); \n"
    "   float3 N    = normalize( normal ); \n"
    "   float  diff = clamp( dot( L, N ), 0, 1 ); \n"
    "   float4 tex  = texture2D( t0, texcoords ); \n"
    "   gl_FragColor = diff * lightColor * tex; \n"
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

    mDefaultTexture = r.create2DTexture( 1, 1, 0, COLOR_FORMAT_RGBA32 );
    if( NULL == mDefaultTexture ) return failure();
    UInt8 white[4] = { 255, 255, 255, 255 };
    mDefaultTexture->updateMipmap( 0, 0, NULL, 4, 4, white, SURFACE_UPDATE_DEFAULT );

    EffectDesc ed;
    ed.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);
    ed.uniforms["MATRIX_WORLD_IT"].size = sizeof(Matrix44f); // used to translate normal from local space into world space
    ed.uniforms["LIGHT0_POSITION"].size = sizeof(Vector4f);
    ed.textures["TEX_DIFFUSE"]; // create a texture parameter named "diffuse"
    ed.shaders["glsl"].gpd.lang = GPL_GLSL;
    ed.shaders["glsl"].gpd.vs.code = glslvscode;
    ed.shaders["glsl"].gpd.ps.code = glslpscode;
    ed.shaders["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["glsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.shaders["glsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.shaders["glsl"].textures["t0"] = "TEXTURE_DIFFUSE";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";
    mEffect = new Effect( r );
    if( !mEffect->init( ed ) ) return failure();

#define INIT_GPP( x, name ) \
    GN_ASSERT( mEffect->hasGpuProgramParam( name ) ); \
    x = mEffect->getGpuProgramParam( name ); \
    GN_ASSERT( x );

    INIT_GPP( mMatrixPvw     , "MATRIX_PVW" );
    INIT_GPP( mMatrixWorld   , "MATRIX_WORLD" );
    INIT_GPP( mMatrixWorldIT , "MATRIX_WORLD_IT" );
    INIT_GPP( mLightPos      , "LIGHT0_POSITION" );
    INIT_GPP( mLightColor    , "LIGHT0_DIRECTION" );

    mTexture = mEffect->getTextureParam( "TEXTURE_DIFFUSE" );
    GN_ASSERT( mTexture );
    mTexture->setTexture( mDefaultTexture );

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
    safeDecref( mMatrixPvw );
    safeDecref( mMatrixWorld );
    safeDecref( mMatrixWorldIT );
    safeDecref( mLightPos );
    safeDecref( mLightColor );
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
    mMatrixPvw->set( pvw );
    mMatrixWorld->set( world );
    mMatrixWorldIT->set( wit );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setLightPos( const Vector4f & pos )
{
    mLightPos->set( pos );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setLightColor( const Vector4f & clr )
{
    mLightColor->set( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setDiffuseTexture( gfx::Texture * tex )
{
    mTexture->setTexture( tex ? tex : mDefaultTexture );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setMesh( gfx::Mesh & mesh, size_t firstidx, size_t numidx )
{
    mesh.applySubsetToDrawable( mDrawable, firstidx, numidx );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::draw()
{
    mEffect->applyToDrawable( mDrawable, 0 );
    mDrawable.draw();
}
