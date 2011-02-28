#include "pch.h"

using namespace GN;

bool run()
{
    return true;
}

int main()
{
    bool noerror = engine::initialize();

    if( noerror )
    {
        noerror = engine::inputInitialize( input::InputAPI::NATIVE );
    }

    if( noerror )
    {
        // setup default graphics options
        engine::GfxInitOptions gio;
        gio.useMultithreadGpu = !GN_POSIX;
        gio.defaultNonAsciiFont.fontname = "font::/simsun.ttc";
        gio.defaultNonAsciiFont.width = 16;
        gio.defaultNonAsciiFont.height = 16;
        gio.defaultNonAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;
        gio.defaultAsciiFont.fontname = "font::/ltype.ttf";
        gio.defaultAsciiFont.width = 16;
        gio.defaultAsciiFont.height = 16;
        gio.defaultAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;

        noerror = engine::gfxInitialize( gio );
    }

    if( noerror )
    {
        noerror = run();
    }

    engine::shutdown();

    return noerror ? 0 : -1;
}
