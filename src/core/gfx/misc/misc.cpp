#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.misc");

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::Texture * GN::gfx::loadTextureFromFile(Gpu & gpu, const char * filename) {
    GN_VERBOSE(sLogger)("Load texture from file: %s", filename);

    // load image
    auto image = Image::load(filename);
    if (image.empty()) return 0;

    // create texture
    TextureDesc td;
    td.fromImageDesc(image.desc());
    AutoRef<Texture> tex = attachTo(gpu.createTexture(td));
    if (!tex) return 0;

    // update texture content
    for (uint32 f = 0; f < td.faces; ++f)
        for (uint32 l = 0; l < td.levels; ++l) {
            auto & md = image.plane(f, l);
            tex->updateMipmap(f, l, 0, md.pitch, md.slice, image.data() + md.offset, SurfaceUpdateFlag::DEFAULT);
        }

    // success
    return tex.detach();
}
