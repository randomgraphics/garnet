#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseKernelParameter");

// *****************************************************************************
//
// *****************************************************************************

#define CHECK_INPUT( T ) \
    if( KERNEL_PARAMETER_TYPE_##T != mDesc.type ) { GN_ERROR(sLogger)( "not a " #T " parameter." ); return; } \
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
    sigValueSet( mIndex, offset, count );
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
    sigValueSet( mIndex, offset, count );
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
    sigValueSet( mIndex, offset, count );
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
    sigValueSet( mIndex, offset, count );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameter::unset()
{
    mEmpty = true;
    sigValueUnset( mIndex );
}

// *****************************************************************************
// BaseKernelParameterSet
// *****************************************************************************

GN::gfx::BaseKernelParameterSet::BaseKernelParameterSet( BaseKernel & e )
    : KernelParameterSet(e)
{
    BaseKernel & k = GN_SAFE_CAST<BaseKernel&>( getKernel() );

    size_t count = k.getNumParameters();

    mParameters.resize( count );

    for( size_t i = 0; i < count; ++i )
    {
        mParameters[i] = new BaseKernelParameter( *k.getParameterDesc(i), i );
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::BaseKernelParameterSet::~BaseKernelParameterSet()
{
    GN_GUARD;

    for( size_t i = 0; i < mParameters.size(); ++i )
    {
        safeDelete( mParameters[i] );
    }

    mParameters.clear();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameter *
GN::gfx::BaseKernelParameterSet::getParameter( size_t index ) const
{
    GN_GUARD_SLOW;

    if( index >= mParameters.size() )
    {
        GN_ERROR(sLogger)( "index is out of range." );
        return 0;
    }

    GN_ASSERT( mParameters[index] );

    return mParameters[index];

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::BaseKernelParameter *
GN::gfx::BaseKernelParameterSet::getBaseParameterByName( const StrA & name ) const
{
    size_t index = getKernel().getParameterIndex( name );
    if( (size_t)-1 == index ) return 0;
    return GN_SAFE_CAST<BaseKernelParameter*>( mParameters[index] );
}
