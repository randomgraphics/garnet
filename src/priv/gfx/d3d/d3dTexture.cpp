#include "pch.h"
#include "d3dTexture.h"
#include "d3dRenderer.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline bool sAdjustOffsetAndRange( T & offset, T & length, T maxLength )
{
    if( offset >= maxLength )
    {
        GN_ERROR( "offset is beyond the end of valid range." );
        return false;
    }
    if( 0 == length ) length = maxLength;
    if( offset + length > maxLength ) length = maxLength - offset;
    return true;
}

//
//
// -----------------------------------------------------------------------------
static inline uint32_t sLockFlag2D3D( uint32_t flag )
{
    uint32_t d3dflag = 0;

    if( (GN::gfx::LOCK_RO & flag) && !(GN::gfx::LOCK_WO & flag) )
    {
        d3dflag |= D3DLOCK_READONLY;
    }

#if GN_DEBUG
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
// -----------------------------------------------------------------------------
static inline GN::gfx::ClrFmt sD3DFMT2ClrFmt( D3DFORMAT d3dfmt )
{
    // determine texture format
    switch( d3dfmt )
    {
        case D3DFMT_A8R8G8B8     : return GN::gfx::FMT_BGRA_8_8_8_8;
        case D3DFMT_X8R8G8B8     : return GN::gfx::FMT_BGRX_8_8_8_8;
        case D3DFMT_R5G6B5       : return GN::gfx::FMT_BGR_5_6_5;
        case D3DFMT_X1R5G5B5     : return GN::gfx::FMT_BGRX_5_5_5_1;
        case D3DFMT_A1R5G5B5     : return GN::gfx::FMT_BGRA_5_5_5_1;
        case D3DFMT_A4R4G4B4     : return GN::gfx::FMT_BGRA_4_4_4_4;
        //case D3DFMT_R3G3B2       : return GN::gfx::FMT_BGR_2_3_3;
        case D3DFMT_A8           : return GN::gfx::FMT_A_8;
        //case D3DFMT_A8R3G3B2     : return GN::gfx::FMT_BGRA_2_3_3_8;
        //case D3DFMT_X4R4G4B4     : return GN::gfx::FMT_BGRX_4_4_4_4;
        //case D3DFMT_A2B10G10R10  : return GN::gfx::FMT_RGBA_10_10_10_2;
        case D3DFMT_G16R16       : return GN::gfx::FMT_RG_16_16;
        //case D3DFMT_A8P8         : return GN::gfx::FMT_PA_8_8;
        //case D3DFMT_P8           : return GN::gfx::FMT_P_8;
        case D3DFMT_L8           : return GN::gfx::FMT_L_8;
        case D3DFMT_A8L8         : return GN::gfx::FMT_LA_8_8;
        //case D3DFMT_A4L4         : return GN::gfx::FMT_LA_4_4;
        case D3DFMT_V8U8         : return GN::gfx::FMT_UV_8_8;
        //case D3DFMT_L6V5U5       : return GN::gfx::FMT_UVL_5_5_6;
        //case D3DFMT_X8L8V8U8     : return GN::gfx::FMT_UVLX_8_8_8_8;
        //case D3DFMT_Q8W8V8U8     : return GN::gfx::FMT_UVWQ_8_8_8_8;
        case D3DFMT_V16U16       : return GN::gfx::FMT_UV_16_16;
        //case D3DFMT_W11V11U10    : return GN::gfx::FMT_UVW_10_11_11;
        //case D3DFMT_A2W10V10U10  : return GN::gfx::FMT_UVWA_10_10_10_2;
        //case D3DFMT_UYVY         : return GN::gfx::FMT_;
        //case D3DFMT_YUY2         : return GN::gfx::FMT_;
        case D3DFMT_DXT1         : return GN::gfx::FMT_DXT1;
        case D3DFMT_DXT2         : return GN::gfx::FMT_DXT2;
        case D3DFMT_DXT4         : return GN::gfx::FMT_DXT4;
#if !GN_XENON
        case D3DFMT_DXT3         : return GN::gfx::FMT_DXT3;
        case D3DFMT_DXT5         : return GN::gfx::FMT_DXT5;
        case D3DFMT_D16_LOCKABLE : return GN::gfx::FMT_D_16;
#endif
        case D3DFMT_D32          : return GN::gfx::FMT_D_32;
        //case D3DFMT_D15S1        : return GN::gfx::FMT_DS_15_1;
        case D3DFMT_D24S8        : return GN::gfx::FMT_DS_24_8;
        case D3DFMT_D16          : return GN::gfx::FMT_D_16;
        case D3DFMT_D24X8        : return GN::gfx::FMT_DX_24_8;
        //case D3DFMT_D24X4S4      : return GN::gfx::FMT_DXS_24_4_4;
        default                  :
            // invalid format, failed
            GN_ERROR( "Unsupported or invalid D3DFORMAT: '%s'!", GN::gfx::D3DFORMAT2Str(d3dfmt) );
            return GN::gfx::FMT_INVALID;
    }
}

static inline D3DFORMAT sClrFmt2D3DFMT( GN::gfx::ClrFmt clrfmt )
{
    // determine texture format
    switch( clrfmt )
    {
        case GN::gfx::FMT_BGRA_8_8_8_8  : return D3DFMT_A8R8G8B8;
        case GN::gfx::FMT_BGRA_5_5_5_1  : return D3DFMT_A1R5G5B5;
        case GN::gfx::FMT_BGR_5_6_5     : return D3DFMT_R5G6B5;
        case GN::gfx::FMT_LA_8_8        : return D3DFMT_A8L8;
        case GN::gfx::FMT_UV_8_8        : return D3DFMT_V8U8;
        case GN::gfx::FMT_L_8           : return D3DFMT_L8;
        case GN::gfx::FMT_A_8           : return D3DFMT_A8;
        case GN::gfx::FMT_D_16          : return D3DFMT_D16;
        case GN::gfx::FMT_DX_24_8       : return D3DFMT_D24X8;
        case GN::gfx::FMT_DS_24_8       : return D3DFMT_D24S8;
        case GN::gfx::FMT_D_32          : return D3DFMT_D32;
        case GN::gfx::FMT_DXT1          : return D3DFMT_DXT1;
        case GN::gfx::FMT_DXT2          : return D3DFMT_DXT2;
        default                       :
            // invalid format, failed
            GN_ERROR( "Unsupported or invalid color format: '%s'!", GN::gfx::getClrFmtDesc(clrfmt).name );
            return D3DFMT_UNKNOWN;
    }
}

// ****************************************************************************
//  init / quit functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3DTexture::init( TexType type,
                                uint32_t sx, uint32_t sy, uint32_t sz,
                                uint32_t levels,
                                ClrFmt format,
                                uint32_t usage )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3DTexture, () );

    mInitType = type;
    mInitSize[0] = sx;
    mInitSize[1] = sy;
    mInitSize[2] = sz;
    mInitLevels = levels;
    mInitFormat = format;
    mInitUsage = usage;

    // create device data
    if( !deviceCreate() || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3DTexture::initFromFile( File & file )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3DTexture, () );

    GN_ASSERT( !mD3DTexture );

    // check for empty file
    if( 0 == file.size() )
    {
        GN_ERROR( "empty file!" );
        quit(); return selfOK();
    }

    // read file contents
    std::vector<uint8_t> buf;
    buf.resize( file.size() );
    size_t sz = file.read( &buf[0], buf.size() );
    if( size_t(-1) == sz )
    { quit(); return selfOK(); }

    // get image info.
    D3DXIMAGE_INFO info;
    GN_DX_CHECK_DO(
        D3DXGetImageInfoFromFileInMemory( &buf[0], (UINT)sz, &info ),
        quit(); return selfOK(); );

    // set initialize parameters
    mInitFormat = sD3DFMT2ClrFmt( info.Format );
    if( (TexType)-1 == mInitType || FMT_INVALID == mInitFormat )
    {
        quit();
        return selfOK();
    }
    mInitSize[0] = info.Width;
    mInitSize[1] = info.Height;
    mInitSize[2] = info.Depth;
    mInitLevels = info.MipLevels;
    mInitUsage = 0;

    LPDIRECT3DDEVICE9 dev = mRenderer.getDevice();

    // load texture contents
    if( D3DRTYPE_TEXTURE == info.ResourceType )
    {
        LPDIRECT3DTEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateTextureFromFileInMemory( dev, &buf[0], (UINT)sz, &tex ),
            quit(); return selfOK(); );
        mInitType = TEXTYPE_2D;
        mD3DTexture = tex;
    }
    else if( D3DRTYPE_VOLUMETEXTURE == info.ResourceType )
    {
        LPDIRECT3DVOLUMETEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateVolumeTextureFromFileInMemory( dev, &buf[0], (UINT)sz, &tex ),
            quit(); return selfOK(); );
        mInitType = TEXTYPE_3D;
        mD3DTexture = tex;
    }
    else if( D3DRTYPE_CUBETEXTURE == info.ResourceType )
    {
        LPDIRECT3DCUBETEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateCubeTextureFromFileInMemory( dev, &buf[0], (UINT)sz, &tex ),
            quit(); return selfOK(); );
        mInitType = TEXTYPE_CUBE;
        mD3DTexture = tex;
    }
    else
    {
        GN_ERROR( "unknown resource type!" );
        quit(); return selfOK();
    }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DTexture::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
