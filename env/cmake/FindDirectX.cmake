# -----------------------------------------------------------------------------
# Find DirectX SDK
# Define:
# D3D9_FOUND
# D3DX9_FOUND
# D3D11_FOUND
# D3D11_1_FOUND
# DWIRTE_FOUND
# DXERR_FOUND
# XINPUT_FOUND
# XINPUT2_FOUND
# DIRECTXMATH_FOUND
# DIRECTX_LIBRARIES

set(D3D9_FOUND)
set(D3D11_FOUND)
set(D3D11_1_FOUND)
set(DXERR_FOUND)
set(XINPUT_FOUND)
set(XINPUT2_FOUND)
set(XNAMATH_FOUND)
set(DIRECTX_LIBRARIES)

if(WIN32)

    INCLUDE(CheckIncludeFile)
    INCLUDE(CheckFunctionExists)

    check_include_file(d3d9.h D3D9_FOUND)
    if (D3D9_FOUND)
        list(APPEND DIRECTX_LIBRARIES d3d9.lib)
    endif()

    check_include_file(d3dx9.h D3DX9_FOUND)
    if (D3DX9_FOUND)
        if (CMAKE_BUILD_TYPE STREQUAL Debug)
            list(APPEND DIRECTX_LIBRARIES d3dx9d.lib)
        else()
            list(APPEND DIRECTX_LIBRARIES d3dx9d.lib)
        endif()
    endif()

    check_include_file(dxerr.h DXERR_FOUND)
    if (DXERR_FOUND)
        list(APPEND DIRECTX_LIBRARIES dxerr.lib)
    endif()

    check_include_file(d3d11.h D3D11_FOUND)
    check_include_file(d3d11_1.h D3D11_1_FOUND)
    if (D3D11_FOUND)
        set(D3DCOMPILER_FOUND ${D3D11_FOUND})
        set(XINPUT_FOUND ${D3D11_FOUND})
        set(XINPUT2_FOUND ${D3D11_FOUND})
        set(DIRECTXMATH_FOUND ${D3D11_FOUND})
        list(APPEND DIRECTX_LIBRARIES dxgi.lib dxguid.lib d3d11.lib dwrite.lib d2d1.lib d3dcompiler.lib xinput.lib)
    endif()

    message("DIRECTX_LIBRARIES = ${DIRECTX_LIBRARIES}")

endif(WIN32)