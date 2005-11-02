#include "pch.h"
#include "garnet/win/xWindow.h"

#if GN_POSIX

int main()
{
    GN::win::XWindow win;

    GN::win::XWindow::CreateParam cp;

    if( !win.create(cp) ) return -1;
    win.showWindow();

    GN::sleep(3000);

    return 0;
}

#endif // GN_POSIX
