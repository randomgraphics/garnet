#include "pch.h"
#include "oglVtxBuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.VtxBuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufNormal::init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGLVtxBufNormal, () );

    if( 0 == desc.length )
    {
        GN_ERROR(sLogger)( "Vertex buffer size can't be zero!" );
        return failure();
    }

    // store descriptor
    setDesc( desc );

    mBuffer = (uint8*)HeapMemory::alloc( desc.length );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufNormal::quit()
{
    GN_GUARD;

    safeHeapDealloc( mBuffer );

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
void GN::gfx::OGLVtxBufNormal::update( uint32 offset, uint32 length, const void * data, SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    GN_UNUSED_PARAM( flag );

    if( !validateUpdateParameters( offset, &length, data, flag ) ) return;

    if( 0 == length ) return;

    if( NULL == data )
    {
        GN_ERROR(sLogger)( "NULL data pointer." );
        return;
    }

    memcpy( mBuffer + offset, data, length );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufNormal::readback( DynaArray<uint8> & data )
{
    size_t length = getDesc().length;

    data.resize( length );

    memcpy( &data[0], mBuffer, length );
}