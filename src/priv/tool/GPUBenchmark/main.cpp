#include "pch.h"
#include "fillrate.cpp" // pixel pipeline speed
//#include "bandwidth.cpp" // memory bandwidth
//#include "triangles.cpp" // vertex pipeline speed
int main( int argc, const char * argv[] )
{
    int r;
    {
        FillrateApp app;
        r = app.run( argc, argv );
        if( 0 != r ) return r;
    }
    return 0;
}
