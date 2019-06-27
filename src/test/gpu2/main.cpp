#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

struct gpu2ex
{
    AutoRef<Gpu2> gpu;
    AutoRef<Gpu2::CommandList> cl;
    AutoRef<Gpu2::MemoryBlock> um, dm;

    gpu2ex(GN::win::Window * window)
    {
        gpu = GN::gfx::Gpu2::createGpu2({window});
        cl = gpu->createCommandList({});
        um = gpu->createMemoryBlock({16, Gpu2::MemoryType::UPLOAD});
        dm = gpu->createMemoryBlock({16, Gpu2::MemoryType::DEFAULT});
    }
};

class DX12Triangle : public StdClass
{
    GN_DECLARE_STDCLASS(DX12Triangle, StdClass);

    AutoRef<Gpu2::Surface> _buffer;

    void clear()
    {

    }

public:
    
    DX12Triangle()
    {
        clear();
    }

    ~DX12Triangle()
    {
        quit();
    }

    bool init(gpu2ex &)
    {
        GN_STDCLASS_INIT();

        return success();
    }

    void quit()
    {
        GN_STDCLASS_QUIT();
    }

    void render()
    {

    }
};

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
    gpu2ex g(mainWindow.get());

    // create a triangle
    DX12Triangle tri;
    GN_VERIFY(tri.init(g));

    // main loop
    while(mainWindow->runUntilNoNewEvents()) {
        gInput.processInputEvents();
        if( gInput.getKeyStatus( KeyCode::ESCAPE ).down || gInput.getKeyStatus( KeyCode::XB360_A ).down )
        {
            break;
        }

        g.cl->clear({{0.f, 1.f, 0.f, 0.f}});
        tri.render();
        g.gpu->kickoff(*g.cl);
        g.gpu->present({});
    }

    // ensure all rendering is done before everything tears down.
    g.gpu->finish();

    // done
    return 0;
}
