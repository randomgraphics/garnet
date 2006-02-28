#include "pch.h"
#include <windows.h>

#ifdef HAS_CEGUI
#include "ceguiApp.h"
#else
class GuiTest : public SampleApp {};
#endif

int main( int argc, const char * argv[] )
{
    GuiTest app;
    return app.run( argc, argv );
}
