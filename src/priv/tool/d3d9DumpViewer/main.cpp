#include "pch.h"
#include "d3d9app.h"

int main()
{
    GN_GUARD_ALWAYS;

    GN::gfx::d3d9::D3D9Application app;
    return app.run();

    GN_UNGUARD_ALWAYS_NO_THROW;
    return -1;
}
