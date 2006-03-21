#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Texture.h"
#include "garnet/GNd3d.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline DWORD sLockFlag2D3D( DWORD d3dUsage, GN::gfx::LockFlag flag )
{
    DWORD d3dflag = 0;

    GN_UNUSED_PARAM( d3dUsage );
    GN_UNUSED_PARAM( flag );

    /*switch( flag )
    {
        case GN::gfx::LOCK_RW :
            return 0;

        case GN::gfx::LOCK_RO :
            if( 
            break;

        case GN::gfx::LOCK_DISCARD :
        case GN::gfx::LOCK_NO_OVERWRITE :
        default :
            GN_UNEXPECTED();
            GN_ERROR( "invalid lock flag: %d", flag );
            return 0;
    }

    if( GN::gfx::TEXUSAGE_READBACK & usage 

    if( (GN::gfx::LOCK_RO & flag) && !(GN::gfx::LOCK_WO & flag) )
    {
        d3dflag |= D3DLOCK_READONLY;
    }*/

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
static GN::gfx::ClrFmt sGetDefaultDepthTextureFormat( GN::gfx::D3D9Renderer & r )
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
DWORD GN::gfx::texUsage2D3DUsage( BitField usage )
{
    DWORD d3dUsage  = 0;
    
#if GN_XENON
    if( TEXUSAGE_AUTOGEN_MIPMAP & usage )
    {
        GN_WARN( "Xenon does not support mipmap auto-generation!" );
    }
#else

    d3dUsage |= TEXUSAGE_RENDER_TARGET & usage ? D3DUSAGE_RENDERTARGET : 0;
    d3dUsage |= TEXUSAGE_DEPTH & usage ? D3DUSAGE_DEPTHSTENCIL : 0;

    // Note: D3DUSAGE_DYNAMIC can't use with D3DUSAGE_RENDERTARGET and D3DUSAGE_DEPTH.
    if( !(TEXUSAGE_RENDER_TARGET&usage) && !(TEXUSAGE_DEPTH) )
    {
        d3dUsage |= D3DUSAGE_WRITEONLY;
        d3dUsage |= (TEXUSAGE_DYNAMIC & usage) ? D3DUSAGE_DYNAMIC : 0;
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
    if( !setDesc( desc ) ||
        !deviceCreate() ||
        !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::initFromFile( File & file )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9Texture, () );

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

    TextureDesc texDesc;

    // load texture contents
    if( D3DRTYPE_TEXTURE == info.ResourceType )
    {
        LPDIRECT3DTEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateTextureFromFileInMemoryEx(
                dev,
                &buf[0], (UINT)sz,
                D3DX_DEFAULT, D3DX_DEFAULT, // width, height
                D3DX_DEFAULT, // miplevels
                0, // usage
                D3DFMT_UNKNOWN,
                D3DPOOL_DEFAULT,
                D3DX_DEFAULT, D3DX_DEFAULT, // filters
                0, // colorkey
                NULL, // source info
                NULL, // palette
                &tex ),
            quit(); return selfOK(); );

        D3DSURFACE_DESC desc;
        GN_DX_CHECK_DO(
            tex->GetLevelDesc( 0, &desc ),
            quit(); return selfOK(); );

        mD3DTexture = tex;

        // update texture properties
        texDesc.format = d3d::d3dFormat2ClrFmt( desc.Format );
        if( FMT_INVALID == texDesc.format )
        {
            GN_ERROR( "Can't convert D3D format %s to garnet color format.", d3d::d3dFormat2Str( desc.Format ) );
            return false;
        }
        texDesc.type = TEXTYPE_2D;
        texDesc.width = desc.Width;
        texDesc.height = desc.Height;
        texDesc.depth = 1;
        texDesc.faces = 1;
        texDesc.levels = tex->GetLevelCount();
    }
    else if( D3DRTYPE_VOLUMETEXTURE == info.ResourceType )
    {
        LPDIRECT3DVOLUMETEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateVolumeTextureFromFileInMemoryEx(
                dev,
                &buf[0], (UINT)sz,
                D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, // width, height and depth
                D3DX_DEFAULT, // miplevels
                0, // usage
                D3DFMT_UNKNOWN,
                D3DPOOL_DEFAULT,
                D3DX_DEFAULT, D3DX_DEFAULT, // filters
                0, // colorkey
                NULL, // source info
                NULL, // palette
                &tex ),
            quit(); return selfOK(); );

        D3DVOLUME_DESC desc;
        GN_DX_CHECK_DO(
            tex->GetLevelDesc( 0, &desc ),
            quit(); return selfOK(); );

        mD3DTexture = tex;

        // update texture properties
        texDesc.format = d3d::d3dFormat2ClrFmt( desc.Format );
        if( FMT_INVALID == texDesc.format )
        {
            GN_ERROR( "Can't convert D3D format %s to garnet color format.", d3d::d3dFormat2Str( desc.Format ) );
            return false;
        }
        texDesc.type = TEXTYPE_3D;
        texDesc.width = desc.Width;
        texDesc.height = desc.Height;
        texDesc.depth = desc.Depth;
        texDesc.faces = 1;
        texDesc.levels = tex->GetLevelCount();
    }
    else if( D3DRTYPE_CUBETEXTURE == info.ResourceType )
    {
        LPDIRECT3DCUBETEXTURE9 tex;
        GN_DX_CHECK_DO(
            D3DXCreateCubeTextureFromFileInMemoryEx(
                dev, &buf[0], (UINT)sz,
                D3DX_DEFAULT, // width
                D3DX_DEFAULT, // miplevels
                0, // usage
                D3DFMT_UNKNOWN,
                D3DPOOL_DEFAULT,
                D3DX_DEFAULT, D3DX_DEFAULT, // filters
                0, // colorkey
                NULL, // source info
                NULL, // palette
                &tex ),
            quit(); return selfOK(); );

        D3DSURFACE_DESC desc;
        GN_DX_CHECK_DO(
            tex->GetLevelDesc( 0, &desc ),
            quit(); return selfOK(); );

        mD3DTexture = tex;

        // update texture properties
        texDesc.format = d3d::d3dFormat2ClrFmt( desc.Format );
        if( FMT_INVALID == texDesc.format )
        {
            GN_ERROR( "Can't convert D3D format %s to garnet color format.", d3d::d3dFormat2Str( desc.Format ) );
            return false;
        }
        texDesc.type = TEXTYPE_CUBE;
        texDesc.width = desc.Width;
        texDesc.height = desc.Height;
        texDesc.depth = 1;
        texDesc.faces = 6;
        texDesc.levels = tex->GetLevelCount();
    }
    else
    {
        GN_ERROR( "unknown resource type!" );
        quit(); return selfOK();
    }

    // store texture properties
    texDesc.usage = 0;
    if( !setDesc( texDesc ) ) return false;

    // setup other properites
    mD3DUsage = 0;
    mWritable = true;

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::quit()
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
bool GN::gfx::D3D9Texture::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mD3DTexture );

    // determine default format
    if( FMT_DEFAULT == getDesc().format )
    {
        TextureDesc desc = getDesc();
        if( TEXUSAGE_DEPTH & getDesc().usage )
        {
            // find default depth texture format
            desc.format = sGetDefaultDepthTextureFormat( mRenderer );
            if( FMT_INVALID == desc.format ) return false;
            GN_TRACE( "Use default depth texture format: %s", clrFmt2Str(desc.format) );
        }
        else
        {
            desc.format = FMT_BGRA_8_8_8_8_UNORM; // this is default format
            GN_TRACE( "Use default texture format: %s", clrFmt2Str(desc.format) );
        }
        setDesc( desc );
    }

    // determine D3D format
    D3DFORMAT d3dfmt = d3d::clrFmt2D3DFormat( getDesc().format );
    if( D3DFMT_UNKNOWN == d3dfmt )
    {
        GN_ERROR( "Fail to convert color format '%s' to D3DFORMAT.", clrFmt2Str(getDesc().format) );
        return false;
    }

    // determine D3D usage
    mD3DUsage = texUsage2D3DUsage( getDesc().usage );

    // check texture format compatibility
    HRESULT hr = mRenderer.checkD3DDeviceFormat(
        mD3DUsage, texType2D3DResourceType(getDesc().type), d3dfmt );
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

    // create texture instance
    const Vector3<uint32_t> & sz = getBaseSize();
    mD3DTexture = newD3DTexture(
        getDesc().type,
        sz.x, sz.y, sz.z,
        getDesc().levels,
        mD3DUsage,
        d3dfmt,
        D3DPOOL_DEFAULT );
    if( 0 == mD3DTexture ) return false;

    // create shadow copy (Note: Xenon texture has no need of shadow copy)
