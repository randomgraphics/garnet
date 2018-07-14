cmake_minimum_required(VERSION 3.9)
project(garnet)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/env/cmake)
message(${CMAKE_MODULE_PATH})

# =================
# utility functions
# =================

# Append source files from specific folder to list specified by parameter 'result'
# Setup PCH if specified.
#
# Usage:
#     add_sources(sources_list folder <pch_source> <pch_header>=pch.h)
# ------------------------------------------------------------------------------
function(add_sources result folder)
    set(input ${${result}})

    # parse optional parameters
    set(oneValueArgs PCH_SOURCE PCH_HEADER)
    set(multiValueArgs SOURCE_WILDCARDS HEADER_WILDCARDS)
    cmake_parse_arguments(arg "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if (NOT arg_SOURCE_WILDCARDS)
        set(arg_SOURCE_WILDCARDS *.cpp *.cxx *.c)
    endif()
    if (NOT arg_HEADER_WILDCARDS)
        set(arg_HEADER_WILDCARDS *.h *.hpp *.inc *.inl *.h.in)
    endif()

    # glob source files
    foreach(x ${arg_SOURCE_WILDCARDS})
        list(APPEND source_patterns ${folder}/${x})
    endforeach()
    file(GLOB sources ${source_patterns})

    # setup PCH if specified
    #list(LENGTH ARGN len_argn)
    #if (len_argn GREATER_EQUAL 1)
    if (arg_PCH_SOURCE)
        if (NOT arg_PCH_HEADER)
            set(arg_PCH_HEADER pch.h)
        endif()
        list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/${arg_PCH_SOURCE})
        if (MSVC)
            set(pch_output ${CMAKE_CURRENT_BINARY_DIR}/${arg_PCH_SOURCE}.pch)
            set_property(SOURCE ${sources} APPEND_STRING PROPERTY COMPILE_FLAGS "/Yu${arg_PCH_HEADER} /Fp${pch_output}")
            set_property(SOURCE ${arg_PCH_SOURCE} APPEND_STRING PROPERTY COMPILE_FLAGS "/Yc${arg_PCH_HEADER} /Fp${pch_output}")
        else()
            # todo: gcc support
        endif()
    endif()

    # glob headers
    foreach(x ${arg_HEADER_WILDCARDS})
        list(APPEND header_patterns ${folder}/${x})
    endforeach()
    file(GLOB headers ${header_patterns})

    # done
    set(${result} ${input} ${sources} ${headers} ${arg_PCH_SOURCE} PARENT_SCOPE)
endfunction()

# =================================
# Configure Options and Build Flags
# =================================

option(GN_BUILD_IS_STATIC "Set to ON|OFF to build GNcore as static|shared library." OFF)

# OpenGL
find_package(OpenGL)
set(GN_BUILD_HAS_OGL ${OPENGL_FOUND})

# DirectX
find_package(DirectX)
set(GN_BUILD_HAS_D3D9 ${D3D9_FOUND})
set(GN_BUILD_HAS_D3D11 ${D3D11_FOUND})
set(GN_BUILD_HAS_D3D11_1 ${D3D11_FOUND})
set(GN_BUILD_HAS_XNAMATH OFF) # this is legacy flag that is available on pre-vista OS and Xbox2

# check build type
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

# print summary
message("GN_BUILD_IS_STATIC         = ${GN_BUILD_IS_STATIC}")
message("CMAKE_BUILD_TYPE           = ${CMAKE_BUILD_TYPE}")
message("GN_BUILD_DEBUG_ENABLED     = ${GN_BUILD_DEBUG_ENABLED}")
message("GN_BUILD_PROFILING_ENABLED = ${GN_BUILD_PROFILING_ENABLED}")
message("GN_BUILD_HAS_OGL           = ${GN_BUILD_HAS_OGL}")
message("GN_BUILD_HAS_D3D9          = ${GN_BUILD_HAS_D3D9}")
message("GN_BUILD_HAS_D3D11         = ${GN_BUILD_HAS_D3D11}")
message("GN_BUILD_HAS_D3D11_1       = ${GN_BUILD_HAS_D3D11_1}")

# ===================
# call subdirectories
# ===================
add_subdirectory(src/contrib/src)
add_subdirectory(src/private)
add_subdirectory(sconstest)
