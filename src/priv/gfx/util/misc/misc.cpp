#include "pch.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.util.misc");

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::loadTextureFromFile( Gpu & gpu, const char * filename )
{
    GN_VERBOSE(sLogger)( "Load texture from file: %s", filename );

    // load image
    ImageDesc id;
    DynaArray<UInt8> texels;
    if( !LoadImageFromFile( id, texels, filename ) ) return 0;

    // create texture
    TextureDesc td;
    td.FromImageDesc( id );
    AutoRef<Texture> tex( gpu.createTexture( td ) );
    if( !tex ) return 0;

    // update texture content
    for( size_t f = 0; f < td.faces; ++f )
    for( size_t l = 0; l < td.levels; ++l )
    {
        const MipmapDesc & md = id.GetMipmap( f, l );
        size_t offset = id.GetMipmapOffset( f, l );
        tex->UpdateMipmap( f, l, 0, md.rowPitch, md.slicePitch, &texels[offset], SurfaceUpdateFlag::DEFAULT );
    }

    // success
    return tex.Detach();
}