#if !GN_XENON
    if( TEXUSAGE_READBACK & getDesc().usage )
    {
        mShadowCopy = newD3DTexture(
            getDesc().type,
            sz.x, sz.y, sz.z,
            getDesc().levels,
            0,
            d3dfmt,
            D3DPOOL_SYSTEMMEM );
    }
#endif

    // setup misc. flag
#if GN_XENON
    mWritable = true; // Xenon texture is always writeable.
#else
    mWritable = !(mD3DUsage & D3DUSAGE_RENDERTARGET)
             && !(mD3DUsage & D3DUSAGE_DEPTHSTENCIL)
             &&  (mD3DUsage & D3DUSAGE_DYNAMIC);
#endif

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
void GN::gfx::D3D9Texture::deviceDispose()
{
    GN_GUARD;

    // check if locked
    if( isLocked() )
    {
        GN_WARN( "You are destroying a locked texture!" );
        unlock();
    }

    GN_ASSERT( !mLockCopy );

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
GN::Vector3<uint32_t> GN::gfx::D3D9Texture::getMipSize( size_t level ) const
{
    GN_GUARD_SLOW;

    GN_ASSERT( level < getDesc().levels );

    Vector3<uint32_t> sz;

    if( TEXTYPE_3D == getDesc().type )
    {
        LPDIRECT3DVOLUMETEXTURE9 tex3D = static_cast<LPDIRECT3DVOLUMETEXTURE9>( mD3DTexture );

        D3DVOLUME_DESC desc;
        GN_DX_CHECK( tex3D->GetLevelDesc( (UINT)level, &desc ) );

        sz.x = desc.Width;
        sz.y = desc.Height;
        sz.z = desc.Depth;
    }
    else
    {
        D3DSURFACE_DESC desc;

        if( TEXTYPE_CUBE == getDesc().type )
        {
            LPDIRECT3DCUBETEXTURE9 texCube = static_cast<LPDIRECT3DCUBETEXTURE9>( mD3DTexture );
            GN_DX_CHECK( texCube->GetLevelDesc( (UINT)level, &desc ) );
        }
        else
        {
            LPDIRECT3DTEXTURE9 tex2D = static_cast<LPDIRECT3DTEXTURE9>( mD3DTexture );
            GN_DX_CHECK( tex2D->GetLevelDesc( (UINT)level, &desc ) );
        }

        sz.x = desc.Width;
        sz.y = desc.Height;
        sz.z = 1;
    }

    // success
    return sz;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::setFilter( TexFilter min, TexFilter mag ) const
{
    GN_ASSERT( selfOK() );
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
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(this,&D3D9Texture::basicUnlock) );

#if GN_XENON
    // On Xenon, always lock target texture directly
    mLockedTexture = mD3DTexture;
#else
    if( mShadowCopy ) mLockedTexture = mShadowCopy;
    else if( ( LOCK_RO == flag || LOCK_RW == flag ) || !mWritable )
    {
        // create temporary surface for read-lock of non-shadow texture,
        // or write-lock of non-writable texture.
        GN_ASSERT( !mLockCopy );
        size_t sx, sy, sz;
        getBaseSize( &sx, &sy, &sz );
        mLockCopy = newD3DTexture(
            getDesc().type,
            sx, sy, sz,
            getDesc().levels,
            0,
            d3d::clrFmt2D3DFormat( getDesc().format ),
            D3DPOOL_SYSTEMMEM );
        if( 0 == mLockCopy ) return false;
        mLockedTexture = mLockCopy;
    }
    else mLockedTexture = mD3DTexture;
#endif

    DWORD d3dLockFlag = sLockFlag2D3D( mD3DUsage, flag );

    switch( getDesc().type )
    {
        case TEXTYPE_1D:
        case TEXTYPE_2D:
        {
            RECT rc;
            rc.left = clippedArea.x;
            rc.top = clippedArea.y;
            rc.right = clippedArea.x + clippedArea.w;
            rc.bottom = clippedArea.y + clippedArea.h;

            D3DLOCKED_RECT lrc;
            GN_DX_CHECK_RV( static_cast<LPDIRECT3DTEXTURE9>(mLockedTexture)->LockRect(
                (UINT)level, &lrc, &rc, d3dLockFlag ), false );

            result.rowBytes = lrc.Pitch;
            result.sliceBytes = lrc.Pitch * clippedArea.h;
            result.data = lrc.pBits;
            break;
        }

        case TEXTYPE_3D:
        {
            D3DBOX box;
            box.Left = clippedArea.x;
            box.Top = clippedArea.y;
            box.Front = clippedArea.z;
            box.Right = clippedArea.x + clippedArea.w;
            box.Bottom = clippedArea.y + clippedArea.h;
            box.Back = clippedArea.z + clippedArea.d;

            D3DLOCKED_BOX lb;
            GN_DX_CHECK_RV( static_cast<LPDIRECT3DVOLUMETEXTURE9>(mLockedTexture)->LockBox(
                (UINT)level, &lb, &box, d3dLockFlag ), false );

            result.rowBytes = lb.RowPitch;
            result.sliceBytes = lb.SlicePitch;
            result.data = lb.pBits;
            break;
        }

        case TEXTYPE_CUBE:
        {
            RECT rc;
            rc.left = clippedArea.x;
            rc.top = clippedArea.y;
            rc.right = clippedArea.x + clippedArea.w;
            rc.bottom = clippedArea.y + clippedArea.h;

            GN_ASSERT( face < 6 );

            D3DLOCKED_RECT lrc;
            GN_DX_CHECK_RV( static_cast<LPDIRECT3DCUBETEXTURE9>(mLockedTexture)->LockRect(
                sCubeFace2D3D(face), (UINT)level, &lrc, &rc, d3dLockFlag ), false );

            result.rowBytes = lrc.Pitch;
            result.sliceBytes = lrc.Pitch * clippedArea.h;
            result.data = lrc.pBits;
            break;
        }

        default:
            GN_UNEXPECTED();
            GN_ERROR( "Invalid texture type." );
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

    GN_ASSERT( mLockedTexture );

    // unlock texture
    if( TEXTYPE_1D == getDesc().type || TEXTYPE_2D == getDesc().type )
    {
        GN_DX_CHECK( static_cast<LPDIRECT3DTEXTURE9>(mLockedTexture)->UnlockRect( (UINT)mLockedLevel ) );
    }
    else if( TEXTYPE_3D == getDesc().type )
    {
        GN_DX_CHECK( static_cast<LPDIRECT3DVOLUMETEXTURE9>(mLockedTexture)->UnlockBox( (UINT)mLockedLevel ) );
    }
    else if( TEXTYPE_CUBE == getDesc().type )
    {
        GN_DX_CHECK( static_cast<LPDIRECT3DCUBETEXTURE9>(mLockedTexture)->UnlockRect( sCubeFace2D3D(mLockedFace), (UINT)mLockedLevel ) );
    }

    if( LOCK_RO == mLockedFlag || mLockedTexture == mD3DTexture )
    {
        safeRelease( mLockCopy );
        return;
    }

    // copy data from mLockedTexture to mD3DTexture
#if !GN_XENON
    GN_DX_CHECK( mRenderer.getDevice()->UpdateTexture( mLockedTexture, mD3DTexture ) );
#endif

    //
    // dump texture content
    //
    //static int i = 0;
    //char fname[100];
    //sprintf( fname, "tex_%d.dds", i );
    //GN_DX_CHECK( D3DXSaveTextureToFileA( fname, D3DXIFF_DDS, mD3DTexture, 0 ) );

    // release mLockCopy
    safeRelease( mLockCopy );

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
GN::gfx::D3D9Texture::newD3DTexture( TexType   type,
                                    size_t    width,
                                    size_t    height,
                                    size_t    depth,
                                    size_t    levels,
                                    DWORD     d3dusage,
                                    D3DFORMAT d3dformat,
                                    D3DPOOL   d3dpool )
{
    GN_GUARD;

    LPDIRECT3DDEVICE9 dev = mRenderer.getDevice();

    // make sure texture format is supported by current device
    GN_ASSERT( D3D_OK == mRenderer.checkD3DDeviceFormat(
        d3dusage, texType2D3DResourceType(getDesc().type), d3dformat ) );

#if !GN_XENON
    // evict managed resources first, if creating texture in default pool.
    if( D3DPOOL_DEFAULT == d3dpool )
    {
        GN_DX_CHECK_RV( mRenderer.getDevice()->EvictManagedResources(), 0 );
    }
#endif

    // create new texture
    if( TEXTYPE_1D == type || TEXTYPE_2D == type )
    {
        LPDIRECT3DTEXTURE9 result;
        GN_DX_CHECK_RV(
            dev->CreateTexture(
                (UINT)width, (UINT)height, (UINT)levels,
                d3dusage, d3dformat, d3dpool,
                &result, 0 ),
            0 );
        return result;
    }
    else if( TEXTYPE_3D == type )
    {
        LPDIRECT3DVOLUMETEXTURE9 result;
        GN_DX_CHECK_RV(
            dev->CreateVolumeTexture(
                (UINT)width, (UINT)height, (UINT)depth, (UINT)levels,
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
