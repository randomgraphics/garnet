cmake_minimum_required(VERSION 3.16.3) # 3.16.3 is the default version coming with Ubuntu 20.04

project(garnet LANGUAGES C CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/env/cmake)
set (CMAKE_CXX_STANDARD 17)

# =================
# utility functions
# =================

macro(GN_print_status)
    message("[GARNET] ${ARGN}")
endmacro()

macro(GN_warning)
    message(WARNING "========================\n\n[GARNET] ${ARGN}\n\n========================")
endmacro()

# Setup PCH for source files
#
# Usage:
#    GN_setup_pch(source1 [source2 ...] PCH_SOURCE pch_source [PCH_HEADER pch_header])
#
# Default pch header is pch.h, if not specified.
# ------------------------------------------------------------------------------
function(GN_setup_pch)
    string(FIND ${CMAKE_MAKE_PROGRAM} MSBuild isMsBuild)
    if (${isMsBuild} GREATER 0)
        set(oneValueArgs PCH_SOURCE PCH_HEADER)
        cmake_parse_arguments(arg "" "${oneValueArgs}" "" ${ARGN})
        if (arg_UNPARSED_ARGUMENTS AND arg_PCH_SOURCE)
            if (NOT arg_PCH_HEADER)
                set(arg_PCH_HEADER pch.h)
            endif()
            # remove PCH_SOURCE from source list
            list(REMOVE_ITEM arg_UNPARSED_ARGUMENTS ${CMAKE_CURRENT_SOURCE_DIR}/${arg_PCH_SOURCE})
            list(REMOVE_ITEM arg_UNPARSED_ARGUMENTS ${arg_PCH_SOURCE})
            if (MSVC)
                set(pch_output ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${arg_PCH_SOURCE}.pch)
                set_property(SOURCE ${arg_UNPARSED_ARGUMENTS} APPEND_STRING PROPERTY COMPILE_FLAGS "/Yu${arg_PCH_HEADER} /Fp${pch_output}")
                set_property(SOURCE ${arg_PCH_SOURCE} APPEND_STRING PROPERTY COMPILE_FLAGS "/Yc${arg_PCH_HEADER} /Fp${pch_output}")
            else()
                # todo: gcc support
            endif()
        endif()
    endif()
endfunction()

# Append source files from specific folder to list specified by parameter 'result'
# Setup PCH if specified.
#
# Usage:
#     GN_add_source_folder(sources_list folder <SOURCE_WILDCARDS ...> <HEADER_WILDCARDS ...> <PCH_SOURCE ...> <PCH_HEADER ...>)
# ------------------------------------------------------------------------------
function(GN_add_source_folder result folder)
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
    if (arg_PCH_SOURCE)
        list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/${arg_PCH_SOURCE})
		GN_setup_pch(${sources} PCH_SOURCE ${arg_PCH_SOURCE} PCH_HEADER ${arg_PCH_HEADER})
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
if (NOT GN_BUILD_IS_STATIC)
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()

# check build type
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    # debug build
    set(GN_BUILD_DEBUG_ENABLED ON)
    set(GN_BUILD_PROFILING_ENABLED ON)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    # profile build
    set(GN_BUILD_DEBUG_ENABLED OFF)
    set(GN_BUILD_PROFILING_ENABLED ON)
else()
    # release build
    set(GN_BUILD_DEBUG_ENABLED OFF)
    set(GN_BUILD_PROFILING_ENABLED OFF)
endif()

# runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# search commonly used packages
# set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
find_package(DirectX)
find_package(Vulkan)
find_package(X11)
find_package(Qt6 COMPONENTS Core Widgets)
find_package(CUDAToolkit)
find_package(OptiX)

# Check for CUDA support
include(CheckLanguage)
check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA OPTIONAL)
endif()

# ===================
# call subdirectories
# ===================
add_subdirectory(src)
