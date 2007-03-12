#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");
GN::Logger * GN::gfx::D3D9Texture::sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline DWORD sLockFlag2D3D( DWORD d3dUsage, GN::gfx::LockFlag flag )
{
    using namespace GN;
    using namespace GN::gfx;

    DWORD d3dflag;

    if( D3DUSAGE_DYNAMIC & d3dUsage )
    {
        // dynamic texture
        switch( flag )
        {
            case LOCK_RO           : d3dflag = D3DLOCK_READONLY;
            case LOCK_DISCARD      : d3dflag = D3DLOCK_DISCARD;
            case LOCK_NO_OVERWRITE : d3dflag = D3DLOCK_NOOVERWRITE;
            default                : d3dflag = 0;
        }
    }
    else if( ( D3DUSAGE_RENDERTARGET | D3DUSAGE_DEPTHSTENCIL ) & d3dUsage )
    {
        // render target texture
        d3dflag = 0;
    }
    else
    {
        // normal static texture
        switch( flag )
        {
            case LOCK_RO           : d3dflag = D3DLOCK_READONLY;
            default                : d3dflag = 0;
        }
    }

#if GN_DEBUG_BUILD
    d3dflag |= D3DLOCK_NOSYSLOCK;
#endif

    return d3dflag;
}

//
//
// -----------------------------------------------------------------------------
static inline void
sTexFilter2D3D(
    D3DTEXTUREFILTERTYPE & minmag,
    D3DTEXTUREFILTERTYPE * mip,
    GN::gfx::TexFilter f )
{
    switch( f )
    {
        case GN::gfx::TEXFILTER_NEAREST    :
            minmag  = D3DTEXF_POINT;
            if( mip ) *mip = D3DTEXF_NONE;
            break;

        case GN::gfx::TEXFILTER_LINEAR     :
            minmag  = D3DTEXF_LINEAR;
            if( mip ) *mip = D3DTEXF_NONE;
            break;

        case GN::gfx::TEXFILTER_N_MIP_N    :
            minmag  = D3DTEXF_POINT;
            if( mip ) *mip = D3DTEXF_POINT;
            break;

        case GN::gfx::TEXFILTER_N_MIP_L    :
            minmag  = D3DTEXF_POINT;
            if( mip ) *mip = D3DTEXF_LINEAR;
            break;

        case GN::gfx::TEXFILTER_L_MIP_N    :
            minmag  = D3DTEXF_LINEAR;
            if( mip ) *mip = D3DTEXF_POINT;
            break;

        case GN::gfx::TEXFILTER_L_MIP_L    :
            minmag  = D3DTEXF_LINEAR;
            if( mip ) *mip = D3DTEXF_LINEAR;
            break;

        default             :
            GN_ASSERT_EX( 0, "invaid filter type!" );
            minmag  = D3DTEXF_LINEAR;
            if( mip ) *mip = D3DTEXF_NONE;
    }
}

//
//
// -----------------------------------------------------------------------------
static inline D3DTEXTUREADDRESS sTexWrap2D3D( GN::gfx::TexWrap w )
{
    switch( w )
    {
        case GN::gfx::TEXWRAP_REPEAT : return D3DTADDRESS_WRAP;
        case GN::gfx::TEXWRAP_CLAMP  : return D3DTADDRESS_CLAMP;
        case GN::gfx::TEXWRAP_CLAMP_TO_EDGE : return D3DTADDRESS_CLAMP;
        default : GN_ASSERT_EX( 0, "invalid wrap mode!" );
            return D3DTADDRESS_WRAP;
    }
}

//
//
// ----------------------------------------------------------------------------
static D3DFORMAT sGetDefaultDepthTextureFormat( GN::gfx::D3D9Renderer & r )
{
#if GN_XENON

    return GN::gfx::DEFAULT_DEPTH_FORMAT;

#else
    GN_GUARD;

    static D3DFORMAT candidates[] =
    {
        (D3DFORMAT)MAKEFOURCC('D','F','2','4'), (D3DFORMAT)MAKEFOURCC('D','F','1','6'), // for ATI
        D3DFMT_D32, D3DFMT_D24FS8, D3DFMT_D24S8, D3DFMT_D24X8, D3DFMT_D16 // for NVIDIA
    };
    for( size_t i = 0; i < sizeof(candidates)/sizeof(candidates[0]); ++i )
    {
        if( D3D_OK == r.checkD3DDeviceFormat( D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, candidates[i] ) )
        {
            // success
            return candidates[i];
        }
    }

    // failed
    GN_ERROR(sLogger)( "Current renderer does not support depth texture." );
    return D3DFMT_UNKNOWN;

    GN_UNGUARD;
#endif
}

