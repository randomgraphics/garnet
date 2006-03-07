#include "pch.h"
#include "ceguiTexture.h"

#ifdef HAS_CEGUI

int CEGUI::GarnetTexture::msNameCounter = 0;

// *****************************************************************************
// local function
// *****************************************************************************

static bool sCreateTextureFromFile( GN::gfx::Texture * & result, const GN::StrA & name, void * )
{
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    // analyze texture name
    AutoObjPtr<char> filename( new char[name.size()+1] );
    AutoObjPtr<char> group( new char[name.size()+1] );
    memset( filename, 0, name.size()+1 );
    memset( group, 0, name.size()+1 );
    sscanf( name.cstr(), "%s %s", filename.get(), group.get() );

    // load texture file using CEGUI's resource provider
    CEGUI::ResourceProvider * rp = CEGUI::System::getSingleton().getResourceProvider();
    GN_ASSERT( rp );

    CEGUI::RawDataContainer rdc;
    GN_TRY {
        rp->loadRawDataContainer( filename.get(), rdc, group.get() );
    } GN_CATCH( CEGUI::Exception & ) {
        GN_ERROR( "Fail to read resource : name(%s), group(%s)", filename.get(), group.get() );
        return false;
    }

    GN_INFO( "Load GUI texture: name(%s), group(%s).", filename.get(), group.get() );

    // load texture
    MemFile<uint8_t> mf( rdc.getDataPtr(), rdc.getSize(), name );
    Renderer & r = gRenderer;
    result = r.createTextureFromFile( mf );

    // success
    return 0 != result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sCreateMemoryTexture( GN::gfx::Texture * & result, const GN::StrA & name, void * memBuf )
{
    GN_GUARD;

    using namespace GN;
    using namespace GN::gfx;

    Renderer & r = gRenderer;

    // get texture size
    int w, h;
    if( 2 != sscanf( name.cstr(), "CEGUITexture(%d,%d)", &w, &h ) )
    {
        GN_ERROR( "Invalid texture name: %s", name.cstr() );
        return false;
    }

    // create the texture
    AutoRef<Texture> tex( r.create2DTexture( (uint32_t)w, (uint32_t)h, 1, FMT_BGRA32 ) );
    if( tex.empty() ) return false;

    // copy data from memory buffer
    if( memBuf )
    {
        // lock texture
        TexLockedResult tlr;
        if( !tex->lock( tlr, 0, 0, 0, LOCK_DISCARD ) ) return false;

        // copy data
        const uint8_t * src = (const uint8_t*)memBuf;
        size_t srcPitch = (size_t)w * 4;
        GN_ASSERT( tlr.rowBytes >= srcPitch );
        uint8_t * dst = (uint8_t*)tlr.data;
        for( int y = 0; y < h; ++y, src += srcPitch, dst += tlr.rowBytes )
        {
            memcpy( dst, src, srcPitch );
        }

        // unlock texture
        tex->unlock();
    }

    // success
    result = tex.detach();
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetTexture::loadFromFile(const CEGUI::String& filename, const String & group )
{
    GN_GUARD;

    // destroy old texture
    destroy();

    // generate new texture handle
    GN_ASSERT( 0 == mTextureHandle );
    GN::StrA resourecName;
    resourecName.format( "%s %s", filename.c_str(), group.c_str() );
    mTextureHandle = gTexDict.addResource( resourecName, 0, &sCreateTextureFromFile );
    if( 0 == mTextureHandle ) throw RendererException("Fail to get texture handle.");

    // update texture size
    GN::gfx::Texture * tex = gTexDict.getResource(  mTextureHandle );
    if( tex ) tex->getBaseSize( &mWidth, &mHeight );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void CEGUI::GarnetTexture::loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight)
{
    GN_GUARD;

    // destroy old texture
    destroy();

    // store memory buffer
    GN_ASSERT( 0 == mMemBuffer );
    mMemBuffer = GN::memAlloc( buffWidth * buffHeight * 4 );
    if( 0 == mMemBuffer )
    {
        GN_ERROR( "Fail to create in-memory buffer." );
		throw RendererException("Failed to load texture from memory: D3D Texture creation failed.");
    }
    ::memcpy( mMemBuffer, buffPtr, buffWidth * buffHeight * 4 );

    // generate new texture name
    GN::StrA name;
    do {
        name.format( "CEGUITexture(%d,%d)#%d", buffWidth, buffHeight, msNameCounter );
        ++msNameCounter;
    } while( gTexDict.validResourceName( name ) );

    // generate new texture handle
    GN_ASSERT( 0 == mTextureHandle );
    mTextureHandle = gTexDict.addResource( name, mMemBuffer, &sCreateMemoryTexture );
    if( 0 == mTextureHandle ) throw RendererException("Fail to get texture handle.");

    // update texture size
    GN::gfx::Texture * tex = gTexDict.getResource(  mTextureHandle );
    if( tex ) tex->getBaseSize( &mWidth, &mHeight );

    GN_UNGUARD;
}

#endif
