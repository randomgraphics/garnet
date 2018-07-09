cmake_minimum_required(VERSION 3.11)
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

# print options summary

# =============================
# define global build variables
# =============================

set(GN_ROOT_DIR ${PROJECT_SOURCE_DIR})
set(GN_BUILD_ROOT_DIR ${PROJECT_BINARY_DIR})

# configure build variant
#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_DIRECTORY build.tmp/${CMAKE_SYSTEM_NAME}.${CMAKE_SYSTEM_PROCESSOR}.${COMPILERNAME}${COMPILER_VERSION}.${CMAKE_BUILD_TYPE})

# ===================
# call subdirectories
# ===================
add_subdirectory(src/private/core)
add_subdirectory(sconstest)
