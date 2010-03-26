#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpures");

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
        md.uniforms[name].initialValue.Resize( sizeof(type) ); \
        type default = (defval); \
        memcpy( md.uniforms[name].initialValue.ToRawPtr(), &default, sizeof(type) ); \
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
bool GN::gfx::SimpleNormalMapModel::Init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SimpleNormalMapModel, () );

    mModel = mDatabase.createResource<ModelResource>( NULL );
    if( 0 == mModel || !mModel->Reset(&DESC) ) return Failure();

    // initialize uniforms
    mMatrixPvw     = mModel->getUniformResource( "MATRIX_PVW"      );
    mMatrixWorld   = mModel->getUniformResource( "MATRIX_WORLD"    );
    mMatrixWorldIT = mModel->getUniformResource( "MATRIX_WORLD_IT" );
    mLightPos      = mModel->getUniformResource( "LIGHT0_POSITION" );
    mLightColor    = mModel->getUniformResource( "LIGHT0_DIFFUSE"  );
    mAlbedoColor   = mModel->getUniformResource( "ALBEDO_COLOR"    );
    mAlbedoTexture = mModel->getTextureResource( "ALBEDO_TEXTURE"  );

    // initialize textures
    mAlbedoTexture = mModel->getTextureResource("ALBEDO_TEXTURE");
    mNormalTexture = mModel->getTextureResource("NORMAL_TEXTURE");

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleNormalMapModel::Quit()
{
    GN_GUARD;

    mModel.Clear();
    mDefaultAlbedoTexture.Clear();
    mDefaultNormalTexture.Clear();
    mMatrixPvw.Clear();
    mMatrixWorld.Clear();
    mMatrixWorldIT.Clear();
    mLightPos.Clear();
    mLightColor.Clear();
    mAlbedoColor.Clear();
    mAlbedoTexture.Clear();
    mNormalTexture.Clear();

    // standard Quit procedure
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
