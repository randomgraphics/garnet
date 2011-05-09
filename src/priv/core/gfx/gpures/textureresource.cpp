#include "pch.h"
#include "textureresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

// *****************************************************************************
// GN::gfx::TextureResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::TextureResource::guid()
{
    static const Guid TEXTURE_GUID = {0x6ad8b59d, 0xe672, 0x4b5e, { 0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd } };
    return TEXTURE_GUID;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<TextureResource>
GN::gfx::TextureResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    if( NULL == filename )
    {
        GN_INFO(sLogger)( "Null filename string." );
        return AutoRef<TextureResource>::NULLREF;
    }

    // Reuse existing resource, if possible
    AutoRef<TextureResource> texres( db.findResource<TextureResource>( filename ) );
    if( texres ) return texres;

    // convert to full (absolute) path
    StrA abspath = fs::resolvePath( fs::getCurrentDir(), filename );
    filename = abspath;

    // Try search for existing resource again with full path
    texres = db.findResource<TextureResource>( filename );
    if( texres ) return texres;

    // load new texture from file
    GN_INFO(sLogger)( "Load texture from file: %s", filename );

    // load image
    ImageDesc id;
    DynaArray<uint8> texels;
    if( !loadImageFromFile( id, texels, filename ) ) return AutoRef<TextureResource>::NULLREF;

    // create texture
    TextureDesc td;
    td.fromImageDesc( id );
    AutoRef<Texture> tex( db.getGpu().createTexture( td ) );
    if( !tex ) return AutoRef<TextureResource>::NULLREF;

    // update texture content
    for( uint32 f = 0; f < td.faces; ++f )
    for( uint32 l = 0; l < td.levels; ++l )
    {
        const MipmapDesc & md = id.getMipmap( f, l );
        size_t offset = id.getMipmapOffset( f, l );
        tex->updateMipmap( f, l, 0, md.rowPitch, md.slicePitch, &texels[offset], SurfaceUpdateFlag::DEFAULT );
    }

    // create new texture resource
    texres = db.createResource<TextureResource>( filename );
    if( 0 == texres ) return AutoRef<TextureResource>::NULLREF;

    // attach the texture to the resource
    texres->setTexture( tex );

    // success
    return texres;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::TextureResource::reset( const TextureDesc * desc )
{
    AutoRef<Texture> tex;

    if( desc )
    {
        tex.attach( getGdb().getGpu().createTexture( *desc ) );
        if( !tex ) return false;
    }

    setTexture( tex );

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::TextureResource::setTexture( const AutoRef<Texture> & newTexture )
{
    if( mTexture == newTexture ) return;

    mTexture = newTexture;

    sigTextureChanged(*this);
}

// *****************************************************************************
// GN::gfx::TextureResourceInternal
// *****************************************************************************

class TextureResourceInternal : public TextureResource
{
    //
    //
    // -----------------------------------------------------------------------------
    TextureResourceInternal( GpuResourceDatabase & db )
        : TextureResource( db )
    {
    }

    //
    //
    // -----------------------------------------------------------------------------
    virtual ~TextureResourceInternal()
    {
    }

    //
    //
    // -----------------------------------------------------------------------------
    static GpuResource * sCreateInstance( GpuResourceDatabase & db )
    {
        return new TextureResourceInternal( db );
    }


public:

    //
    //
    // -----------------------------------------------------------------------------
    static bool sRegisterFactory( GpuResourceDatabase & db )
    {
        GpuResourceFactory factory = { &sCreateInstance };

        if( db.hasResourceFactory( TextureResource::guid() ) ) return true;

        return db.registerResourceFactory( TextureResource::guid(), "Texture Resource", factory );
    }
};

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::registerTextureResourceFactory( GpuResourceDatabase & db )
{
    return TextureResourceInternal::sRegisterFactory( db );
}
