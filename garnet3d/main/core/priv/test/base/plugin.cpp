#include "../testCommon.h"
#include "garnet/base/plugin.h"

struct MyPlugin : public GN::PluginBase
{
    virtual int whoAmI() const = 0;
};

struct Plugin1 : public MyPlugin
{
    int whoAmI() const { return 1; }
    static GN::PluginBase * newInstance( void * )
    {
        return new Plugin1;
    }
};

struct Plugin2 : public MyPlugin
{
    int whoAmI() const { return 2; }
    static GN::PluginBase * newInstance( void * )
    {
        return new Plugin2;
    }
};

struct Plugin3 : public MyPlugin
{
    int whoAmI() const { return 3; }
    static GN::PluginBase * newInstance( void * )
    {
        return new Plugin3;
    }
};

static int whoAreYou( GN::PluginBase * p )
{
    if ( 0 == p ) return -1;
    MyPlugin * pp = GN::safeCast<MyPlugin*>( p );
    return pp->whoAmI();
}

class PluginTest : public CxxTest::TestSuite
{
public:

    void test1()
    {
        GN::PluginManager mgr;

        TS_ASSERT( !mgr.registerPlugin( "", "aa", "", 0 ) );
        TS_ASSERT( !mgr.registerPlugin( "aa", "", "", 0 ) );
        TS_ASSERT( !mgr.registerPlugin( "aa", "aa", "", 0 ) );
        TS_ASSERT( !mgr.registerPlugin( "aa", "aa", "", &Plugin1::newInstance ) );
        TS_ASSERT_EQUALS( mgr.size(), 0 );

        GN::PluginID ap1, ap2, bp1, bp2, bp3;

        TS_ASSERT( mgr.registerPluginType( "aa", "aaa" ) );
        TS_ASSERT( !mgr.registerPluginType( "aa", "bbb" ) );
        TS_ASSERT( !mgr.registerPlugin( "bb", "p1", "bp1", &Plugin1::newInstance ) );
        TS_ASSERT( mgr.registerPluginType( "bb", "bbb" ) );
        TS_ASSERT( ap1 = mgr.registerPlugin( "aa", "p1", "ap1", &Plugin1::newInstance ) );
        TS_ASSERT( ap2 = mgr.registerPlugin( "aa", "p2", "ap2", &Plugin2::newInstance ) );
        TS_ASSERT( bp1 = mgr.registerPlugin( "bb", "p1", "bp1", &Plugin1::newInstance ) );
        TS_ASSERT( bp2 = mgr.registerPlugin( "bb", "p2", "bp2", &Plugin2::newInstance ) );
        TS_ASSERT( bp3 = mgr.registerPlugin( "bb", "p3", "bp3", &Plugin3::newInstance ) );
        TS_ASSERT( !mgr.registerPlugin( "bb", "p4", "bp4", NULL ) );
        TS_ASSERT_EQUALS( mgr.size(), 5 );

        TS_ASSERT_EQUALS( mgr.getPluginID( "aa", "p1" ), ap1 );
        TS_ASSERT_EQUALS( mgr.getPluginID( "aa", "P1" ), GN::PluginID::INVALID );
        TS_ASSERT_EQUALS( mgr.getPluginID( "Aa", "p1" ), GN::PluginID::INVALID );

        TS_ASSERT_EQUALS( mgr.getPluginTypeName(0), "" );
        TS_ASSERT_EQUALS( mgr.getPluginTypeName(ap1.type), "aa" );

        TS_ASSERT_EQUALS( mgr.getPluginTypeDesc(0), "" );
        TS_ASSERT_EQUALS( mgr.getPluginTypeDesc(ap2.type), "aaa" );

        TS_ASSERT_EQUALS( mgr.getPluginName(0), "" );
        TS_ASSERT_EQUALS( mgr.getPluginName(bp1), "p1" );

        TS_ASSERT_EQUALS( mgr.getPluginDesc(0), "" );
        TS_ASSERT_EQUALS( mgr.getPluginDesc(bp2), "bp2" );

        mgr.removePlugin( "aa", "p3" );
        mgr.removePlugin( "bb", "p2" );
        TS_ASSERT_EQUALS( mgr.size(), 4 );

        mgr.removePluginType( "bb" );
        TS_ASSERT_EQUALS( mgr.size(), 2 );
        mgr.removePluginType( "bb" );
        TS_ASSERT_EQUALS( mgr.size(), 2 );
        TS_ASSERT( !mgr.registerPlugin( "bb", "p1", "bp1", &Plugin1::newInstance ) );

        mgr.removePlugin( "aa", "p1" );
        mgr.removePlugin( "aa", "p2" );
        TS_ASSERT_EQUALS( mgr.size(), 0 );
        TS_ASSERT( mgr.registerPlugin( "aa", "p1", "ap1", &Plugin1::newInstance ) );
        TS_ASSERT_EQUALS( mgr.size(), 1 );
    }

    void testOverrideExistingPlugin()
    {
        GN::PluginManager mgr;

        TS_ASSERT( mgr.registerPluginType( "a", "aa" ) );

        // can't override existing plugin type
        TS_ASSERT( !mgr.registerPluginType( "a", "aa" ) );

        TS_ASSERT( mgr.registerPlugin( "a", "p", "ap", &Plugin1::newInstance ) );

        // can't override existing plugin by default
        TS_ASSERT( !mgr.registerPlugin( "a", "p", "ap", &Plugin2::newInstance ) );

        TS_ASSERT_EQUALS( whoAreYou( mgr.createInstance( "a", "p" ) ), 1 );

        // force override existing plugin
        TS_ASSERT( mgr.registerPlugin( "a", "p", "ap", &Plugin2::newInstance, true ) );

        TS_ASSERT_EQUALS( whoAreYou( mgr.createInstance( "a", "p" ) ), 2 );

        // create instance of invalid plugin
        TS_ASSERT_EQUALS( whoAreYou( mgr.createInstance( "A", "p" ) ), -1 );
        TS_ASSERT_EQUALS( whoAreYou( mgr.createInstance( "a", "P" ) ), -1 );
    }
};
