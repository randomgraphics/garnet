#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.util");

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
    "   texcoords   = gl_Vertex.xy; \n"
    "}";

const char * glslpscode =
    "uniform vec4 lightpos; // light positin in world space \n"
    "uniform vec4 lightColor; \n"
    "uniform vec4 diffuseColor; \n"
    "uniform sampler2D t0; \n"
    "varying vec4 pos_world; // position in world space \n"
    "varying vec3 nml_world; // normal in world space \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   vec3  L      = normalize( (lightpos - pos_world).xyz ); \n"
    "   vec3  N      = normalize( nml_world ); \n"
    "   float diff   = clamp( dot( L, N ), 0.0, 1.0 ); \n"
    "   vec4  tex    = texture2D( t0, texcoords ); \n"
    "   gl_FragColor = (diff * lightColor + diffuseColor * tex) / 2.0; \n"
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
    ed.uniforms["MATRIX_WORLD"].size = sizeof(Matrix44f);
    ed.uniforms["MATRIX_WORLD_IT"].size = sizeof(Matrix44f); // used to translate normal from local space into world space
    ed.uniforms["LIGHT0_POSITION"].size = sizeof(Vector4f);
    ed.uniforms["LIGHT0_COLOR"].size = sizeof(Vector4f);
    ed.uniforms["DIFFUSE_COLOR"].size = sizeof(Vector4f);
    ed.textures["DIFFUSE_TEXTURE"]; // create a texture parameter named "DIFFUSE_TEXTURE"
    ed.shaders["glsl"].gpd.lang = GPL_GLSL;
    ed.shaders["glsl"].gpd.vs.source = glslvscode;
    ed.shaders["glsl"].gpd.ps.source = glslpscode;
    ed.shaders["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["glsl"].uniforms["world"] = "MATRIX_WORLD";
    ed.shaders["glsl"].uniforms["wit"] = "MATRIX_WORLD_IT";
    ed.shaders["glsl"].uniforms["lightpos"] = "LIGHT0_POSITION";
    ed.shaders["glsl"].uniforms["lightColor"] = "LIGHT0_COLOR";
    ed.shaders["glsl"].uniforms["diffuseColor"] = "DIFFUSE_COLOR";
    ed.shaders["glsl"].textures["t0"] = "DIFFUSE_TEXTURE";
    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";
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
    INIT_UNIFORM( mDiffuseColor  , "DIFFUSE_COLOR"   , Vector4f(1,1,1,1) );

    // setup default texture
    mDiffuseTexture = &mEffect->textures["DIFFUSE_TEXTURE"];
    mDiffuseTexture->set( mDefaultTexture );

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
void GN::util::SimpleDiffuseEffect::setDiffuseColor( const Vector4f & clr )
{
    (*mDiffuseColor)->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleDiffuseEffect::setDiffuseTexture( gfx::Texture * tex )
{
    mDiffuseTexture->set( tex ? tex : mDefaultTexture );
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
