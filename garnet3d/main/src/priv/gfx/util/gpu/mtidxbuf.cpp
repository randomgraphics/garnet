#include "pch.h"
#include "mtidxbuf.h"
#include "mtrndrCmd.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.rndr.mtidxbuf");

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
        mRenderer.postCommand1( CMD_IDXBUF_DESTROY, mIdxBuf );
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

    void * tmpbuf = heapAlloc( length );
    if( NULL == tmpbuf )
    {
        GN_ERROR(sLogger)( "fail to allocate temporary buffer." );
        return;
    }
    memcpy( tmpbuf, data, length );

    mRenderer.postCommand5( CMD_IDXBUF_UPDATE, mIdxBuf, offset, length, tmpbuf, flag );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::readback( std::vector<UInt8> & data )
{
    mRenderer.postCommand2( CMD_IDXBUF_READBACK, mIdxBuf, &data );
}

// *****************************************************************************
// Command handlers (called by back end thread)
// *****************************************************************************

namespace GN { namespace gfx
{
    //
    //
    // -------------------------------------------------------------------------
    void func_IDXBUF_DESTROY( Renderer &, void * p, size_t )
    {
        IdxBuf * ib = *(IdxBuf**)p;
        ib->decref();
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_IDXBUF_UPDATE( Renderer &, void * p, size_t )
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

        heapFree( vbup->data );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_IDXBUF_READBACK( Renderer &, void * p, size_t )
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

