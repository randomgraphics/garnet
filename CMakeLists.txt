cmake_minimum_required(VERSION 3.9)
project(garnet)

include(CMakeDependentOption)

# ============================
# define global config options
# ============================

option(GN_BUILD_IS_STATIC "Set to ON|OFF to build GNcore as static|shared library." OFF)

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
set(GN_BUILD_HAS_XINPUT OFF)
set(GN_BUILD_HAS_XINPUT2 OFF)
set(GN_BUILD_HAS_DINPUT OFF)

# =======================
# define global constants
# =======================

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(GN_BUILD_DEBUG_ENABLED ON)
    set(GN_BUILD_PROFILING_ENABLED ON)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    set(GN_BUILD_DEBUG_ENABLED OFF)
    set(GN_BUILD_PROFILING_ENABLED ON)
else()
    set(GN_BUILD_DEBUG_ENABLED OFF)
    set(GN_BUILD_PROFILING_ENABLED OFF)
endif()

set(GN_ROOT_DIR ${PROJECT_SOURCE_DIR})
set(GN_BUILD_ROOT_DIR ${PROJECT_BINARY_DIR})

# ===================
# call subdirectories
# ===================
add_subdirectory(src/contrib/src)
add_subdirectory(src/private/core)
add_subdirectory(sconstest)

# =======================
# print configure summary
# =======================
message("CMAKE_BUILD_TYPE           = ${CMAKE_BUILD_TYPE}")
message("GN_BUILD_IS_STATIC         = ${GN_BUILD_IS_STATIC}")
message("GN_BUILD_DEBUG_ENABLED     = ${GN_BUILD_DEBUG_ENABLED}")
message("GN_BUILD_PROFILING_ENABLED = ${GN_BUILD_PROFILING_ENABLED}")
message("GN_BUILD_HAS_OGL           = ${GN_BUILD_HAS_OGL}")
message("GN_BUILD_HAS_D3D9          = ${GN_BUILD_HAS_D3D9}")
message("GN_BUILD_HAS_D3D11         = ${GN_BUILD_HAS_D3D11}")
message("GN_BUILD_HAS_D3D11_1       = ${GN_BUILD_HAS_D3D11_1}")
