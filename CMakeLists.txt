cmake_minimum_required(VERSION 3.9)
project(garnet)

include(CMakeDependentOption)

# ===========================
# define global build options
# ===========================

option(GN_BUILD_DEBUG_ENABLED "Set to ON|OFF to enable|disable debug build." ON)
message("GN_BUILD_DEBUG_ENABLED=${GN_BUILD_DEBUG_ENABLED}")

# todo: force it on, if debug is on
option(GN_BUILD_PROFILING_ENABLED "Set to ON|OFF to enable|disable profile build." ON)
message("GN_BUILD_PROFILING_ENABLED=${GN_BUILD_PROFILING_ENABLED}")

option(GN_BUILD_IS_STATIC "Set to ON|OFF to build GNcore as static|shared library." OFF)
message("GN_BUILD_IS_STATIC=${GN_BUILD_IS_STATIC}")

# =============================
# search commonly used packages
# =============================

# OpenGL
find_package(OpenGL)
set(GN_BUILD_HAS_OGL ${OPENGL_FOUND})

# TODO: DirectX
set(GN_BUILD_HAS_D3D9 OFF)
set(GN_BUILD_HAS_D3D11 OFF)
set(GN_BUILD_HAS_D3D11_1 OFF)
set(GN_BUILD_HAS_D3DX9 OFF)
set(GN_BUILD_HAS_DXERR OFF)
set(GN_BUILD_HAS_D3DCOMPILER OFF)
set(GN_BUILD_HAS_XNAMATH OFF)
set(GN_BUILD_HAS_DIRECTXMATH OFF)

# =============================
# define global build variables
# =============================

set(GN_ROOT_DIR ${PROJECT_SOURCE_DIR})
set(GN_BUILD_ROOT_DIR ${PROJECT_BINARY_DIR})

# ============================
# search common compiler flags
# ============================

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_C_FLAGS "/utf-8 /EHsc /W4 /WX /Zi /MP")
    set(CMAKE_CXX_FLAGS "/utf-8 /EHsc /W4 /WX /Zi /MP")
endif()

# ===================
# call subdirectories
# ===================
add_subdirectory(src/private/core)
add_subdirectory(sconstest)
