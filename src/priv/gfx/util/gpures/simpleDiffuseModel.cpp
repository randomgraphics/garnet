#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpures");

//
//
// -----------------------------------------------------------------------------
static ModelResourceDesc sDiffuseModelDesc()
{
    ModelResourceDesc md;
    md.effect = "@DIFFUSE";

    md.textures["ALBEDO_TEXTURE"].resourceName = "@WHITE";

#define INIT_UNIFORM( name, type, defval ) \
    if( 1 ) { \
        md.uniforms[name].size = sizeof(type); \
        md.uniforms[name].initialValue.resize( sizeof(type) ); \
        type default = (defval); \
        memcpy( md.uniforms[name].initialValue.GetRawPtr(), &default, sizeof(type) ); \
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

const ModelResourceDesc GN::gfx::SimpleDiffuseModel::DESC = sDiffuseModelDesc();

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SimpleDiffuseModel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SimpleDiffuseModel, () );

    mModel = mDatabase.createResource<ModelResource>( NULL );
    if( 0 == mModel || !mModel->reset(&DESC) ) return failure();

    // initialize uniforms
    mMatrixPvw     = mModel->getUniformResource( "MATRIX_PVW"      );
    mMatrixWorld   = mModel->getUniformResource( "MATRIX_WORLD"    );
    mMatrixWorldIT = mModel->getUniformResource( "MATRIX_WORLD_IT" );
    mLightPos      = mModel->getUniformResource( "LIGHT0_POSITION" );
    mLightColor    = mModel->getUniformResource( "LIGHT0_DIFFUSE"  );
    mAlbedoColor   = mModel->getUniformResource( "ALBEDO_COLOR"    );

    // initialize texture
    mAlbedoTexture = mModel->getTextureResource( "ALBEDO_TEXTURE"  );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleDiffuseModel::quit()
{
    GN_GUARD;

    mModel.clear();
    mDefaultTexture.clear();
    mMatrixPvw.clear();
    mMatrixWorld.clear();
    mMatrixWorldIT.clear();
    mLightPos.clear();
    mLightColor.clear();
    mAlbedoColor.clear();
    mAlbedoTexture.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleDiffuseModel::setTransformation(
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
void GN::gfx::SimpleDiffuseModel::setLightPos( const Vector4f & pos )
{
    mLightPos->getUniform()->update( pos );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleDiffuseModel::setLightColor( const Vector4f & clr )
{
    mLightColor->getUniform()->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleDiffuseModel::setAlbedoColor( const Vector4f & clr )
{
    mAlbedoColor->getUniform()->update( clr );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::SimpleDiffuseModel::setAlbedoTexture( const AutoRef<Texture> & tex )
{
    mAlbedoTexture->setTexture( tex ? tex : mDefaultTexture );
}
