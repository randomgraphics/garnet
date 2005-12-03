#include "pch.h"
#include "d3dUtils.h"

//
//
// -----------------------------------------------------------------------------
void GN::gfx::printShaderCompileError( HRESULT hr, const char * code, LPD3DXBUFFER err )
{
    GN_GUARD;

    GNGFX_ERROR(
        "\n"
        "================== Shader compile failure ===============\n"
        "%s\n"
        "---------------------------------------------------------\n"
        "hr=08x%X\n"
        "%s\n"
        "---------------------------------------------------------\n"
        "%s\n"
        "=========================================================\n",
        code ? code : "Shader code: <EMPTY>",
        hr, DXGetErrorDescription9A(hr),
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}
