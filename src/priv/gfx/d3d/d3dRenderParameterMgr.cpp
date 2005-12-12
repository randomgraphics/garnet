#include "pch.h"
#include "d3dRenderer.h"

static inline void sSetColorValue( D3DCOLORVALUE & c, float r, float g, float b, float a )
{
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
}

static inline void sSetColorValue( D3DCOLORVALUE & c, const float * v )
{
    c.r = v[0];
    c.g = v[1];
    c.b = v[2];
    c.a = v[3];
}

static inline void sSetD3DVector( D3DVECTOR & v, float x, float y, float z, float w )
{
    float k = 1 / w;
    v.x = x * k;
    v.y = y * k;
    v.z = z * k;
}

static inline void sSetD3DVector( D3DVECTOR & dst, const float * src )
{
    float k = 1.0f / src[3];
    dst.x = src[0] * k;
    dst.y = src[1] * k;
    dst.z = src[2] * k;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::paramDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_WARN( "TODO: setup default parameters!" );

    // success
    return true;

    GN_UNGUARD;
}


// *****************************************************************************
// private management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::applyRenderParameters()
{
    GN_GUARD;

    const std::set<RenderParameterType> & dirtySet = getRpDirtySet();

    GN_ASSERT( !dirtySet.empty() );

    std::set<RenderParameterType>::const_iterator i, e = dirtySet.end();

    for( i = dirtySet.begin(); i != e; ++i )
    {
        const RenderParameterValue & value = getRenderParameter( *i );
        switch( *i )
        {
            case RPT_TRANSFORM_WORLD :
                {
                    GN_ASSERT( RPVT_FLOAT == value.type && 16 == value.count );
                    Matrix44f mat = Matrix44f::transpose( *(const Matrix44f*)value.valueFloats );
                    GN_DX_CHECK( mDevice->SetTransform( D3DTS_WORLD, (const D3DMATRIX*)&mat ) );
                }
                break;

            case RPT_TRANSFORM_VIEW :
                {
                    GN_ASSERT( RPVT_FLOAT == value.type && 16 == value.count );
                    Matrix44f mat = Matrix44f::transpose( *(const Matrix44f*)value.valueFloats );
                    GN_DX_CHECK( mDevice->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&mat ) );
                }
                break;

            case RPT_TRANSFORM_PROJ :
                {
                    GN_ASSERT( RPVT_FLOAT == value.type && 16 == value.count );
                    Matrix44f mat = Matrix44f::transpose( *(const Matrix44f*)value.valueFloats );
                    GN_DX_CHECK( mDevice->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&mat ) );
                }
                break;

            case RPT_TRANSFORM_VIEWPORT :
                {
                    GN_ASSERT( RPVT_FLOAT == value.type && 4 == value.count );

                    // clamp viewport in valid range
                    float l = value.valueFloats[0];
                    float t = value.valueFloats[1];
                    float r = l + value.valueFloats[2];
                    float b = t + value.valueFloats[3];
                    clamp<float>( l, 0.0f, 1.0f );
                    clamp<float>( b, 0.0f, 1.0f );
                    clamp<float>( r, 0.0f, 1.0f );
                    clamp<float>( t, 0.0f, 1.0f );
                    float w = r - l;
                    float h = b - t;

                    // set d3d viewport
                    D3DVIEWPORT9 d3dvp = {
                        (DWORD)l * mCurrentRTSize.x,
                        (DWORD)t * mCurrentRTSize.y,
                        (DWORD)w * mCurrentRTSize.x,
                        (DWORD)h * mCurrentRTSize.y,
                        0.0f, 1.0f,
                    };

                    // update D3D viewport
                    GN_DX_CHECK( mDevice->SetViewport(&d3dvp) );

                    // update D3D scissors
                    RECT rc = {
                        int( d3dvp.X ),
                        int( d3dvp.Y ),
                        int( d3dvp.X+d3dvp.Width ),
                        int( d3dvp.Y+d3dvp.Height ),
                    };
                    GN_DX_CHECK( mDevice->SetScissorRect( &rc ) );
                }
                break;

            case RPT_LIGHT0_POSITION :
            case RPT_LIGHT0_DIFFUSE :
                {
                    GN_ASSERT( RPVT_FLOAT == value.type && 4 == value.count );
                    D3DLIGHT9 d3dlight;
                    d3dlight.Type = D3DLIGHT_POINT;
                    sSetColorValue( d3dlight.Diffuse, getRenderParameter( RPT_LIGHT0_DIFFUSE ).valueFloats );
                    sSetColorValue( d3dlight.Specular, 0, 0, 0, 0 );
                    sSetColorValue( d3dlight.Ambient, 0, 0, 0, 0 );
                    sSetD3DVector( d3dlight.Position, getRenderParameter( RPT_LIGHT0_POSITION ).valueFloats );
                    sSetD3DVector( d3dlight.Direction, 0, 0, 0, 0 );
                    d3dlight.Range = sqrt(FLT_MAX);
                    d3dlight.Falloff = 1.0f;
                    d3dlight.Attenuation0 = 1.0f;
                    d3dlight.Attenuation1 = 0.0f;
                    d3dlight.Attenuation2 = 0.0f;
                    d3dlight.Theta = D3DX_PI;
                    d3dlight.Phi = D3DX_PI;
                    GN_DX_CHECK( mDevice->SetLight( 0, &d3dlight ) );
                }
                break;

            case RPT_MATERIAL_DIFFUSE :
            case RPT_MATERIAL_SPECULAR :
                {
                    GN_ASSERT( RPVT_FLOAT == value.type && 4 == value.count );
                    D3DMATERIAL9 mat;
                    sSetColorValue( mat.Diffuse, getRenderParameter( RPT_MATERIAL_DIFFUSE ).valueFloats );
                    sSetColorValue( mat.Specular, getRenderParameter( RPT_MATERIAL_SPECULAR ).valueFloats );
                    sSetColorValue( mat.Ambient, 0, 0, 0, 0 );
                    sSetColorValue( mat.Emissive, 0, 0, 0, 0 );
                    mat.Power = 1.0f;
                    GN_DX_CHECK( mDevice->SetMaterial( &mat ) );
                }
                break;

            default:
                // program should not reach here.
                GN_UNEXPECTED();
                GNGFX_ERROR( "invalid render parameter type %d", *i );
        }
    }

    // clear dirty set
    clearRpDirtySet();

    GN_UNGUARD;
}
