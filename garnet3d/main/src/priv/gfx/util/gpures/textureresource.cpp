#include "pch.h"
#include "textureresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

// *****************************************************************************
// GN::gfx::TextureResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::TextureResource::GetGuid()
{
    static const Guid TEXTURE_GUID = {0x6ad8b59d, 0xe672, 0x4b5e, { 0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd } };
    return TEXTURE_GUID;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<TextureResource>
GN::gfx::TextureResource::LoadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    if( NULL == filename )
    {
        GN_INFO(sLogger)( "Null filename string." );
        return AutoRef<TextureResource>::NULLREF;
    }

    // Reuse existing resource, if possible
    AutoRef<TextureResource> texres( db.FindResource<TextureResource>( filename ) );
    if( texres ) return texres;

    // convert to full (absolute) path
    StrA abspath = fs::ResolvePath( fs::GetCurrentDir(), filename );
    filename = abspath;

    // Try search for existing resource again with full path
    texres = db.FindResource<TextureResource>( filename );
    if( texres ) return texres;

    // load new texture from file
    GN_INFO(sLogger)( "Load texture from file: %s", filename );

    // load image
    ImageDesc id;
    DynaArray<UInt8> texels;
    if( !LoadImageFromFile( id, texels, filename ) ) return AutoRef<TextureResource>::NULLREF;

    // create texture
    TextureDesc td;
    td.FromImageDesc( id );
    AutoRef<Texture> tex( db.GetGpu().CreateTexture( td ) );
    if( !tex ) return AutoRef<TextureResource>::NULLREF;

    // update texture content
    for( size_t f = 0; f < td.faces; ++f )
    for( size_t l = 0; l < td.levels; ++l )
    {
        const MipmapDesc & md = id.GetMipmap( f, l );
        size_t offset = id.GetMipmapOffset( f, l );
        tex->UpdateMipmap( f, l, 0, md.rowPitch, md.slicePitch, &texels[offset], SurfaceUpdateFlag::DEFAULT );
    }

    // create new texture resource
    texres = db.CreateResource<TextureResource>( filename );
    if( 0 == texres ) return AutoRef<TextureResource>::NULLREF;

    // attach the texture to the resource
    texres->SetTexture( tex );

    // success
    return texres;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::TextureResource::Reset( const TextureDesc * desc )
{
    AutoRef<Texture> tex;

    if( desc )
    {
        tex.Attach( GetGdb().GetGpu().CreateTexture( *desc ) );
        if( !tex ) return false;
    }

    SetTexture( tex );

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::TextureResource::SetTexture( const AutoRef<Texture> & newTexture )
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

        if( db.HasResourceFactory( TextureResource::GetGuid() ) ) return true;

        return db.RegisterResourceFactory( TextureResource::GetGuid(), "Texture Resource", factory );
    }
};

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::registerTextureResourceFactory( GpuResourceDatabase & db )
{
    return TextureResourceInternal::sRegisterFactory( db );
}
