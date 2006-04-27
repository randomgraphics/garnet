#include "pch.h"
#include "coreResourceDict.h"

// *****************************************************************************
// local functions
// *****************************************************************************

using namespace GN;
using namespace GN::core;

struct CoreRawData : public RawData
{
    std::vector<uint8_t> rawData;
    CoreRawData( size_t sz ) : rawData(sz) {}
    virtual size_t size() const { GN_ASSERT(!rawData.empty()); return rawData.size(); }
    virtual void * data() const { GN_ASSERT(!rawData.empty()); return (void*)&rawData[0]; }
};

//
// 
// -----------------------------------------------------------------------------
static bool sCreateNullResource( RawData * & result, const StrA &, void * )
{
    GN_GUARD;
    CoreRawData * r = new CoreRawData(1);
    r->rawData[0] = 0;
    result = r;
    return true;
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sCreateResource( RawData * & result, const StrA & name, void * )
{
    GN_GUARD;

    // get resource path
    StrA path = GN::core::searchResourceFile( name );
    if( path.empty() )
    {
        GN_ERROR( "Raw resource '%s' creation failed: path not found.", name.cptr() );
        return false;
    }

    GN_INFO( "Load raw resource '%s' from file '%s'.", name.cptr(), path.cptr() ); 

    // open file
    AnsiFile fp;
    if( !fp.open( path::toNative(path), "rb" ) )
    {
        GN_ERROR( "Raw resource '%s' creation failed: can't open file '%s'.", name.cptr(), path.cptr() );
        return false;
    }

    // read file content
    CoreRawData * r = new CoreRawData( fp.size() );
    if( 0 == fp.size() )
    {
        r = new CoreRawData(1);
        r->rawData[0] = 0;
    }
    else
    {
        r = new CoreRawData( fp.size() );
        if( fp.size() != fp.read( &r->rawData[0], fp.size() ) )
        {
            GN_ERROR( "Raw resource '%s' creation failed: read file '%s' error.", name.cptr(), path.cptr() );
            delete r;
            return false;
        }
    }

    // success
    result = r;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteResource( RawData * & ptr, void * )
{
    GN::safeDelete( ptr );
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::core::CoreRawDataDict::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::core::CoreRawDataDict, () );

    // register functors
    mDict.setCreator( &sCreateResource );
    mDict.setDeletor( &sDeleteResource );
    mDict.setNullor( &sCreateNullResource );

     // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::core::CoreRawDataDict::quit()
{
    GN_GUARD;

     // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
