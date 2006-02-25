#include "pch.h"
#include "d3dTexture.h"
#include "d3dRenderer.h"
#include "garnet/GNd3d.h"

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
// ----------------------------------------------------------------------------
static GN::gfx::ClrFmt sGetDefaultDepthTextureFormat( GN::gfx::D3DRenderer & r )
{
    GN_GUARD;

    static D3DFORMAT candidates[] = { D3DFMT_D32, D3DFMT_D24S8, D3DFMT_D24X8, D3DFMT_D16 };
    for( size_t i = 0; i < sizeof(candidates)/sizeof(candidates[0]); ++i )
    {
        if( D3D_OK == r.checkD3DDeviceFormat( D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_TEXTURE, candidates[i] ) )
        {
            // success
            GN_ASSERT( GN::gfx::FMT_INVALID != GN::gfx::d3d::d3dFormat2ClrFmt( candidates[i] ) );
            return GN::gfx::d3d::d3dFormat2ClrFmt( candidates[i] );
        }
    }

    // failed
    GN_ERROR( "Current renderer does not support depth texture." );
    return GN::gfx::FMT_INVALID;

    GN_UNGUARD;
}

// ****************************************************************************
//  public utils
// ****************************************************************************

//!
//! Convert texture type to D3DRESOURCETYPE
//!
D3DRESOURCETYPE GN::gfx::texType2D3DResourceType( TexType type )
{
    switch( type )
    {
        case TEXTYPE_1D   :
        case TEXTYPE_2D   : return D3DRTYPE_TEXTURE;
        case TEXTYPE_3D   : return D3DRTYPE_VOLUMETEXTURE;
        case TEXTYPE_CUBE : return D3DRTYPE_CUBETEXTURE;

        default:
            // failed
            GN_ERROR( "invalid texture type : %d", type );
            return (D3DRESOURCETYPE)-1;
    }
}