//
//
// ----------------------------------------------------------------------------
static GN::Vector3<UInt32> sGetMipSize( LPDIRECT3DBASETEXTURE9 tex, GN::gfx::TexDim type, size_t level )
{
    GN_GUARD_SLOW;

    using namespace GN;
    using namespace GN::gfx;

    Vector3<UInt32> sz;

    if( TEXDIM_3D == type )
    {
        LPDIRECT3DVOLUMETEXTURE9 tex3D = static_cast<LPDIRECT3DVOLUMETEXTURE9>( tex );

        D3DVOLUME_DESC desc;
        GN_DX9_CHECK( tex3D->GetLevelDesc( (UINT)level, &desc ) );

        sz.x = desc.Width;
        sz.y = desc.Height;
        sz.z = desc.Depth;
    }
    else
    {
        D3DSURFACE_DESC desc;

        if( TEXDIM_CUBE == type )
        {
            LPDIRECT3DCUBETEXTURE9 texCube = static_cast<LPDIRECT3DCUBETEXTURE9>( tex );
            GN_DX9_CHECK( texCube->GetLevelDesc( (UINT)level, &desc ) );
        }
        else
        {
            LPDIRECT3DTEXTURE9 tex2D = static_cast<LPDIRECT3DTEXTURE9>( tex );
            GN_DX9_CHECK( tex2D->GetLevelDesc( (UINT)level, &desc ) );
        }

        sz.x = desc.Width;
        sz.y = desc.Height;
        sz.z = 1;
    }

    // success
    return sz;

    GN_UNGUARD_SLOW;
}

// ****************************************************************************
//  public utils
// ****************************************************************************

///
/// Convert texture type to D3DRESOURCETYPE
///
D3DRESOURCETYPE GN::gfx::texType2D3DResourceType( TexDim type )
{
    switch( type )
    {
        case TEXDIM_1D   :
        case TEXDIM_2D   : return D3DRTYPE_TEXTURE;
        case TEXDIM_3D   : return D3DRTYPE_VOLUMETEXTURE;
        case TEXDIM_CUBE : return D3DRTYPE_CUBETEXTURE;

        default:
            // failed
            GN_ERROR(sLogger)( "invalid texture type : %d", type );
            return (D3DRESOURCETYPE)-1;
    }
}

///
/// Convert texture usage to D3DUSAGE(s)
///
DWORD GN::gfx::texUsage2D3DUsage( BitFields usage )
{
    DWORD d3dUsage  = 0;
    
#if GN_XENON
    if( TEXUSAGE_AUTOGEN_MIPMAP & usage )
    {
        GN_WARN(sLogger)( "Xenon does not support mipmap auto-generation!" );
    }
    d3dUsage |= TEXUSAGE_RENDER_TARGET & usage ? D3DUSAGE_RENDERTARGET : 0;
    d3dUsage |= TEXUSAGE_DEPTH & usage ? D3DUSAGE_RENDERTARGET : 0;
#else

    d3dUsage |= TEXUSAGE_RENDER_TARGET & usage ? D3DUSAGE_RENDERTARGET : 0;
    d3dUsage |= TEXUSAGE_DEPTH & usage ? D3DUSAGE_DEPTHSTENCIL : 0;

    // Note: D3DUSAGE_DYNAMIC can't use with D3DUSAGE_RENDERTARGET and D3DUSAGE_DEPTH.
    if( !(TEXUSAGE_RENDER_TARGET&usage) && !(TEXUSAGE_DEPTH&usage) && (TEXUSAGE_DYNAMIC&usage) )
    {
        d3dUsage |= D3DUSAGE_DYNAMIC;
    }

    d3dUsage |= TEXUSAGE_AUTOGEN_MIPMAP & usage ? D3DUSAGE_AUTOGENMIPMAP : 0;
#endif

    return d3dUsage;
}

