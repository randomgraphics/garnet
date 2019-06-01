#include "pch.h"

//*
using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

int main()
{
    enableCRTMemoryCheck();

    // create a window
    std::unique_ptr<GN::win::Window> mainWindow(GN::win::createWindow({"GPU2 test", 1280, 720}));
    mainWindow->show();

    // initialize input system
    initializeInputSystem( InputAPI::NATIVE );
    gInput.attachToWindow( mainWindow->getDisplayHandle(), mainWindow->getWindowHandle() );

    // create GPU2
    AutoRef<Gpu2> g = GN::gfx::Gpu2::createGpu2({
        mainWindow.get(),
    });
    if( nullptr == g ) return -1;

    // create a command list
    auto cl = g->createCommandList({});

    // main loop
    while(mainWindow->runUntilNoNewEvents()) {
        gInput.processInputEvents();
        if( gInput.getKeyStatus( KeyCode::ESCAPE ).down || gInput.getKeyStatus( KeyCode::XB360_A ).down )
        {
            break;
        }

        cl->clear({{0.f, 1.f, 0.f, 0.f}});
        g->kickoff(*cl);
        g->present({});
    }

    // done
    return 0;
}
