#include "pch.h"
#include "mtidxbuf.h"
#include "mtgpuCmd.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.gpu.mtidxbuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadIdxBuf::init(IdxBuf * ib) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    if (NULL == ib) return failure();

    mIdxBuf = ib;

    const IdxBufDesc & desc = mIdxBuf->getDesc();

    setDesc(desc);

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::quit() {
    GN_GUARD;

    if (mIdxBuf) {
        mGpu.cmdbuf().postCommand1(CMD_IDXBUF_DESTROY, mIdxBuf);
        mIdxBuf = NULL;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from IdxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::update(uint32 startidx, uint32 numidx, const void * data, SurfaceUpdateFlag flag) {
    if (NULL == data) {
        GN_ERROR(sLogger)("Null data pointer.");
        return;
    }

    const IdxBufDesc & d = getDesc();

    // Validate startidx and numidx. Make sure they are in valid range.
    if (startidx >= d.numidx) return;
    if (0 == numidx) numidx = d.numidx - startidx;

    uint32 length = numidx * (d.bits32 ? 4 : 2);

    void * tmpbuf = HeapMemory::alloc(length);
    if (NULL == tmpbuf) {
        GN_ERROR(sLogger)("fail to allocate temporary buffer.");
        return;
    }
    memcpy(tmpbuf, data, length);

    mGpu.cmdbuf().postCommand5(CMD_IDXBUF_UPDATE, mIdxBuf, startidx, numidx, tmpbuf, flag);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::readback(DynaArray<uint8> & data) { mGpu.cmdbuf().postCommand2(CMD_IDXBUF_READBACK, mIdxBuf, &data); }

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN {
namespace gfx {
//
//
// -------------------------------------------------------------------------
void func_IDXBUF_DESTROY(Gpu &, void * p, uint32) {
    IdxBuf * ib = *(IdxBuf **) p;
    ib->decref();
}

//
//
// -------------------------------------------------------------------------
void func_IDXBUF_UPDATE(Gpu &, void * p, uint32) {
    struct IdxBufUpdateParam {
        IdxBuf *          idxbuf;
        uint32            offset;
        uint32            length;
        void *            data;
        SurfaceUpdateFlag flag;
    };

    IdxBufUpdateParam * vbup = (IdxBufUpdateParam *) p;

    vbup->idxbuf->update(vbup->offset, vbup->length, vbup->data, vbup->flag);

    HeapMemory::dealloc(vbup->data);
}

//
//
// -------------------------------------------------------------------------
void func_IDXBUF_READBACK(Gpu &, void * p, uint32) {
    struct IdxBufReadBackParam {
        IdxBuf *           ib;
        DynaArray<uint8> * buf;
    };
    IdxBufReadBackParam * vbrp = (IdxBufReadBackParam *) p;

    vbrp->ib->readback(*vbrp->buf);
}
} // namespace gfx
} // namespace GN
