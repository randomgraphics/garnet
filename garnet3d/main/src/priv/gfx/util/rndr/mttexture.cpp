#include "pch.h"
#include "mttexture.h"
#include "mtrndrCmd.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.rndr.mttexture");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Local structures
// *****************************************************************************

struct UpdateMipmapParam
{
    Texture   * tex;
    size_t      face;
    size_t      level;
    Box<UInt32> area;
    size_t      rowPitch;
    size_t      slicePitch;
    void      * data;
    UpdateFlag  flag;
};

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadTexture::init( Texture * tex )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadTexture, () );

    if( NULL == tex ) return failure();

    mTexture = tex;

    const TextureDesc & desc = mTexture->getDesc();

    setDesc( desc );

    for( size_t i = 0; i < desc.levels; ++i )
    {
        setMipSize( i, mTexture->getMipSize( i ) );
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::quit()
{
    GN_GUARD;

    if( mTexture )
    {
        mRenderer.postCommand1( CMD_TEXTURE_DESTROY, mTexture );
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Texture
// *****************************************************************************

#pragma warning( disable : 4100 )

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::updateMipmap(
    size_t              face,
    size_t              level,
    const Box<UInt32> * area,
    size_t              rowPitch,
    size_t              slicePitch,
    const void        * data,
    UpdateFlag          flag )
{
    if( level >= getDesc().levels )
    {
        GN_ERROR(sLogger)( "Invalid mipmap level: %d", level );
        return;
    }

    const Vector3<UInt32> & mipsize = getMipSize( level );

    size_t dataSize = slicePitch * mipsize.z;

    void * tmpbuf = heapAlloc( dataSize );
    if( NULL == tmpbuf )
    {
        GN_ERROR(sLogger)( "fail to allocate temporary data buffer." );
        return;
    }
    memcpy( tmpbuf, data, dataSize );

    UpdateMipmapParam * ump = (UpdateMipmapParam*)mRenderer.beginPostCommand( CMD_TEXTURE_UPDATE_MIPMAP, sizeof(*ump) );
    ump->tex        = mTexture;
    ump->face       = face;
    ump->level      = level;
    ump->area       = area ? *area : Box<UInt32>( 0, 0, 0, mipsize.x, mipsize.y, mipsize.z );
    ump->rowPitch   = rowPitch;
    ump->slicePitch = slicePitch;
    ump->data       = tmpbuf;
    ump->flag       = flag;

    mRenderer.endPostCommand();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::readMipmap( size_t face, size_t level, MipmapData & data )
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::blobWrite( const void * data, size_t length )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::MultiThreadTexture::blobRead( void * data )
{
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::generateMipmapPyramid()
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadTexture::getAPIDependentData() const
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
    void func_TEXTURE_DESTROY( Renderer &, void * p, size_t )
    {
        Texture ** tex = (Texture**)p;
        (*tex)->decref();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_UPDATE_MIPMAP( Renderer &, void * p, size_t )
    {
        UpdateMipmapParam * ump = (UpdateMipmapParam*)p;

        ump->tex->updateMipmap(
            ump->face,
            ump->level,
            &ump->area,
            ump->rowPitch,
            ump->slicePitch,
            ump->data,
            ump->flag );

        heapFree( ump->data );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_READ_MIPMAP( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_BLOB_WRITE( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_TEXTURE_BLOB_READ( Renderer & r, void * p, size_t )
    {
        GN_UNIMPL();
    }
}}
