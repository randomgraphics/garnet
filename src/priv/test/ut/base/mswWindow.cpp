#include "../testCommon.h"
#include "garnet/GNwin.h"

#if GN_MSWIN

class WindowTest : public CxxTest::TestSuite
{
public:

    void testParentAndChild()
    {
        GN::win::MswWindow parent, child;
        GN::win::MswWindow::CreateParam cp;

        TS_ASSERT( parent.create( cp ) );
        parent.showWindow( true );
        TS_ASSERT( parent.getWindow() );

        cp.parent = parent.getWindow();
        TS_ASSERT( child.create( cp ) );
        child.showWindow( true );
        TS_ASSERT( child.getWindow() );
    }

    void testUserStyle()
    {
        GN::win::MswWindow win;
        GN::win::MswWindow::CreateParam cp;

        cp.style = WS_DLGFRAME;

        TS_ASSERT( win.create(cp) );
        win.showWindow( true );

        TS_ASSERT( WS_DLGFRAME | ::GetWindowLong( (HWND)win.getWindow(), GWL_STYLE ) );
    }

    void testClientSize()
    {
        GN::win::MswWindow win;

        GN::win::MswWindow::CreateParam cp;

        cp.clientWidth = 236;
        cp.clientHeight = 189;

        TS_ASSERT( win.create( cp ) );
        win.showWindow( true );
        if( !win.getWindow() ) return;

        uint32_t w, h;
        win.getClientSize( w, h );
        TS_ASSERT_EQUALS( w, cp.clientWidth );
        TS_ASSERT_EQUALS( h, cp.clientHeight );
    }
};

#endif
