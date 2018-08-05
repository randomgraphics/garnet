#include "pch.h"

using namespace GN;
using namespace GN::gfx;

//static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

//
//
// -----------------------------------------------------------------------------
static ModelResourceDesc sNormalMapModelDesc()
{
    ModelResourceDesc md;
    md.effect = "@NORMAL_MAP";

    md.textures["ALBEDO_TEXTURE"].resourceName = "@WHITE";
    md.textures["NORMAL_TEXTURE"].resourceName = "@FLAT_NORMAL_MAP";

#define INIT_UNIFORM( name, type, defval ) \
    if( 1 ) { \
        md.uniforms[name].size = sizeof(type); \
        md.uniforms[name].initialValue.resize( sizeof(type) ); \
        type def = (defval); \
        memcpy( md.uniforms[name].initialValue.rawptr(), &def, sizeof(type) ); \
    } else void(0)

    INIT_UNIFORM( "MATRIX_PVW"      , Matrix44f, Matrix44f::sIdentity() );
    INIT_UNIFORM( "MATRIX_WORLD"    , Matrix44f, Matrix44f::sIdentity() );
    INIT_UNIFORM( "MATRIX_WORLD_IT" , Matrix44f, Matrix44f::sIdentity() );
    INIT_UNIFORM( "LIGHT0_POSITION" , Vector4f,  Vector4f(0,0,0,0) );
    INIT_UNIFORM( "LIGHT0_DIFFUSE"  , Vector4f,  Vector4f(1,1,1,1) );
    INIT_UNIFORM( "ALBEDO_COLOR"    , Vector4f,  Vector4f(1,1,1,1) );

    return md;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

const ModelResourceDesc GN::gfx::SimpleNormalMapModel::DESC = sNormalMapModelDesc();

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SimpleNormalMapModel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SimpleNormalMapModel, () );

    mModel = mDatabase.createResource<ModelResource>( NULL );
    if( 0 == mModel || !mModel->reset(&DESC) ) return failure();

    // initialize uniforms
    mMatrixPvw     = mModel->uniformResource( "MATRIX_PVW"      );
    mMatrixWorld   = mModel->uniformResource( "MATRIX_WORLD"    );
    mMatrixWorldIT = mModel->uniformResource( "MATRIX_WORLD_IT" );
    mLightPos      = mModel->uniformResource( "LIGHT0_POSITION" );
    mLightColor    = mModel->uniformResource( "LIGHT0_DIFFUSE"  );
    mAlbedoColor   = mModel->uniformResource( "ALBEDO_COLOR"    );
    mAlbedoTexture = mModel->textureResource( "ALBEDO_TEXTURE"  );

    // initialize textures
    mAlbedoTexture = mModel->textureResource("ALBEDO_TEXTURE");
    mNormalTexture = mModel->textureResource("NORMAL_TEXTURE");

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

    mModel.clear();
    mDefaultAlbedoTexture.clear();
    mDefaultNormalTexture.clear();
    mMatrixPvw.clear();
    mMatrixWorld.clear();
    mMatrixWorldIT.clear();
    mLightPos.clear();
    mLightColor.clear();
    mAlbedoColor.clear();
    mAlbedoTexture.clear();
    mNormalTexture.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setTransform(
    const Matrix44f & proj,
    const Matrix44f & view,
    const Matrix44f & world )
{
    Matrix44f pvw = proj * view * world;
    Matrix44f wit = Matrix44f::sInverse( Matrix44f::sTranspose( world ) );
    mMatrixPvw->uniform()->update( pvw );
    mMatrixWorld->uniform()->update( world );
    mMatrixWorldIT->uniform()->update( wit );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setLightPos( const Vector4f & pos )
{
    mLightPos->uniform()->update( pos );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setLightColor( const Vector4f & clr )
{
    mLightColor->uniform()->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::setAlbedoColor( const Vector4f & clr )
{
    mAlbedoColor->uniform()->update( clr );
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