//      interface functions
// ****************************************************************************

//
//! \note During device resetting, we have to recreate all textures, including
//!       those in managed pool. Because backbuffer format might be changed.
//!       And we have re-check the compability of texture format.
// ----------------------------------------------------------------------------
bool GN::gfx::D3DTexture::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mD3DTexture );

    // determine default format
    ClrFmt format;
    if( FMT_DEFAULT == mInitFormat )
    {
        if( TEXUSAGE_DEPTH & mInitUsage )
        {
            format = sD3DFMT2ClrFmt(
                mRenderer.getPresentParameters().AutoDepthStencilFormat );
            if( FMT_INVALID == format )
            {
                GN_ERROR( "unsupport depth-stencil format!" );
                return false;
            }
        }
        else
        {
            format = FMT_BGRA_8_8_8_8; // this is default format
        }
    }
    else
    {
        format = mInitFormat;
    }

    // store texture properties
    if( !setProperties(
        mInitType,
        mInitSize[0],mInitSize[1],mInitSize[2],
        mInitLevels, format, mInitUsage) ) return false;

    // determine D3D format
    D3DFORMAT d3dfmt = sClrFmt2D3DFMT( format );
    if( D3DFMT_UNKNOWN == d3dfmt ) return false;

    // determine D3D usage & pool
    mD3DUsage = 0;
    mD3DUsage |= TEXUSAGE_RENDER_TARGET & getUsage() ? D3DUSAGE_RENDERTARGET : 0;
