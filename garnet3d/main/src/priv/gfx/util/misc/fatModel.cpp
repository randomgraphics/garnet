#include "pch.h"
#include "garnet/gfx/fatModel.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.FatModel");

// *****************************************************************************
// FatVertexBuffer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatVertexBuffer::resize( uint32 format, size_t count )
{
    if( 0 == format || 0 == count )
    {
        clear();
        return true;
    }

    void * vertices[NUM_SEMANTICS];
    memset( vertices, 0, sizeof(vertices) );
    bool outofmem = false;
    for( int i = 0; i < NUM_SEMANTICS; ++i )
    {
        if( (1<<i) & format )
        {
            vertices[i] = HeapMemory::alignedAlloc( count * 128, 16 );
            if( NULL == vertices[i] )
            {
                outofmem = true;
                break;
            }
        }
    }
    if( outofmem )
    {
        for( int i = 0; i < NUM_SEMANTICS; ++i )
        {
            safeHeapDealloc( vertices[i] );
        }
        GN_ERROR(sLogger)( "Fail to resize fat vertex buffer: out of memory." );
        return false;
    }

    for( int i = 0; i < NUM_SEMANTICS; ++i )
    {
        if( (1<<i) & format )
        {
            GN_ASSERT( vertices[i] );
            memcpy( vertices[i], mVertices[i], math::getmin<>(count,mCount) );
            safeHeapDealloc( mVertices[i] );
            mVertices[i] = vertices[i];
        }
        else
        {
            safeHeapDealloc( mVertices[i] );
        }
    }

    mFormat = format;
    mCount = count;

    return true;
}
