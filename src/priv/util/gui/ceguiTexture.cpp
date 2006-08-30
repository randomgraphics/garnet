#include "pch.h"
#include "ceguiTexture.h"

#ifdef HAS_CEGUI

static GN::Logger * sLogger = GN::getLogger("GN.util.gui");

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetTexture::loadFromFile( const CEGUI::String & filename, const CEGUI::String & group )
{
    GN_GUARD;

    mFileName = filename;
    mGroup = group;
    GN::safeHeapFree( mMemBuffer );

    if( !reload() )
    {
        GN_THROW_EX( RendererException( (utf8*)"Failed to create Texture object." ) );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetTexture::loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight)
{
    GN_GUARD;

    GN::safeHeapFree( mMemBuffer );

    // store memory buffer
    GN_ASSERT( 0 == mMemBuffer );
    mMemBuffer = GN::heapAlloc( buffWidth * buffHeight * 4 );
    if( 0 == mMemBuffer )
    {
		GN_THROW_EX( RendererException(
            "Failed to load texture from memory: D3D Texture creation failed." ) );
    }
    ::memcpy( mMemBuffer, buffPtr, buffWidth * buffHeight * 4 );
    mWidth = (ushort)buffWidth;
    mHeight = (ushort)buffHeight;

    // do reload
    if( !reload() )
    {
        GN_THROW_EX( RendererException( (utf8*)"Failed to create Texture object." ) );
    }

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool CEGUI::GarnetTexture::reload()
{
    GN_GUARD;

    using namespace GN;

    gfx::Renderer & r = gRenderer;

    if( mMemBuffer )
    {
        // dispose old texture
        dispose();

        // create the texture
        AutoRef<gfx::Texture> tex( r.create2DTexture( mWidth, mHeight, 1, gfx::FMT_BGRA32 ) );
        if( tex.empty() ) return false;

        // copy data from memory buffer
        // lock texture
        gfx::TexLockedResult tlr;
        if( !tex->lock( tlr, 0, 0, 0, gfx::LOCK_DISCARD ) ) return false;

        // copy data
        const uint8_t * src = (const uint8_t*)mMemBuffer;
        size_t srcPitch = mWidth * 4;
        GN_ASSERT( tlr.rowBytes >= srcPitch );
        uint8_t * dst = (uint8_t*)tlr.data;
        if( srcPitch == tlr.rowBytes )
        {
            memcpy( dst, src, srcPitch * mHeight );
        }
        else for( ushort y = 0; y < mHeight; ++y, src += srcPitch, dst += tlr.rowBytes )
        {
            memcpy( dst, src, srcPitch );
        }

        // unlock texture
        tex->unlock();

        // done
        mGarnetTexture = tex;
    }
    else
    {
        // dispose old texture
        dispose();

        // load texture file using CEGUI's resource provider
        CEGUI::ResourceProvider * rp = CEGUI::System::getSingleton().getResourceProvider();
        GN_ASSERT( rp );

        CEGUI::RawDataContainer rdc;
        rp->loadRawDataContainer( mFileName, rdc, mGroup );

        GN_INFO(sLogger)( "Load GUI texture: name(%s), group(%s).", mFileName.c_str(), mFileName.c_str() );

        // load texture
        MemFile<uint8_t> mf( rdc.getDataPtr(), rdc.getSize(), mFileName.c_str() );
        mGarnetTexture.attach( r.createTextureFromFile( mf ) );
        if( !mGarnetTexture ) return false;

        // update texture size
        mGarnetTexture->getBaseSize( &mWidth, &mHeight );
    }

    // success
    return true;

    GN_UNGUARD;
}

#endif