#if GN_XENON
    if( TEXUSAGE_AUTOGEN_MIPMAP & getUsage() )
    {
        GN_WARN( "Xenon does not support mipmap auto-generation!" );
    }
#else
    mD3DUsage |= TEXUSAGE_AUTOGEN_MIPMAP & getUsage() ? D3DUSAGE_AUTOGENMIPMAP : 0;
#endif
    mD3DUsage |= TEXUSAGE_DEPTH & getUsage() ? D3DUSAGE_DEPTHSTENCIL : 0;
    D3DPOOL d3dpool =
        ( TEXUSAGE_RENDER_TARGET & getUsage() || TEXUSAGE_DEPTH & getUsage() )
        ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

    // check texture format compatibility
    HRESULT hr = mRenderer.getD3D()->CheckDeviceFormat(
        mRenderer.getAdapter(),
        mRenderer.getDeviceType(),
        mRenderer.getPresentParameters().BackBufferFormat,
        mD3DUsage, D3DRTYPE_TEXTURE, d3dfmt );
#if !GN_XENON
    if( D3DOK_NOAUTOGEN == hr )
    {
        GN_WARN( "can't generate mipmap automatically!" );
        GN_ASSERT( D3DUSAGE_AUTOGENMIPMAP & mD3DUsage );
        mD3DUsage &= !D3DUSAGE_AUTOGENMIPMAP;
    }
    else
#endif
    {
        GN_DX_CHECK_RV(hr, false );
    }

#if !GN_XENON
    // evict managed resources first, if creating texture in default pool.
    if( D3DPOOL_DEFAULT == d3dpool )
    {
        GN_DX_CHECK_RV( mRenderer.getDevice()->EvictManagedResources(), 0 );
    }
