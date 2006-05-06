#include "../testCommon.h"

typedef GN::ResourceManager<int> ResMgr;

bool defCreator( int & res, const GN::StrA & name, void * )
{
    return str2Int<int>( res, name.cptr() );
}

bool nullCreator( int & res, const GN::StrA &, void * )
{
    res = -1;
    return true;
}

bool failedCreator( int &, const GN::StrA &, void * )
{
    return false;
}

void defDeletor( int &, void* )
{
    // do nothing
}

class ResourceManagerTest : public CxxTest::TestSuite
{
public:

    // no creator
    void testNoCreator()
    {
        ResMgr rm;

        TS_ASSERT( rm.addResource( "1" ) );

        TS_ASSERT_EQUALS( 0, rm.getResource( "1" ) );
    }

    // default nullor
    void testDefaultNullor()
    {
        ResMgr rm;

        TS_ASSERT( rm.addResource( "1" ) );

        TS_ASSERT( !rm.getResource( "2", false ) );

        // default nullor failure
        rm.setNullor( GN::makeDelegate(&failedCreator) );
        TS_ASSERT_EQUALS( 0, rm.getResource( "1" ) );
        TS_ASSERT_EQUALS( 0, rm.getResource( "2", false ) );

        // default nullor success
        rm.setNullor( GN::makeDelegate(&nullCreator) );
        TS_ASSERT_EQUALS( -1, rm.getResource( "1" ) );
        TS_ASSERT_EQUALS( -1, rm.getResource( "2", false ) );
        TS_ASSERT_EQUALS( -1, rm.getResource( "2" ) );
    }

    // per-resource nullor
    void testNullor()
    {
        ResMgr rm;

        // per-resource nullor failed
        TS_ASSERT( rm.addResource(
            "1",
            0,
            ResMgr::Creator(),
            GN::makeDelegate(&failedCreator) ) );
        TS_ASSERT_EQUALS( 0, rm.getResource("1") );

        // per-resource nullor success
        TS_ASSERT( rm.addResource(
            "2",
            0,
            ResMgr::Creator(),
            GN::makeDelegate(&nullCreator) ) );
        TS_ASSERT_EQUALS( -1, rm.getResource("2") );
    }

    // default creator
    void testDefaultCreator()
    {
        ResMgr rm;

        TS_ASSERT( rm.addResource( "1" ) );

        // default nullor failure
        rm.setCreator( GN::makeDelegate(&failedCreator) );
        TS_ASSERT_EQUALS( 0, rm.getResource( "1" ) );

        // default nullor success
        rm.disposeAll();
        rm.setCreator( GN::makeDelegate(&defCreator) );
        TS_ASSERT_EQUALS( 1, rm.getResource( "1" ) );
    }

    // per-resource creator
    void testCreator()
    {
        // TODO: implement this test case
    }

    // override existing resource
    void testOverrideExistingResource()
    {
        ResMgr rm;

        TS_ASSERT( rm.addResource( "1", 0, GN::makeDelegate(&defCreator), ResMgr::Creator(), false ) );
        TS_ASSERT_EQUALS( 1, rm.getResource( "1" ) );

        // default is not overriding
        TS_ASSERT( !rm.addResource( "1" ) );
        TS_ASSERT_EQUALS( 1, rm.getResource( "1" ) );

        // override existing
        TS_ASSERT( rm.addResource( "1", 0, GN::makeDelegate(&nullCreator), ResMgr::Creator(), true ) );
        TS_ASSERT_EQUALS( -1, rm.getResource( "1" ) );
    }

    void testName2Handle()
    {
        ResMgr rm;

        // rm should be empty
        TS_ASSERT( rm.empty() );

        ResMgr::HandleType h1 = rm.addResource( "1" );
        TS_ASSERT( h1 );

        // rm should NOT be empty
        TS_ASSERT( !rm.empty() );

        // handle -> name
        TS_ASSERT_EQUALS( "1", rm.getResourceName(h1) );
        TS_ASSERT_EQUALS( "", rm.getResourceName(h1+1) );

        // name -> handle
        TS_ASSERT_EQUALS( h1, rm.getResourceHandle("1") );
        TS_ASSERT_EQUALS( 0, rm.getResourceHandle("2",false) );
        TS_ASSERT_DIFFERS( h1, rm.getResourceHandle("2") );
        TS_ASSERT_DIFFERS( 0, rm.getResourceHandle("2") );
        TS_ASSERT_EQUALS( rm.getResourceHandle("2"), rm.getResourceHandle("2") );
    }

    void testRemoveHandle()
    {
        ResMgr rm;

        ResMgr::HandleType h1 = rm.addResource( "1" );
        ResMgr::HandleType h2 = rm.addResource( "2" );

        TS_ASSERT( rm.validResourceHandle( h1 ) );
        TS_ASSERT( rm.validResourceHandle( h2 ) );

        rm.removeResourceByHandle( h1 );

        TS_ASSERT( !rm.validResourceHandle( h1 ) );
        TS_ASSERT( rm.validResourceHandle( h2 ) );
    }

    void testRemoveName()
    {
        ResMgr rm;

        ResMgr::HandleType h1 = rm.addResource( "1" );
        ResMgr::HandleType h2 = rm.addResource( "2" );

        TS_ASSERT( rm.validResourceHandle( h1 ) );
        TS_ASSERT( rm.validResourceHandle( h2 ) );

        rm.removeResourceByName( "1" );

        TS_ASSERT( !rm.validResourceHandle( h1 ) );
        TS_ASSERT( rm.validResourceHandle( h2 ) );
    }
};