// ****************************************************************************
//  init / quit functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::init( const TextureDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9Texture, () );

    // create device data
    if( !setDesc( desc ) || !deviceRestore() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::quit()
{
    GN_GUARD;

    deviceDispose();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
//      interface functions
// ****************************************************************************

//
/// \note During device resetting, we have to recreate all textures, including
///       those in managed pool. Because backbuffer format might be changed.
///       And we have to re-check the compability of texture format.
// ----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mD3DTexture );

#if GN_XENON
    // rendertarget and depth texture requires tiled format.
    if( ( getDesc().usage.rendertarget || getDesc().usage.depthstencil ) && !getDesc().usage.tiled )
    {
        GN_TRACE(sLogger)( "Force tiled texture" );
        TextureDesc modifiedDesc = getDesc();
        modifiedDesc.usage.tiled = true;
        setDesc( modifiedDesc );
    }
#endif

    const TextureDesc & desc = getDesc();

    // determine texture format
    mD3DFormat = D3DFMT_UNKNOWN;
    if( FMT_DEFAULT == desc.format )
    {
        mD3DFormat = desc.usage.depthstencil ? sGetDefaultDepthTextureFormat( getRenderer() ) : D3DFMT_A8R8G8B8;
        if( D3DFMT_UNKNOWN == mD3DFormat ) return false;
        GN_TRACE(sLogger)( "Use default texture format: %s", d3d9::d3dFormat2Str( mD3DFormat ) );
    }
    else
    {
        mD3DFormat = d3d9::clrFmt2D3DFormat( desc.format, desc.usage.tiled );
        if( D3DFMT_UNKNOWN == mD3DFormat )
        {
            GN_ERROR(sLogger)( "Fail to convert color format '%s' to D3DFORMAT.", clrFmt2Str(desc.format) );
            return false;
        }
    }

    // determine D3D usage
    mD3DUsage = texUsage2D3DUsage( desc.usage.u32 );
    if( desc.usage.depthstencil )
    {
        GN_WARN(sLogger)( "depth texture does not support autogen-mipmap" );
        mD3DUsage &= !D3DUSAGE_AUTOGENMIPMAP;
    }

    // check texture format compatibility
    HRESULT hr = getRenderer().checkD3DDeviceFormat(
        mD3DUsage, texType2D3DResourceType(desc.dim), mD3DFormat );
#if !GN_XENON
    if( D3DOK_NOAUTOGEN == hr )
    {
        GN_WARN(sLogger)( "can't generate mipmap automatically!" );
        GN_ASSERT( D3DUSAGE_AUTOGENMIPMAP & mD3DUsage );
        mD3DUsage &= !D3DUSAGE_AUTOGENMIPMAP;
    }
    else
#endif
    GN_DX9_CHECK_RV(hr, false );

    // create texture instance
    mD3DTexture = newD3DTexture(
        desc.dim,
        desc.width, desc.height, desc.depth,
        desc.levels,
        mD3DUsage,
        mD3DFormat,
        D3DPOOL_DEFAULT );
    if( 0 == mD3DTexture ) return false;

    // create shadow copy for both read-back and static textures
    // Note: Xenon texture does not need shadow copy
#if !GN_XENON
    if( desc.usage.readback || !desc.usage.dynamic )
    {
        mShadowCopy = newD3DTexture(
            desc.dim,
            desc.width, desc.height, desc.depth,
            desc.levels,
            0,
            mD3DFormat,
            D3DPOOL_SYSTEMMEM );
    }
#endif

    // setup mip size
    for( size_t i = 0; i < desc.levels; ++i )
    {
        setMipSize( i, sGetMipSize( mD3DTexture, desc.dim, i ) );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::deviceDispose()
{
    GN_GUARD;

    // check if locked
    if( isLocked() )
    {
        GN_WARN(sLogger)( "You are destroying a locked texture!" );
        unlock();
    }

    safeRelease( mD3DTexture );
    safeRelease( mShadowCopy );

    GN_UNGUARD;
}

// ****************************************************************************
//      interface functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::setFilter( TexFilter min, TexFilter mag ) const
{
    GN_ASSERT( ok() );
    sTexFilter2D3D( mD3DFilters[0], &mD3DFilters[2], min );
    sTexFilter2D3D( mD3DFilters[1], 0, mag );
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::setWrap( TexWrap s, TexWrap t, TexWrap r ) const
{
    mD3DWraps[0] = sTexWrap2D3D( s );
    mD3DWraps[1] = sTexWrap2D3D( t );
    mD3DWraps[2] = sTexWrap2D3D( r );
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::lock(
    TexLockedResult & result,
    size_t face,
    size_t level,
    const Boxi * area,
    LockFlag flag )
{
    GN_GUARD_SLOW;

    // call basic lock
    Boxi clippedArea;
    if( !basicLock( face, level, area, flag, clippedArea ) ) return false;
    AutoScope< Delegate0<bool> > basicUnlocker( makeDelegate(this,&D3D9Texture::basicUnlock) );

    // Note: On Xenon, always lock target texture directly
    LPDIRECT3DBASETEXTURE9 lockedtex;
    DWORD lockedUsage;
#if !GN_XENON
    if( mShadowCopy )
    {
        lockedtex = mShadowCopy;
        lockedUsage = 0;
    }
    else
#endif
    {
        lockedtex = mD3DTexture;
        lockedUsage = mD3DUsage;
    }

    // determine lock flag
    DWORD lockedFlag = sLockFlag2D3D( lockedUsage, flag );

    switch( getDesc().dim )
    {
        case TEXDIM_1D:
        case TEXDIM_2D:
        {
            RECT rc;
            rc.left = clippedArea.x;
            rc.top = clippedArea.y;
            rc.right = clippedArea.x + clippedArea.w;
            rc.bottom = clippedArea.y + clippedArea.h;

            D3DLOCKED_RECT lrc;
            GN_DX9_CHECK_RV( static_cast<LPDIRECT3DTEXTURE9>(lockedtex)->LockRect(
                (UINT)level, &lrc, &rc, lockedFlag ), false );

            result.rowBytes = lrc.Pitch;
            result.sliceBytes = lrc.Pitch * clippedArea.h;
            result.data = lrc.pBits;
            break;
        }

        case TEXDIM_3D:
        {
            D3DBOX box;
            box.Left = clippedArea.x;
            box.Top = clippedArea.y;
            box.Front = clippedArea.z;
            box.Right = clippedArea.x + clippedArea.w;
            box.Bottom = clippedArea.y + clippedArea.h;
            box.Back = clippedArea.z + clippedArea.d;

            D3DLOCKED_BOX lb;
            GN_DX9_CHECK_RV( static_cast<LPDIRECT3DVOLUMETEXTURE9>(lockedtex)->LockBox(
                (UINT)level, &lb, &box, lockedFlag ), false );

            result.rowBytes = lb.RowPitch;
            result.sliceBytes = lb.SlicePitch;
            result.data = lb.pBits;
            break;
        }

        case TEXDIM_CUBE:
        {
            RECT rc;
            rc.left = clippedArea.x;
            rc.top = clippedArea.y;
            rc.right = clippedArea.x + clippedArea.w;
            rc.bottom = clippedArea.y + clippedArea.h;

            GN_ASSERT( face < 6 );

            D3DLOCKED_RECT lrc;
            GN_DX9_CHECK_RV( static_cast<LPDIRECT3DCUBETEXTURE9>(lockedtex)->LockRect(
                sCubeFace2D3D(face), (UINT)level, &lrc, &rc, lockedFlag ), false );

            result.rowBytes = lrc.Pitch;
            result.sliceBytes = lrc.Pitch * clippedArea.h;
            result.data = lrc.pBits;
            break;
        }

        default:
            GN_UNEXPECTED();
            GN_ERROR(sLogger)( "Invalid texture type." );
            return false;
    }

    // success
    mLockedFlag = flag;
    mLockedFace = face;
    mLockedLevel = level;
    basicUnlocker.dismiss();
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::unlock()
{
    GN_GUARD_SLOW;

    if( !basicUnlock() ) return;

    // unlock texture
    LPDIRECT3DBASETEXTURE9 lockedtex = mShadowCopy ? mShadowCopy : mD3DTexture;
    if( TEXDIM_1D == getDesc().dim || TEXDIM_2D == getDesc().dim )
    {
        GN_DX9_CHECK( static_cast<LPDIRECT3DTEXTURE9>(lockedtex)->UnlockRect( (UINT)mLockedLevel ) );
    }
    else if( TEXDIM_3D == getDesc().dim )
    {
        GN_DX9_CHECK( static_cast<LPDIRECT3DVOLUMETEXTURE9>(lockedtex)->UnlockBox( (UINT)mLockedLevel ) );
    }
    else if( TEXDIM_CUBE == getDesc().dim )
    {
        GN_DX9_CHECK( static_cast<LPDIRECT3DCUBETEXTURE9>(lockedtex)->UnlockRect( sCubeFace2D3D(mLockedFace), (UINT)mLockedLevel ) );
    }

    // copy data from lockedtex to mD3DTexture
#if GN_XENON
    GN_ASSERT( lockedtex == mD3DTexture );
#else
    if( mShadowCopy && LOCK_RO != mLockedFlag )
    {
        GN_DX9_CHECK( getRenderer().getDevice()->UpdateTexture( mShadowCopy, mD3DTexture ) );
    }
#endif

    //
    // dump texture content
    //
    //static int i = 0;
    //char fname[100];
    //sprintf( fname, "tex_%d.dds", i );
    //GN_DX9_CHECK( D3DXSaveTextureToFileA( fname, D3DXIFF_DDS, mD3DTexture, 0 ) );

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::updateMipmap()
{
    GN_GUARD;

#if !GN_XENON
    if( D3DUSAGE_AUTOGENMIPMAP & mD3DUsage ) return;
#endif

    GN_ASSERT( mD3DTexture );

    GN_DX9_CHECK( D3DXFilterTexture( mD3DTexture, 0, D3DX_DEFAULT, D3DX_DEFAULT) );

    GN_UNGUARD;
}

// ****************************************************************************
//      private functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
LPDIRECT3DBASETEXTURE9
GN::gfx::D3D9Texture::newD3DTexture( TexDim   type,
                                     size_t    width,
                                     size_t    height,
                                     size_t    depth,
                                     size_t    levels,
                                     DWORD     d3dusage,
                                     D3DFORMAT d3dformat,
                                     D3DPOOL   d3dpool )
{
    GN_GUARD;

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // make sure texture format is supported by current device
    GN_ASSERT( D3D_OK == getRenderer().checkD3DDeviceFormat(
        d3dusage, texType2D3DResourceType(getDesc().dim), d3dformat ) );

#if !GN_XENON
    // evict managed resources first, if creating texture in default pool.
    if( D3DPOOL_DEFAULT == d3dpool )
    {
        GN_DX9_CHECK_RV( dev->EvictManagedResources(), 0 );
    }
#endif

    // create new texture
    if( TEXDIM_1D == type || TEXDIM_2D == type )
    {
        LPDIRECT3DTEXTURE9 result;
        GN_DX9_CHECK_RV(
            dev->CreateTexture(
                (UINT)width, (UINT)height, (UINT)levels,
                d3dusage, d3dformat, d3dpool,
                &result, 0 ),
            0 );
        return result;
    }
    else if( TEXDIM_3D == type )
    {
        LPDIRECT3DVOLUMETEXTURE9 result;
        GN_DX9_CHECK_RV(
            dev->CreateVolumeTexture(
                (UINT)width, (UINT)height, (UINT)depth, (UINT)levels,
                d3dusage, d3dformat, d3dpool,
                &result, 0 ),
            0 );
        return result;
    }
    else if( TEXDIM_CUBE == type )
    {
        GN_ASSERT( width == height );
        LPDIRECT3DCUBETEXTURE9 result;
        GN_DX9_CHECK_RV(
            dev->CreateCubeTexture(
                (UINT)width, (UINT)levels,
                d3dusage, d3dformat, d3dpool,
                &result, 0 ),
            0 );
        return result;
    }
    else
    {
        GN_ASSERT_EX( 0, "Program should not reach here!" );
        return 0;
    }

    GN_UNGUARD;
}
