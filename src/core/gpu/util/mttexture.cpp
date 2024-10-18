#include "pch.h"
#include "mttexture.h"
#include "mtgpuCmd.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.gpu.mttexture");

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Local structures
// *****************************************************************************

struct UpdateMipmapParam {
    Texture *         tex;
    uint32_t          face;
    uint32_t          level;
    Box<uint32_t>     area;
    uint32_t          rowPitch;
    uint32_t          slicePitch;
    void *            data;
    SurfaceUpdateFlag flag;
};

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadTexture::init(Texture * tex) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    if (NULL == tex) return failure();

    mTexture = tex;

    const TextureDesc & desc = mTexture->getDesc();

    setDesc(desc);

    for (uint32_t i = 0; i < desc.levels; ++i) { setMipSize(i, mTexture->getMipSize(i)); }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::quit() {
    GN_GUARD;

    if (mTexture) {
        mGpu.cmdbuf().postCommand1(CMD_TEXTURE_DESTROY, mTexture);
        mTexture = NULL;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Texture
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::updateMipmap(uint32_t face, uint32_t level, const Box<uint32_t> * area, uint32_t rowPitch, uint32_t slicePitch,
                                               const void * data, SurfaceUpdateFlag flag) {
    if (level >= getDesc().levels) {
        GN_ERROR(sLogger)("Invalid mipmap level: %d", level);
        return;
    }

    const Vector3<uint32_t> & mipsize = getMipSize(level);

    uint32_t dataSize = slicePitch * mipsize.z;

    void * tmpbuf = HeapMemory::alloc(dataSize);
    if (NULL == tmpbuf) {
        GN_ERROR(sLogger)("fail to allocate temporary data buffer.");
        return;
    }
    memcpy(tmpbuf, data, dataSize);

    UpdateMipmapParam *  ump;
    CommandBuffer::Token token;
    if (CommandBuffer::OPERATION_SUCCEEDED == mGpu.cmdbuf().beginProduce(CMD_TEXTURE_UPDATE_MIPMAP, sizeof(*ump), &token)) {
        ump             = (UpdateMipmapParam *) token.pParameterBuffer;
        ump->tex        = mTexture;
        ump->face       = face;
        ump->level      = level;
        ump->area       = area ? *area : Box<uint32_t>(0, 0, 0, mipsize.x, mipsize.y, mipsize.z);
        ump->rowPitch   = rowPitch;
        ump->slicePitch = slicePitch;
        ump->data       = tmpbuf;
        ump->flag       = flag;

        mGpu.cmdbuf().endProduce();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::readMipmap(uint32_t face, uint32_t level, MipmapData & data) {
    GN_UNUSED_PARAM(face);
    GN_UNUSED_PARAM(level);
    GN_UNUSED_PARAM(data);
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::blobWrite(const void * data, uint32_t length) {
    GN_UNUSED_PARAM(data);
    GN_UNUSED_PARAM(length);
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
uint32_t GN::gfx::MultiThreadTexture::blobRead(void * data) {
    GN_UNUSED_PARAM(data);
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::generateMipmapPyramid() { GN_UNIMPL(); }

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadTexture::getAPIDependentData() const {
    GN_UNIMPL();
    return 0;
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN {
namespace gfx {
//
//
// -------------------------------------------------------------------------
void func_TEXTURE_DESTROY(Gpu &, void * p, uint32_t) {
    Texture ** tex = (Texture **) p;
    (*tex)->decref();
}

//
//
// -------------------------------------------------------------------------
void func_TEXTURE_UPDATE_MIPMAP(Gpu &, void * p, uint32_t) {
    UpdateMipmapParam * ump = (UpdateMipmapParam *) p;

    ump->tex->updateMipmap(ump->face, ump->level, &ump->area, ump->rowPitch, ump->slicePitch, ump->data, ump->flag);

    HeapMemory::dealloc(ump->data);
}

//
//
// -------------------------------------------------------------------------
void func_TEXTURE_READ_MIPMAP(Gpu &, void *, uint32_t) { GN_UNIMPL(); }

//
//
// -------------------------------------------------------------------------
void func_TEXTURE_BLOB_WRITE(Gpu &, void *, uint32_t) { GN_UNIMPL(); }

//
//
// -------------------------------------------------------------------------
void func_TEXTURE_BLOB_READ(Gpu &, void *, uint32_t) { GN_UNIMPL(); }
} // namespace gfx
} // namespace GN
