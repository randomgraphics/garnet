#include "../testCommon.h"

class WindowTest : public CxxTest::TestSuite
{
public:

    void testParentAndChild()
    {
        GN::Window parent, child;
        GN::Window::CreateParam cp;

        TS_ASSERT( parent.create( cp ) );
        TS_ASSERT( parent.getWindow() );

        cp.parent = parent.getWindow();
        TS_ASSERT( child.create( cp ) );
        TS_ASSERT( child.getWindow() );
    }

    void testUserStyle()
    {
#if GN_WINNT
        GN::Window win;
        GN::Window::CreateParam cp;

        cp.style = WS_DLGFRAME;

        TS_ASSERT( win.create(cp) );

        TS_ASSERT( WS_DLGFRAME | ::GetWindowLong( (HWND)win.getWindow(), GWL_STYLE ) );
#endif
    }

    void testClientSize()
    {
        GN::Window win;

        GN::Window::CreateParam cp;

        cp.clientWidth = 236;
        cp.clientHeight = 189;

        TS_ASSERT( win.create( cp ) );
        if( !win.getWindow() ) return;

        uint32_t w, h;
        win.getClientSize( w, h );
        TS_ASSERT_EQUALS( w, cp.clientWidth );
        TS_ASSERT_EQUALS( h, cp.clientHeight );
    }
};
