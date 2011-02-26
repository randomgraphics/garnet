#include "pch.h"

using namespace GN;
using namespace GN::engine;

int run()
{
    if( !world::initialize() ) return -1;
    if( !world::gfxInitialize() ) return -1;
    if( !world::inputInitialize() ) return -1;
}

int main()
{
    int r = run();
    world::shutdown();
    return r;
}
