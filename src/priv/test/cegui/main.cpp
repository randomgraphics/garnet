#include "pch.h"
#include "garnet/GNapp.h"

#ifdef HAS_CEGUI
#include "ceguiApp.h"
#else
class GuiTest : public GN::app::SampleApp
{
public:
    bool onInit() { return true; }
    void onQuit() {}
    void onUpdate() {}
    void onRender() {}
};
#endif

int main( int argc, const char * argv[] )
{
    GuiTest app;
    return app.run( argc, argv );
}
