#include "pch.h"
#include "mtidxbuf.h"
#include "mtgpuCmd.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.util.gpu.mtidxbuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadIdxBuf::Init( IdxBuf * ib )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadIdxBuf, () );

    if( NULL == ib ) return Failure();

    mIdxBuf = ib;

    const IdxBufDesc & desc = mIdxBuf->GetDesc();

    SetDesc( desc );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::Quit()
{
    GN_GUARD;

    if( mIdxBuf )
    {
        mGpu.postCommand1( CMD_IDXBUF_DESTROY, mIdxBuf );
        mIdxBuf = NULL;
    }

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from IdxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadIdxBuf::Update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag )
{
    if( 0 == length )
    {
        const IdxBufDesc & desc = GetDesc();
        size_t maxlen = desc.numidx * ( desc.bits32 ? 4 : 2 );
        length = maxlen - offset;
    }

    void * tmpbuf = HeapMemory::Alloc( length );
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
void GN::gfx::MultiThreadIdxBuf::Readback( DynaArray<UInt8> & data )
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
        ib->DecRef();
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

        vbup->idxbuf->Update( vbup->offset, vbup->length, vbup->data, vbup->flag );

        HeapMemory::Free( vbup->data );
    }

    //
    //
    // -------------------------------------------------------------------------
    void func_IDXBUF_READBACK( Gpu &, void * p, size_t )
    {
        struct IdxBufReadBackParam
        {
            IdxBuf             * ib;
            DynaArray<UInt8> * buf;
        };
        IdxBufReadBackParam * vbrp = (IdxBufReadBackParam*)p;

        vbrp->ib->Readback( *vbrp->buf );
     }
}}

