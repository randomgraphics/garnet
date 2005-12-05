#include "pch.h"
#include "d3dRenderer.h"

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

    std::set<RenderParameterType>::const_iterator i = dirtySet.begin();

    while( i != dirtySet.end() )
    {
        const RenderParameterValue & value = getRenderParameter( *i );
        switch( *i )
        {
            case RPT_LIGHT0_POSITION :
            case RPT_LIGHT0_DIFFUSE :
            case RPT_MATERIAL_DIFFUSE :
            case RPT_MATERIAL_SPECULAR :
                GN_ASSERT( RPVT_FLOAT == value.type && 4 == value.count );
                GN_WARN( "not support %s yet.", rpt2Str(*i) );
                break;

            case RPT_TRANSFORM_WORLD :
            case RPT_TRANSFORM_VIEW :
            case RPT_TRANSFORM_PROJ :
                GN_ASSERT( RPVT_FLOAT == value.type && 16 == value.count );
                GN_WARN( "not support %s yet.", rpt2Str(*i) );
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