#endif

    // create texture instance in default pool
    uint32_t sx, sy, sz;
    getBaseMapSize( &sx, &sy, &sz );
    mD3DTexture = newD3DTexture(
        getType(),
        sx, sy, sz,
        getLevels(),
        mD3DUsage,
        d3dfmt,
        d3dpool );
    if( 0 == mD3DTexture ) return false;

    // call user-defined content loader
    if( !getLoader().empty() )
    {
        if( !getLoader()( *this ) ) return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DTexture::deviceDispose()
{
    GN_GUARD;

    // check if locked
    if( isLocked() )
    {
        GN_WARN( "You are destroying a locked texture!" );
        unlock();
    }

    safeRelease( mD3DTexture );

    GN_UNGUARD;
}

// ****************************************************************************
//      interface functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DTexture::getMipMapSize(
    uint32_t level, uint32_t * sx, uint32_t * sy, uint32_t * sz ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( level < getLevels() );

    if( TEXTYPE_3D == getType() )
    {
        LPDIRECT3DVOLUMETEXTURE9 tex3D = static_cast<LPDIRECT3DVOLUMETEXTURE9>( mD3DTexture );

        D3DVOLUME_DESC desc;
        GN_DX_CHECK( tex3D->GetLevelDesc( level, &desc ) );

        if( sx ) *sx = desc.Width;
        if( sy ) *sy = desc.Height;
        if( sz ) *sz = desc.Depth;
    }
    else
    {
        D3DSURFACE_DESC desc;

        if( TEXTYPE_CUBE == getType() )
        {
            LPDIRECT3DCUBETEXTURE9 texCube = static_cast<LPDIRECT3DCUBETEXTURE9>( mD3DTexture );
            GN_DX_CHECK( texCube->GetLevelDesc( level, &desc ) );
        }
        else
        {
            LPDIRECT3DTEXTURE9 tex2D = static_cast<LPDIRECT3DTEXTURE9>( mD3DTexture );
            GN_DX_CHECK( tex2D->GetLevelDesc( level, &desc ) );
        }

        if( sx ) *sx = desc.Width;
        if( sy ) *sy = desc.Height;
        if( sz ) *sz = 1;
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DTexture::setFilter( TexFilter min, TexFilter mag ) const
{
    GN_ASSERT( selfOK() );
    sTexFilter2D3D( mD3DFilters[0], &mD3DFilters[2], min );
    sTexFilter2D3D( mD3DFilters[1], 0, mag );
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DTexture::setWrap( TexWrap s, TexWrap t, TexWrap r ) const
{
    mD3DWraps[0] = sTexWrap2D3D( s );
    mD3DWraps[1] = sTexWrap2D3D( t );
    mD3DWraps[2] = sTexWrap2D3D( r );
}

//
//
// ----------------------------------------------------------------------------
void * GN::gfx::D3DTexture::lock1D( uint32_t level,
                                    uint32_t offset,
                                    uint32_t length,
                                    uint32_t flag )
{
    GN_GUARD_SLOW;

    if ( TEXTYPE_1D != getType() )
    {
        GN_ERROR( "can't do 1D lock on an non-1D texture!" );
        return false;
    }

    // call basic lock
    if( !basicLock() ) return false;
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(this,&D3DTexture::basicUnlock) );

    // adjust offset and length
    uint32_t sx;
    getMipMapSize( level, &sx, 0, 0 );
    if( !sAdjustOffsetAndRange( offset, length, sx ) ) return 0;

    RECT d3drc;
    d3drc.left = offset;
    d3drc.top = 0;
    d3drc.right = offset + length;
    d3drc.bottom = 1;

    // lock texture
    D3DLOCKED_RECT d3dlr;
    LPDIRECT3DTEXTURE9 p = static_cast<LPDIRECT3DTEXTURE9>(mD3DTexture);
    GN_DX_CHECK_RV( p->LockRect( level, &d3dlr, &d3drc, sLockFlag2D3D(flag) ), 0 );

    // success
    mLockedLevel = level;
    basicUnlocker.dismiss();
    return d3dlr.pBits;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3DTexture::lock2D( LockedRect &  result,
                                  uint32_t      level,
                                  const Recti * area,
                                  uint32_t      flag )
{
    GN_GUARD_SLOW;

    if ( TEXTYPE_2D != getType() )
    {
        GN_ERROR( "can't do 2D lock on an non-2D texture!" );
        return false;
    }

    // call basic lock
    if( !basicLock() ) return false;
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(this,&D3DTexture::basicUnlock) );

    // get surface size
    uint32_t sx, sy;
    getMipMapSize( level, &sx, &sy, 0 );

    RECT d3drc;
    if( area )
    {
        Recti rc( *area );
        
        if( !sAdjustOffsetAndRange( rc.x, rc.w, (int)sx ) ||
            !sAdjustOffsetAndRange( rc.y, rc.h, (int)sy ) )
            return 0;

        d3drc.left   = rc.x;
        d3drc.top    = rc.y;
        d3drc.right  = d3drc.left + rc.w;
        d3drc.bottom = d3drc.top + rc.h;
    }
    else
    {
        d3drc.left = d3drc.top = 0;
        d3drc.right = sx;
        d3drc.bottom = sy;
    }

    // lock texture
    D3DLOCKED_RECT d3dlr;
    LPDIRECT3DTEXTURE9 p = static_cast<LPDIRECT3DTEXTURE9>(mD3DTexture);
    GN_DX_CHECK_RV( p->LockRect( level, &d3dlr, &d3drc, sLockFlag2D3D(flag) ), false );

    // success
    mLockedLevel = level;
    result.rowBytes = d3dlr.Pitch;
    result.data = d3dlr.pBits;
    basicUnlocker.dismiss();
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3DTexture::lock3D( LockedBox &  result,
                                  uint32_t     level,
                                  const Boxi * box,
                                  uint32_t     flag )
{
    GN_GUARD_SLOW;

    if ( TEXTYPE_3D != getType() )
    {
        GN_ERROR( "can't do 3D lock on an non-3D texture!" );
        return false;
    }

    // call basic lock
    if( !basicLock() ) return 0;
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(this,&D3DTexture::basicUnlock) );

    // get surface size
    uint32_t sx, sy, sz;
    getMipMapSize( level, &sx, &sy, &sz );

    // construct D3DBOX
    D3DBOX d3dbox;
    if ( box )
    {
        Boxi b( *box );

        if( !sAdjustOffsetAndRange( b.x, b.w, (int)sx ) ||
            !sAdjustOffsetAndRange( b.y, b.h, (int)sy ) ||
            !sAdjustOffsetAndRange( b.z, b.d, (int)sz ) )
            return 0;
        
        d3dbox.Left   = (uint32_t)b.x;
        d3dbox.Top    = (uint32_t)b.y;
        d3dbox.Front  = (uint32_t)b.z;
        d3dbox.Right  = (uint32_t)( b.x + b.w );
        d3dbox.Bottom = (uint32_t)( b.y + b.h );
        d3dbox.Back   = (uint32_t)( b.z + b.d );
    }
    else
    {
        d3dbox.Left   = 0;
        d3dbox.Top    = 0;
        d3dbox.Front  = 0;
        d3dbox.Right  = sx;
        d3dbox.Bottom = sy;
        d3dbox.Back   = sz;
    }

    // lock texture
    D3DLOCKED_BOX d3dlb;
    LPDIRECT3DVOLUMETEXTURE9 p = static_cast<LPDIRECT3DVOLUMETEXTURE9>(mD3DTexture);
    GN_DX_CHECK_RV( p->LockBox( level, &d3dlb, &d3dbox, sLockFlag2D3D(flag) ), false );

    // success
    mLockedLevel = level;
    result.data = d3dlb.pBits;
    result.rowBytes = d3dlb.RowPitch;
    result.sliceBytes = d3dlb.SlicePitch;
    basicUnlocker.dismiss();
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3DTexture::lockCube( LockedRect &  result,
                                    TexFace       face,
                                    uint32_t      level,
                                    const Recti * area,
                                    uint32_t      flag )
{
    GN_GUARD_SLOW;

    if ( TEXTYPE_CUBE != getType() )
    {
        GN_ERROR( "can't do CUBE lock on an non-CUBE texture!" );
        return false;
    }
    // call basic lock
    if( !basicLock() ) return 0;
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(this,&D3DTexture::basicUnlock) );

    // get surface size
    uint32_t sx;
    getMipMapSize( level, &sx, 0, 0 );

    // convert aera to RECT
    RECT d3drc;
    if( area )
    {
        Recti rc( *area );
        
        if( !sAdjustOffsetAndRange( rc.x, rc.w, (int)sx ) ||
            !sAdjustOffsetAndRange( rc.y, rc.h, (int)sx  ) )
            return 0;

        d3drc.left   = rc.x;
        d3drc.top    = rc.y;
        d3drc.right  = d3drc.left + rc.w;
        d3drc.bottom = d3drc.top + rc.h;
        d3drc.bottom = d3drc.top + area->h;
    }
    else
    {
        d3drc.left = 0;
        d3drc.top = 0;
        d3drc.right = sx;
        d3drc.bottom = sx;
    }

    // lock texture
    D3DLOCKED_RECT d3dlr;
    LPDIRECT3DCUBETEXTURE9 p = static_cast<LPDIRECT3DCUBETEXTURE9>(mD3DTexture);
    GN_DX_CHECK_RV(
        p->LockRect( sTexFace2D3D(face), level, &d3dlr, &d3drc, sLockFlag2D3D(flag) ),
        false );

    // success
    mLockedFace = sTexFace2D3D(face);
    mLockedLevel = level;
    result.rowBytes = d3dlr.Pitch;
    result.data = d3dlr.pBits;
    basicUnlocker.dismiss();
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DTexture::unlock()
{
    GN_GUARD_SLOW;

    if( !basicUnlock() ) return;

    // unlock texture
    if( TEXTYPE_1D == getType() || TEXTYPE_2D == getType() )
    {
        GN_DX_CHECK( static_cast<LPDIRECT3DTEXTURE9>(mD3DTexture)->UnlockRect( mLockedLevel ) );
    }
    else if( TEXTYPE_3D == getType() )
    {
        GN_DX_CHECK( static_cast<LPDIRECT3DVOLUMETEXTURE9>(mD3DTexture)->UnlockBox( mLockedLevel ) );
    }
    else if( TEXTYPE_CUBE == getType() )
    {
        GN_DX_CHECK( static_cast<LPDIRECT3DCUBETEXTURE9>(mD3DTexture)->UnlockRect( mLockedFace, mLockedLevel ) );
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DTexture::updateMipmap()
{
    GN_GUARD;

#if !GN_XENON
    if( D3DUSAGE_AUTOGENMIPMAP & mD3DUsage ) return;
#endif

    GN_ASSERT( mD3DTexture );

    GN_DX_CHECK( D3DXFilterTexture( mD3DTexture, 0, D3DX_DEFAULT, D3DX_DEFAULT) );

    GN_UNGUARD;
}

// ****************************************************************************
//      private functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
LPDIRECT3DBASETEXTURE9
GN::gfx::D3DTexture::newD3DTexture( TexType   type,
                                    uint32_t  width,
                                    uint32_t  height,
                                    uint32_t  depth,
                                    uint32_t  levels,
                                    uint32_t  d3dusage,
                                    D3DFORMAT d3dformat,
                                    D3DPOOL   d3dpool )
{
    GN_GUARD;

    LPDIRECT3DDEVICE9 dev = mRenderer.getDevice();

    // create new texture
    if( TEXTYPE_1D == type || TEXTYPE_2D == type )
    {
        if( D3DERR_NOTAVAILABLE == mRenderer.getD3D()->CheckDeviceFormat(
            mRenderer.getAdapter(),
            mRenderer.getDeviceType(),
            mRenderer.getPresentParameters().BackBufferFormat,
            d3dusage,
            D3DRTYPE_TEXTURE,
            d3dformat ) )
        {
            GN_ERROR( "unsupported texture format!" );
            return 0;
        }

        LPDIRECT3DTEXTURE9 result;
        GN_DX_CHECK_RV(
            dev->CreateTexture( width, height, levels,
                d3dusage, d3dformat, d3dpool,
                &result, 0 ),
            0 );
        return result;
    }
    else if( TEXTYPE_3D == type )
    {
        if( D3DERR_NOTAVAILABLE == mRenderer.getD3D()->CheckDeviceFormat(
            mRenderer.getAdapter(),
            mRenderer.getDeviceType(),
            mRenderer.getPresentParameters().BackBufferFormat,
            d3dusage,
            D3DRTYPE_VOLUMETEXTURE,
            d3dformat ) )
        {
            GN_ERROR( "unsupported texture format!" );
            return 0;
        }

        LPDIRECT3DVOLUMETEXTURE9 result;
        GN_DX_CHECK_RV(
            dev->CreateVolumeTexture( width, height, depth, levels,
                d3dusage, d3dformat, d3dpool,
                &result, 0 ),
            0 );
        return result;
    }
    else if( TEXTYPE_CUBE == type )
    {
        if( D3DERR_NOTAVAILABLE == mRenderer.getD3D()->CheckDeviceFormat(
            mRenderer.getAdapter(),
            mRenderer.getDeviceType(),
            mRenderer.getPresentParameters().BackBufferFormat,
            d3dusage,
            D3DRTYPE_CUBETEXTURE,
            d3dformat ) )
        {
            GN_ERROR( "unsupported texture format!" );
            return 0;
        }

        GN_ASSERT( width == height );
        LPDIRECT3DCUBETEXTURE9 result;
        GN_DX_CHECK_RV(
            dev->CreateCubeTexture( width, levels,
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

