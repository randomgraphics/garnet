# -----------------------------------------------------------------------------
# Find DirectX SDK
# Define:
# D3D9_FOUND
# D3D11_FOUND
# D3D11_1_FOUND
# D3D12_FOUND
# DIRECTX_LIBRARIES

set(D3D9_FOUND)
set(D3D11_FOUND)
set(D3D11_1_FOUND)
set(DIRECTX_LIBRARIES)

if(WIN32)

    INCLUDE(CheckIncludeFile)
    INCLUDE(CheckFunctionExists)

    check_include_file(d3d9.h D3D9_FOUND)
    if (D3D9_FOUND)
        list(APPEND DIRECTX_LIBRARIES d3d9.lib)
    endif()

    check_include_file(d3d11.h D3D11_FOUND)
    check_include_file(d3d11_1.h D3D11_1_FOUND)
    if (D3D11_FOUND)
        list(APPEND DIRECTX_LIBRARIES dxgi.lib dxguid.lib d3d11.lib dwrite.lib d2d1.lib d3dcompiler.lib xinput.lib)
    endif()

    check_include_file(d3d12.h D3D12_FOUND)
    if (D3D12_FOUND)
        list(APPEND DIRECTX_LIBRARIES dxgi.lib dxguid.lib d3d12.lib dwrite.lib d2d1.lib d3dcompiler.lib xinput.lib)
    endif()

    message("DIRECTX_LIBRARIES = ${DIRECTX_LIBRARIES}")

endif(WIN32)
