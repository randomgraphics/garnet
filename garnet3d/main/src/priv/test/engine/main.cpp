#include "pch.h"

using namespace GN;

int run()
{
    if( !engine::initialize() ) return -1;
    if( !engine::gfxInitialize() ) return -1;
    if( !engine::inputInitialize() ) return -1;

    return 0;
}

int main()
{
    int r = run();
    engine::shutdown();
    return r;
}
