#include "pch.h"
#include "d3dRenderer.h"

static inline void sSetColorValue( D3DCOLORVALUE & c, float r, float g, float b, float a )
{
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
}

static inline void sSetColorValue( D3DCOLORVALUE & c, const GN::Vector4f & v )
{
    c.r = v.r;
    c.g = v.g;
    c.b = v.b;
    c.a = v.a;
}

static inline void sSetD3DVector( D3DVECTOR & v, float x, float y, float z, float w )
{
    float k = 1 / w;
    v.x = x * k;
    v.y = y * k;
    v.z = z * k;
}

static inline void sSetD3DVector( D3DVECTOR & dst, const GN::Vector4f & src )
{
    float k = 1.0f / src.w;
    dst.x = src.x * k;
    dst.y = src.y * k;
    dst.z = src.z * k;
}

static D3DTEXTURESTAGESTATETYPE sTextureState2D3D[GN::gfx::NUM_TEXTURE_STATES] =
{
    #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 )  d3dname,
    #include "garnet/gfx/textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
};
//
static DWORD sTextureStateValue2D3D[GN::gfx::NUM_TEXTURE_STATE_VALUES] =
{
    #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) d3dval,
    #include "garnet/gfx/textureStateValueMeta.h"
    #undef GNGFX_DEFINE_TSV
};

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::gfx::D3DRenderer::composePerspectiveMatrix(
    Matrix44f & result,
    float fovy,
    float ratio,
    float znear,
    float zfar ) const
{
    return result.perspectiveD3D( fovy, ratio, znear, zfar );
}

//
//
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::gfx::D3DRenderer::composeOrthoMatrix(
    Matrix44f & result,
    float left,
    float bottom,
    float width,
    float height,
    float znear,
    float zfar ) const
{
    return result.orthoD3D(
        left,
        left+width,
        bottom,
        bottom+height,
        znear,
        zfar );
}

// *****************************************************************************
// private management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::applyFfpState()
{
    GN_GUARD;

    // make sure at least one state is dirty
    GN_ASSERT( mFfpDirtyFlags.u32 );

    if( mFfpDirtyFlags.TransformWorld )
    {
        Matrix44f mat = Matrix44f::sTranspose( mTransformWorld.top() );
        GN_DX_CHECK( mDevice->SetTransform( D3DTS_WORLD, (const D3DMATRIX*)&mat ) );
    }

    if( mFfpDirtyFlags.TransformView )
    {
        Matrix44f mat = Matrix44f::sTranspose( mTransformView.top() );
        GN_DX_CHECK( mDevice->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&mat ) );
    }

    if( mFfpDirtyFlags.TransformProj )
    {
        Matrix44f mat = Matrix44f::sTranspose( mTransformProj.top() );
        GN_DX_CHECK( mDevice->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&mat ) );
    }

    if( mFfpDirtyFlags.Viewport )
    {
        // clamp viewport in valid range
        float l = mViewport.top().x;
        float t = mViewport.top().y;
        float r = l + mViewport.top().w;
        float b = t + mViewport.top().h;
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

    if( mFfpDirtyFlags.Light0Diffuse || mFfpDirtyFlags.Light0Pos )
    {
        D3DLIGHT9 d3dlight;
        d3dlight.Type = D3DLIGHT_POINT;
        sSetColorValue( d3dlight.Diffuse, mLight0Diffuse.top() );
        sSetColorValue( d3dlight.Specular, 0, 0, 0, 0 );
        sSetColorValue( d3dlight.Ambient, 0, 0, 0, 0 );
        sSetD3DVector( d3dlight.Position, mLight0Pos.top() );
        sSetD3DVector( d3dlight.Direction, 0, 0, 1, 1 );
        d3dlight.Range = sqrt(FLT_MAX);
        d3dlight.Falloff = 1.0f;
        d3dlight.Attenuation0 = 1.0f;
        d3dlight.Attenuation1 = 0.0f;
        d3dlight.Attenuation2 = 0.0f;
        d3dlight.Theta = D3DX_PI;
        d3dlight.Phi = D3DX_PI;
        GN_DX_CHECK( mDevice->SetLight( 0, &d3dlight ) );
    }

    if( mFfpDirtyFlags.MaterialDiffuse || mFfpDirtyFlags.MaterialSpecular )
    {
        D3DMATERIAL9 mat;
        sSetColorValue( mat.Diffuse, mMaterialDiffuse.top() );
        sSetColorValue( mat.Specular, mMaterialSpecular.top() );
        sSetColorValue( mat.Ambient, 0, 0, 0, 0 );
        sSetColorValue( mat.Emissive, 0, 0, 0, 0 );
        mat.Power = 1.0f;
        GN_DX_CHECK( mDevice->SetMaterial( &mat ) );
    }

    if( mFfpDirtyFlags.TextureStates )
    {
        // apply all TSSs to API
        uint32_t i;
        const TextureStateBlockDesc & desc = mTextureStates.top();
        TextureStateValue tsv;
        DWORD d3dtsv;
        uint32_t numStages = GN::min<uint32_t>( MAX_TEXTURE_STAGES, getCaps( CAPS_MAX_TEXTURE_STAGES ) );
        for ( i = 0; i < numStages; ++i )
        {
            for ( uint32_t j = 0; j < NUM_TEXTURE_STATES; ++j )
            {
                tsv = desc.ts[i][j];
                if( TSV_INVALID != tsv )
                {
                    d3dtsv = sTextureStateValue2D3D[tsv];

                    if( D3DTOP_DOTPRODUCT3 == d3dtsv &&
                        !getD3DCaps( D3DCAPS_DOT3 ) )
                    {
                        GN_DO_ONCE( GN_WARN(
                            "Current D3D device does not support "
                            "dot3 operation! "
                            "Fallback to D3DTOP_SELECTARG1." ) );
                        d3dtsv = D3DTOP_SELECTARG1;
                    }
                    else if( D3DTA_CONSTANT == (d3dtsv&D3DTA_SELECTMASK) &&
                        !getCaps( CAPS_PER_STAGE_CONSTANT ) )
                    {
                        GN_DO_ONCE( GN_WARN(
                                "Current D3D device does not support "
                                "per-stage constant! "
                                "Fallback to D3DTA_TFACTOR." ) );
                        d3dtsv = D3DTA_TFACTOR;
                    }
                    setD3DTextureState( i, sTextureState2D3D[j], d3dtsv );
                }
            }
        }
    }

    // clear dirty flags
    mFfpDirtyFlags.u32 = 0;

    GN_UNGUARD;
}
