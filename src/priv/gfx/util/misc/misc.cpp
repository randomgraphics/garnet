#include "pch.h"

//
//
// -----------------------------------------------------------------------------
GN::gfx::Texture *
GN::gfx::loadTextureFromFile( Renderer & rndr, const char * filename )
{
    // load image
    ImageDesc id;
    std::vector<UInt8> texels;
    if( !loadImageFromFile( id, texels, filename ) ) return 0;

    // create texture
    TextureDesc td;
    td.fromImageDesc( id );
    AutoRef<Texture> tex( rndr.createTexture( td ) );
    if( !tex ) return 0;

    // update texture content
    const MipmapDesc & md = id.getMipmap( 0, 0 );
    tex->updateMipmap( 0, 0, 0, md.rowPitch, md.slicePitch, &texels[0], SURFACE_UPDATE_DEFAULT );

    // success
    return tex.detach();
}
