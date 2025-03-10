#include <garnet/GNgfx2.h>
#include <garnet/GNinput.h>

using namespace GN;

int main() {
    // create a window
    std::unique_ptr<GN::win::Window> mainWindow(GN::win::createWindow({"GPU2 hello world", 1280, 720}));
    mainWindow->show();

    // initialize input system
    input::ScopedInputInitializer sii(input::InputAPI::NATIVE);
    gInput.attachToWindow(mainWindow->getDisplayHandle(), mainWindow->getWindowHandle());

    // create GPU2
    auto gpu = GN::gfx::Gpu2::createGpu2({mainWindow.get()});
    if (!gpu) return -1;

    // main loop
    while (mainWindow->runUntilNoNewEvents()) {
        gInput.processInputEvents();

        // quit if ESCAPE is pressed
        if (gInput.getKeyStatus(input::KeyCode::ESCAPE).down) { break; }

        // g.cl->reset();
        // g.cl->clear({{0.f, 1.f, 0.f, 0.f}});
        // tri.render(g);
        // g.gpu->kickOff(*g.cl);
        gpu->present({});
    }

    // done
    return 0;
}