//!
//! Convert texture usage to D3DUSAGE(s)
//!
uint32_t GN::gfx::texUsage2D3DUsage( uint32_t usage )
{
    uint32_t d3dUsage  = 0;
    
    d3dUsage |= TEXUSAGE_RENDER_TARGET & usage ? D3DUSAGE_RENDERTARGET : 0;
    d3dUsage |= TEXUSAGE_DEPTH & usage ? D3DUSAGE_DEPTHSTENCIL : 0;

#if GN_XENON
    if( TEXUSAGE_AUTOGEN_MIPMAP & usage )
    {
        GN_WARN( "Xenon does not support mipmap auto-generation!" );
    }
#else
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
bool GN::gfx::D3DTexture::init( TexType type,
                                uint32_t sx, uint32_t sy, uint32_t sz,
                                uint32_t levels,
                                ClrFmt format,
                                uint32_t usage )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3DTexture, () );

    // create device data
    if( !setProperties( type, sx, sy, sz, levels, format, usage ) ||
        !deviceCreate() ||
        !deviceRestore() ) { quit(); return selfOK(); }

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

    LPDIRECT3DDEVICE9 dev = mRenderer.getDevice();

    TexType             type;
    Vector3<uint32_t>   size;
    uint32_t            levels;
    ClrFmt              format;
    uint32_t            usage;

    // load texture contents
    if( D3DRTYPE_TEXTURE == info.ResourceType )
    {
        LPDIRECT3DTEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateTextureFromFileInMemory( dev, &buf[0], (UINT)sz, &tex ),
            quit(); return selfOK(); );

        D3DSURFACE_DESC desc;
        GN_DX_CHECK_DO(
            tex->GetLevelDesc( 0, &desc ),
            quit(); return selfOK(); );

        mD3DTexture = tex;

        // update texture properties
        format = d3d::d3dFormat2ClrFmt( desc.Format );
        if( FMT_INVALID == format )
        {
            GN_ERROR( "Can't convert D3D format %s to garnet color format.", d3d::d3dFormat2Str( desc.Format ) );
            return false;
        }
        type = TEXTYPE_2D;
        size[0] = desc.Width;
        size[1] = desc.Height;
        size[2] = 1;
        levels = tex->GetLevelCount();
    }
    else if( D3DRTYPE_VOLUMETEXTURE == info.ResourceType )
    {
        LPDIRECT3DVOLUMETEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateVolumeTextureFromFileInMemory( dev, &buf[0], (UINT)sz, &tex ),
            quit(); return selfOK(); );

        D3DVOLUME_DESC desc;
        GN_DX_CHECK_DO(
            tex->GetLevelDesc( 0, &desc ),
            quit(); return selfOK(); );

        mD3DTexture = tex;

        // update texture properties
        format = d3d::d3dFormat2ClrFmt( desc.Format );
        if( FMT_INVALID == format )
        {
            GN_ERROR( "Can't convert D3D format %s to garnet color format.", d3d::d3dFormat2Str( desc.Format ) );
            return false;
        }
        type = TEXTYPE_3D;
        size[0] = desc.Width;
        size[1] = desc.Height;
        size[2] = desc.Depth;
        levels = tex->GetLevelCount();
    }
    else if( D3DRTYPE_CUBETEXTURE == info.ResourceType )
    {
        LPDIRECT3DCUBETEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateCubeTextureFromFileInMemory( dev, &buf[0], (UINT)sz, &tex ),
            quit(); return selfOK(); );

        D3DSURFACE_DESC desc;
        GN_DX_CHECK_DO(
            tex->GetLevelDesc( 0, &desc ),
            quit(); return selfOK(); );

        mD3DTexture = tex;

        // update texture properties
        format = d3d::d3dFormat2ClrFmt( desc.Format );
        if( FMT_INVALID == format )
        {
            GN_ERROR( "Can't convert D3D format %s to garnet color format.", d3d::d3dFormat2Str( desc.Format ) );
            return false;
        }
        type = TEXTYPE_CUBE;
        size[0] = desc.Width;
        size[1] = desc.Height;
        size[2] = 6;
        levels = tex->GetLevelCount();
    }
    else
    {
        GN_ERROR( "unknown resource type!" );
        quit(); return selfOK();
    }

    // store texture properties
    usage = 0;
    if( !setProperties(
        type,
        size[0], size[1], size[2],
        levels, format, usage ) ) return false;

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
    if( FMT_DEFAULT == getFormat() )
    {
        ClrFmt format;
        if( TEXUSAGE_DEPTH & getUsage() )
        {
            // find default depth texture format
            format = sGetDefaultDepthTextureFormat( mRenderer );
            if( FMT_INVALID == format ) return false;
            GN_TRACE( "Use default depth texture format: %s", clrFmt2Str(format) );
        }
        else
        {
            format = FMT_BGRA_8_8_8_8_UNORM; // this is default format
            GN_TRACE( "Use default texture format: %s", clrFmt2Str(format) );
        }
        setProperties(
            getType(),
            getBaseMapSize().x,
            getBaseMapSize().y,
            getBaseMapSize().z,
            getLevels(),
            format,
            getUsage() );
    }

    // determine D3D format
    D3DFORMAT d3dfmt = d3d::clrFmt2D3DFormat( getFormat() );
    if( D3DFMT_UNKNOWN == d3dfmt )
    {
        GN_ERROR( "Fail to convert color format '%s' to D3DFORMAT.", clrFmt2Str(getFormat()) );
        return false;
    }

    // determine D3D usage & pool
    mD3DUsage = texUsage2D3DUsage( getUsage() );
    D3DPOOL d3dpool =
        ( TEXUSAGE_RENDER_TARGET & getUsage() || TEXUSAGE_DEPTH & getUsage() )
        ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

    // check texture format compatibility
    HRESULT hr = mRenderer.checkD3DDeviceFormat(
        mD3DUsage, texType2D3DResourceType(getType()), d3dfmt );
#if !GN_XENON
    if( D3DOK_NOAUTOGEN == hr )
    {
        GN_WARN( "can't generate mipmap automatically!" );
        GN_ASSERT( D3DUSAGE_AUTOGENMIPMAP & mD3DUsage );
        mD3DUsage &= !D3DUSAGE_AUTOGENMIPMAP;
    }
    else
#endif
    GN_DX_CHECK_RV(hr, false );

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

    // make sure texture format is supported by current device
    GN_ASSERT( D3D_OK == mRenderer.checkD3DDeviceFormat(
        d3dusage, texType2D3DResourceType(getType()), d3dformat ) );

    // create new texture
    if( TEXTYPE_1D == type || TEXTYPE_2D == type )
    {
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

