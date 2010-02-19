#include "pch.h"
#include "oglVtxBuf.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.OGL.VtxBuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufNormal::Init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGLVtxBufNormal, () );

    if( 0 == desc.length )
    {
        GN_ERROR(sLogger)( "Vertex buffer size can't be zero!" );
        return Failure();
    }

    // store descriptor
    setDesc( desc );

    mBuffer = (UInt8*)HeapAlloc( desc.length );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufNormal::Quit()
{
    GN_GUARD;

    SafeHeapFree( mBuffer );

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from VtxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufNormal::update( size_t offset, size_t length, const void * data, SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( Ok() );

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
void GN::gfx::OGLVtxBufNormal::readback( std::vector<UInt8> & data )
{
    size_t length = getDesc().length;

    data.resize( length );

    memcpy( &data[0], mBuffer, length );
}