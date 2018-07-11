cmake_minimum_required(VERSION 3.9)
project(garnet)

include(CMakeDependentOption)

# ====================
# define build options
# ====================

option(GN_BUILD_IS_STATIC "Set to ON|OFF to build GNcore as static|shared library." OFF)
message("GN_BUILD_IS_STATIC = ${GN_BUILD_IS_STATIC}")

# ===================
# call subdirectories
# ===================
add_subdirectory(src/contrib/src)
add_subdirectory(src/private/core)
add_subdirectory(sconstest)
