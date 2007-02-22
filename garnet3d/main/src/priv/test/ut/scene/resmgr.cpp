#include "../testCommon.h"
#include "garnet/GNscene.h"

class SceneResourceManagerTest : public CxxTest::TestSuite
{
    typedef GN::scene::ResourceManager ResMgr;

    struct MyRes : public GN::RefCounter
    {
        GN::StrA s;
    };

    static GN::RefCounter * sNewMyRes( const GN::StrA & name )
    {
        MyRes * r = new MyRes;
        r->s = name;
        return r;
    };

    static void sDelMyRes( GN::RefCounter * r )
    {
        GN::safeDecref( r );
    }

    void initrm( ResMgr & rm )
    {
        rm.registerResourceType( "r1", &sNewMyRes, &sDelMyRes );
        rm.registerResourceType( "r2", &sNewMyRes, &sDelMyRes );
    }

public:

    // no creator
    void testAddResource()
    {
        using namespace GN::scene;

        ResMgr rm;
        initrm( rm );

        ResourceId id = rm.addResource( "1", "r1" );
        TS_ASSERT( id );
        TS_ASSERT_EQUALS( id, rm.getResourceId( "1" ) );
        TS_ASSERT_EQUALS( "1", rm.getResourceT<MyRes>( "1" )->s );

        TS_ASSERT( !rm.addResource( "1", "r2" ) );

        id = rm.addResource( "2", "r2" );
        TS_ASSERT( id );
        TS_ASSERT_EQUALS( id, rm.getResourceId( "2" ) );
        TS_ASSERT_EQUALS( "2", rm.getResourceT<MyRes>( "2" )->s );
        TS_ASSERT_DIFFERS( id, rm.getResourceId( "1" ) );

        TS_ASSERT( !rm.addResource( "1", "r3" ) );
    }
};
