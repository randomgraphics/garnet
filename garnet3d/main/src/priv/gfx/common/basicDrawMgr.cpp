#include "pch.h"
#include "basicRenderer.h"

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::drawDebugTextA(
    const char * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;

    if( strEmpty(s) ) return;

    size_t len = strLen(s);
    wchar_t * ws = 0;

    // allocate buffer in stack
    ws = (wchar_t*)alloca( sizeof(wchar_t) * (len+1) );

    // convert mbs -> unicode
    size_t wlen = mbs2wcs( ws, len+1, s, len );
    ws[wlen] = 0;

    // call UNICODE version of drawText()
    drawDebugTextW( ws, x, y, c );

    GN_UNGUARD_SLOW;
}
