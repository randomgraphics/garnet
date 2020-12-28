#include "pch.h"
#include "oglIdxBuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.IdxBuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLIdxBuf::init( const IdxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    // check parameter
    if( 0 == desc.numidx )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return failure();
    }

    setDesc( desc );

    mBytesPerIndex = desc.bits32 ? 4 : 2;

    mBuffer.allocate(desc.numidx * mBytesPerIndex, (const uint8_t*)nullptr, desc.fastCpuWrite ? GL_STREAM_DRAW : GL_STATIC_DRAW);

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::quit()
{
    GN_GUARD;

    mBuffer.cleanup();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// From IdxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::update( uint32 startidx, uint32 numidx, const void * data, SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !validateUpdateParameters( startidx, &numidx, data, flag ) ) return;

    if( 0 == numidx ) return;

    mBuffer.update<uint8_t>((const uint8_t*)data, startidx * mBytesPerIndex, numidx * mBytesPerIndex);

    GN_UNGUARD_SLOW
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::readback( DynaArray<uint8> & data )
{
    data.resize(mBuffer.length);
    mBuffer.getData<uint8_t>(data.data(), 0, mBuffer.length);
}