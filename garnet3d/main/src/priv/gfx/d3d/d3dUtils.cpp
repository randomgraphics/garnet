#include "pch.h"
#include "d3dUtils.h"

//
//
// -----------------------------------------------------------------------------
void GN::gfx::printShaderCompileError( const char * code, LPD3DXBUFFER err )
{
    GN_GUARD;

    GNGFX_ERROR(
        "\n================== Shader compile failure ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        code ? code : "Shader code: <EMPTY>",
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}
