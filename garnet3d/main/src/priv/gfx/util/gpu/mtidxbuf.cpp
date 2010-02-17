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
bool GN::gfx::MultiThreadIdxBuf::init( IdxBuf * ib )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadIdxBuf, () );

    if( NULL == ib ) return failure();

    mIdxBuf = ib;

    const IdxBufDesc & desc = mIdxBuf->getDesc();

    setDesc( desc );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::quit()
{
    GN_GUARD;

    if( mIdxBuf )
    {
        mGpu.postCommand1( CMD_IDXBUF_DESTROY, mIdxBuf );
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
void GN::gfx::MultiThreadIdxBuf::update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag )
{
    if( 0 == length )
    {
        const IdxBufDesc & desc = getDesc();
        size_t maxlen = desc.numidx * ( desc.bits32 ? 4 : 2 );
        length = maxlen - offset;
    }

    void * tmpbuf = HeapAlloc( length );
    if( NULL == tmpbuf )
    {
        GN_ERROR(sLogger)( "fail to allocate temporary buffer." );
        return;
    }
    memcpy( tmpbuf, data, length );

    mGpu.postCommand5( CMD_IDXBUF_UPDATE, mIdxBuf, offset, length, tmpbuf, flag );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::readback( std::vector<UInt8> & data )
{
    mGpu.postCommand2( CMD_IDXBUF_READBACK, mIdxBuf, &data );
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    void func_IDXBUF_DESTROY( Gpu &, void * p, size_t )
    {
        IdxBuf * ib = *(IdxBuf**)p;
        ib->decref();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_IDXBUF_UPDATE( Gpu &, void * p, size_t )
    {
        struct IdxBufUpdateParam
        {
            IdxBuf          * idxbuf;
            size_t            offset;
            size_t            length;
            void            * data;
            SurfaceUpdateFlag flag;
        };

        IdxBufUpdateParam * vbup = (IdxBufUpdateParam*)p;

        vbup->idxbuf->update( vbup->offset, vbup->length, vbup->data, vbup->flag );

        HeapFree( vbup->data );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_IDXBUF_READBACK( Gpu &, void * p, size_t )
    {
        struct IdxBufReadBackParam
        {
            IdxBuf             * ib;
            std::vector<UInt8> * buf;
        };
        IdxBufReadBackParam * vbrp = (IdxBufReadBackParam*)p;

        vbrp->ib->readback( *vbrp->buf );
     }
}}

