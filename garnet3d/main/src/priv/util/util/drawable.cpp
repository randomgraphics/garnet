#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::util;

// *****************************************************************************
// local var/types/functions
// *****************************************************************************

class PrivateGpp : public GpuProgramParam
{
public:

    /// constructor
    PrivateGpp( size_t size ) : GpuProgramParam( false, size ) {}
};

class SharedGpp;
static std::map<StrA,SharedGpp*> sSharedGppMap;

class SharedGpp : public GpuProgramParam
{
    std::map<StrA,SharedGpp*>::const_iterator mIter;

public:

    /// constructor
    SharedGpp( const StrA & name, size_t size )
        : GpuProgramParam( false, size )
    {
        mIter = sSharedGppMap.insert( std::make_pair(name, this) ).first;
    }

    /// dtor
    ~SharedGpp()
    {
        sSharedGppMap.erase( mIter );
    }
};

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GpuProgramParam * GN::util::createPrivateGpuProgramParam( size_t size )
{
    return new PrivateGpp( size );
}

//
//
// -----------------------------------------------------------------------------
GpuProgramParam * GN::util::createSharedGpuProgramParam( const StrA & name, size_t size )
{
    std::map<StrA,SharedGpp*>::const_iterator iter = sSharedGppMap.find( name );

    if( iter != sSharedGppMap.end() )
        // found!
        return iter->second;
    else
        // not found!
        return new SharedGpp( name, size );
}
