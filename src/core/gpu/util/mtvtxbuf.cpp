#include "pch.h"
#include "mtvtxbuf.h"
#include "mtgpuCmd.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.gpu.mtvtxbuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadVtxBuf::init(VtxBuf * vb) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    if (NULL == vb) return failure();

    mVtxBuf = vb;

    const VtxBufDesc & desc = mVtxBuf->getDesc();

    setDesc(desc);

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadVtxBuf::quit() {
    GN_GUARD;

    if (mVtxBuf) {
        mGpu.cmdbuf().postCommand1(CMD_VTXBUF_DESTROY, mVtxBuf);
        mVtxBuf = NULL;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from VtxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadVtxBuf::update(uint32 offset, uint32 length, const void * data, SurfaceUpdateFlag flag) {
    if (NULL == data) {
        GN_ERROR(sLogger)("Null data pointer.");
        return;
    }

    const VtxBufDesc & d = getDesc();

    // Validate offset and length. Make sure they are in valid range.
    if (offset >= d.length) return;
    if (0 == length) length = d.length - offset;

    void * tmpbuf = HeapMemory::alloc(length);
    if (NULL == tmpbuf) {
        GN_ERROR(sLogger)("fail to allocate temporary buffer.");
        return;
    }
    memcpy(tmpbuf, data, length);

    mGpu.cmdbuf().postCommand5(CMD_VTXBUF_UPDATE, mVtxBuf, offset, length, tmpbuf, flag);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadVtxBuf::readback(DynaArray<uint8> & data) { mGpu.cmdbuf().postCommand2(CMD_VTXBUF_READBACK, mVtxBuf, &data); }

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN {
namespace gfx {
//
//
// -------------------------------------------------------------------------
void func_VTXBUF_DESTROY(Gpu &, void * p, uint32) {
    VtxBuf * vb = *(VtxBuf **) p;
    vb->decref();
}

//
//
// -------------------------------------------------------------------------
void func_VTXBUF_UPDATE(Gpu &, void * p, uint32) {
    struct VtxBufUpdateParam {
        VtxBuf *          vtxbuf;
        uint32            offset;
        uint32            length;
        void *            data;
        SurfaceUpdateFlag flag;
    };

    VtxBufUpdateParam * vbup = (VtxBufUpdateParam *) p;

    vbup->vtxbuf->update(vbup->offset, vbup->length, vbup->data, vbup->flag);

    HeapMemory::dealloc(vbup->data);
}

//
//
// -------------------------------------------------------------------------
void func_VTXBUF_READBACK(Gpu &, void * p, uint32) {
    struct VtxBufReadBackParam {
        VtxBuf *           vb;
        DynaArray<uint8> * buf;
    };
    VtxBufReadBackParam * vbrp = (VtxBufReadBackParam *) p;

    vbrp->vb->readback(*vbrp->buf);
}
} // namespace gfx
} // namespace GN
