#include "pch.h"
#include "garnet/GNutil.h"

#ifdef HAS_CEGUI
#include "ceguiApp.h"
#else
class GuiTest : public GN::util::SampleApp
{
public:
    bool OnInit() { return true; }
    void OnQuit() {}
    void OnUpdate() {}
    void OnRender() {}
};
#endif

int main( int argc, const char * argv[] )
{
    GuiTest app;
    return app.Run( argc, argv );
}
