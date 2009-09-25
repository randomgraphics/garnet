#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

//
//
// -----------------------------------------------------------------------------
static ModelResourceDesc sWireframeModelDesc()
{
    ModelResourceDesc md;
    md.effect = "@WIREFRAME";

#define INIT_UNIFORM( name, type, defval ) \
    if( 1 ) { \
        md.uniforms[name].size = sizeof(type); \
        md.uniforms[name].initialValue.resize( sizeof(type) ); \
        type default = (defval); \
        memcpy( md.uniforms[name].initialValue.cptr(), &default, sizeof(type) ); \
    } else void(0)

    INIT_UNIFORM( "MATRIX_PVW" , Matrix44f, Matrix44f::sIdentity() );
    INIT_UNIFORM( "COLOR"      , Vector4f,  Vector4f(1,1,1,1) );

    return md;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************


const ModelResourceDesc GN::gfx::SimpleWireframeModel::DESC = sWireframeModelDesc();

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::SimpleWireframeModel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::SimpleWireframeModel, () );

    mModel = mDatabase.createResource<ModelResource>( NULL );
    if( 0 == mModel || !mModel->reset(&DESC) ) return failure();

    // initialize uniforms
    mMatrixPvw = mModel->getUniformResource( "MATRIX_PVW" );
    mColor     = mModel->getUniformResource( "COLOR" );

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
