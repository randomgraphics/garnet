#include "pch.h"
#include <garnet/base/simd.h>
#include <stdio.h>

using namespace GN;

simd::vec4 v;

simd::vec4 foo1( simd::vec4 v1, simd::vec4 v2, simd::vec4 v3 )
{
    v = v1 + v2 + v3;
    return v;
}

const simd::vec4 & foo2( const simd::vec4 & v1, const simd::vec4 & v2, const simd::vec4 & v3 )
{
    v = v1 + v2 + v3;
    return v;
}

int main()
{
    simd::vec4 a = simd::setf4(1,1,1,1), b = simd::setf4(2,2,2,2), c = simd::setf4(3,3,3,3);
    simd::vec4 x = simd::setf4(5,5,5,5), y = simd::setf4(6,6,6,6), z = simd::setf4(7,7,7,7);

    printf( "\n" );

    simd::vec4 d = foo1( a, b, c );

    printf( "\n" );

    simd::vec4 w = foo2( x, y, z );

    printf( "\n" );

    return (int)(simd::getx(d) + simd::getx(w));
}
