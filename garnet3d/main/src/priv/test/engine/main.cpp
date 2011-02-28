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
        engine::GfxOptions go;
        go.useMultithreadGpu = !GN_POSIX;
        go.defaultNonAsciiFont.fontname = "font::/simsun.ttc";
        go.defaultNonAsciiFont.width = 16;
        go.defaultNonAsciiFont.height = 16;
        go.defaultNonAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;
        go.defaultAsciiFont.fontname = "font::/ltype.ttf";
        go.defaultAsciiFont.width = 16;
        go.defaultAsciiFont.height = 16;
        go.defaultAsciiFont.quality = gfx::FontFaceDesc::ANTIALIASED;

        noerror = engine::gfxInitialize( go );
    }

    if( noerror )
    {
        noerror = run();
    }

    engine::shutdown();

    return noerror ? 0 : -1;
}
