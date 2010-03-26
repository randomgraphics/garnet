#include "pch.h"
#include "mttexture.h"
#include "mtgpuCmd.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.util.gpu.mttexture");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Local structures
// *****************************************************************************

struct UpdateMipmapParam
{
    Texture         * tex;
    size_t            face;
    size_t            level;
    Box<UInt32>       area;
    size_t            rowPitch;
    size_t            slicePitch;
    void            * data;
    SurfaceUpdateFlag flag;
};

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadTexture::Init( Texture * tex )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadTexture, () );

    if( NULL == tex ) return Failure();

    mTexture = tex;

    const TextureDesc & desc = mTexture->GetDesc();

    SetDesc( desc );

    for( size_t i = 0; i < desc.levels; ++i )
    {
        SetMipSize( i, mTexture->GetMipSize( i ) );
    }

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::Quit()
{
    GN_GUARD;

    if( mTexture )
    {
        mGpu.postCommand1( CMD_TEXTURE_DESTROY, mTexture );
        mTexture = NULL;
    }

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Texture
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::UpdateMipmap(
    size_t              face,
    size_t              level,
    const Box<UInt32> * area,
    size_t              rowPitch,
    size_t              slicePitch,
    const void        * data,
    SurfaceUpdateFlag   flag )
{
    if( level >= GetDesc().levels )
    {
        GN_ERROR(sLogger)( "Invalid mipmap level: %d", level );
        return;
    }

    const Vector3<UInt32> & mipsize = GetMipSize( level );

    size_t dataSize = slicePitch * mipsize.z;

    void * tmpbuf = HeapMemory::Alloc( dataSize );
    if( NULL == tmpbuf )
    {
        GN_ERROR(sLogger)( "fail to allocate temporary data buffer." );
        return;
    }
    memcpy( tmpbuf, data, dataSize );

    UpdateMipmapParam * ump = (UpdateMipmapParam*)mGpu.beginPostCommand( CMD_TEXTURE_UPDATE_MIPMAP, sizeof(*ump) );
    ump->tex        = mTexture;
    ump->face       = face;
    ump->level      = level;
    ump->area       = area ? *area : Box<UInt32>( 0, 0, 0, mipsize.x, mipsize.y, mipsize.z );
    ump->rowPitch   = rowPitch;
    ump->slicePitch = slicePitch;
    ump->data       = tmpbuf;
    ump->flag       = flag;

    mGpu.endPostCommand();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::ReadMipmap( size_t face, size_t level, MipmapData & data )
{
    GN_UNUSED_PARAM( face );
    GN_UNUSED_PARAM( level );
    GN_UNUSED_PARAM( data );
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::BlobWrite( const void * data, size_t length )
{
    GN_UNUSED_PARAM( data );
    GN_UNUSED_PARAM( length );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::MultiThreadTexture::BlobRead( void * data )
{
    GN_UNUSED_PARAM( data );
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::GenerateMipmapPyramid()
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadTexture::GetAPIDependentData() const
{
    GN_UNIMPL();
    return 0;
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_DESTROY( Gpu &, void * p, size_t )
    {
        Texture ** tex = (Texture**)p;
        (*tex)->DecRef();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_UPDATE_MIPMAP( Gpu &, void * p, size_t )
    {
        UpdateMipmapParam * ump = (UpdateMipmapParam*)p;

        ump->tex->UpdateMipmap(
            ump->face,
            ump->level,
            &ump->area,
            ump->rowPitch,
            ump->slicePitch,
            ump->data,
            ump->flag );

        HeapMemory::Free( ump->data );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_READ_MIPMAP( Gpu &, void *, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_BLOB_WRITE( Gpu &, void *, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_BLOB_READ( Gpu &, void *, size_t )
    {
        GN_UNIMPL();
    }
}}
