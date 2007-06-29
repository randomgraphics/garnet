#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseKernelParameter");

// *****************************************************************************
//
// *****************************************************************************

#define CHECK_INPUT( T ) \
    if( KERNEL_PARAMETER_TYPE_##T != mDesc.type ) { GN_ERROR(sLogger)( "not a " #T "parameter." ); return; } \
    if( (offset+count) > mDesc.count ) { GN_ERROR(sLogger)( "data overflow!" ); return; } \
    if( 0 == values ) { GN_ERROR(sLogger)( "null values" ); return; }

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameter::setb( size_t offset, size_t count, const bool * values )
{
    CHECK_INPUT( BOOL );

    offset *= sizeof(bool);
    count  *= sizeof(bool);

    GN_ASSERT( mData.size() >= (offset*count) );

    memcpy( &mData[offset], values, count );

    mEmpty = false;
    sigValueSet( offset, count );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameter::seti( size_t offset, size_t count, const int * values )
{
    CHECK_INPUT( INT );

    offset *= sizeof(int);
    count  *= sizeof(int);

    GN_ASSERT( mData.size() >= (offset*count) );

    memcpy( &mData[offset], values, count );

    mEmpty = false;
    sigValueSet( offset, count );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameter::setf( size_t offset, size_t count, const float * values )
{
    CHECK_INPUT( FLOAT );

    offset *= sizeof(float);
    count  *= sizeof(float);

    GN_ASSERT( mData.size() >= (offset*count) );

    memcpy( &mData[offset], values, count );

    mEmpty = false;
    sigValueSet( offset, count );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameter::sets( size_t offset, size_t count, const char * const * values )
{
    CHECK_INPUT( STRING );

    GN_ASSERT( mStr.size() >= (offset*count) );

    for( size_t i = offset; i < (offset+count); ++i )
    {
        mStr[i] = values[i - offset];
    }

    mEmpty = false;
    sigValueSet( offset, count );
}

// *****************************************************************************
// BaseKernelParameterSet
// *****************************************************************************

GN::gfx::BaseKernelParameterSet::BaseKernelParameterSet( BaseKernel & e )
    : KernelParameterSet(e)
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BaseKernelParameterSet::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::BaseKernelParameterSet, () );

    BaseKernel & k = GN_SAFE_CAST<BaseKernel&>( getKernel() );

    size_t count = k.getParameterCount();

    mParameters.resize( count );

    for( size_t i = 0; i < count; ++i )
    {
        mParameters[i] = createParameter( k.getParameterDescByIndex(i) );
        if( 0 == mParameters[i] ) return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameterSet::quit()
{
    GN_GUARD;

    for( size_t i = 0; i < mParameters.size(); ++i )
    {
        safeDelete( mParameters[i] );
    }

    mParameters.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameter *
GN::gfx::BaseKernelParameterSet::getParameter( KernelParameterHandle handle ) const
{
    GN_GUARD_SLOW;

    BaseKernel & k = GN_SAFE_CAST<BaseKernel&>( getKernel() );

    size_t index;

    if( !k.getParameterIndex( index, handle ) ) return 0;

    GN_ASSERT( index < mParameters.size() );

    GN_ASSERT( mParameters[index] );

    return mParameters[index];

    GN_UNGUARD_SLOW;
}
