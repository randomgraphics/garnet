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
set(D3D12_FOUND)
set(D3DCOMPILER_FOUND)
set(DXGI_FOUND)
set(XINPUT_FOUND)
set(DIRECTX_LIBRARIES)

if(WIN32)
    # reset CMAKE_C_FLAGS and CMAKE_CXX_FLAGS to default value before the check to avoid user's settings affect the check
    set(CMAKE_C_FLAGS)
    set(CMAKE_CXX_FLAGS)

    INCLUDE(CheckIncludeFiles)
    # INCLUDE(CheckFunctionExists)

    check_include_files("windows.h;d3d9.h" D3D9_FOUND)
    if (D3D9_FOUND)
        list(APPEND DIRECTX_LIBRARIES d3d9.lib)
    endif()

    check_include_files("windows.h;d3d11.h" D3D11_FOUND)
    check_include_files("windows.h;d3d11_1.h" D3D11_1_FOUND)
    if (D3D11_FOUND)
        list(APPEND DIRECTX_LIBRARIES d3d11.lib)
    endif()

    check_include_files("windows.h;d3d12.h" D3D12_FOUND)
    if (D3D12_FOUND)
        list(APPEND DIRECTX_LIBRARIES d3d12.lib)
    endif()

    check_include_files("windows.h;d3dcompiler.h" D3DCOMPILER_FOUND)
    if (D3DCOMPILER_FOUND)
        list(APPEND DIRECTX_LIBRARIES d3dcompiler.lib)
    endif()

    check_include_files("windows.h;dxgi.h" DXGI_FOUND)
    if (DXGI_FOUND)
        list(APPEND DIRECTX_LIBRARIES dxgi.lib dxguid.lib)
    endif()

    check_include_files("windows.h;xinput.h" XINPUT_FOUND)
    if (XINPUT_FOUND)
        list(APPEND DIRECTX_LIBRARIES xinput.lib)
    endif()

    message("DIRECTX_LIBRARIES = ${DIRECTX_LIBRARIES}")

endif(WIN32)
