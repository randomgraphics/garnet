#include "pch.h"
#include "BasicRenderer.h"

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::BasicRenderer::drawTextA(
    const char * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;

    if( strEmpty(s) ) return;

    size_t len = strLen(s);
    wchar_t * ws = 0;

    // allocate buffer in stack
    ws = (wchar_t*)alloca( sizeof(wchar_t) * (len+1) );

    // convert mbs -> unicode
    size_t wlen = mbstowcs( ws, s, len );
    if( static_cast<size_t>(-1L) == wlen )
    {
        GNGFX_ERROR( "fail to convert multi-byte string to unicode string!" );
        return;
    }
    ws[wlen] = 0;

    // call UNICODE version of drawText()
    drawTextW( ws, x, y, c );

    GN_UNGUARD_SLOW;
}
