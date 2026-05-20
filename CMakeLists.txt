cmake_minimum_required(VERSION 3.16.3) # 3.16.3 is the default version coming with Ubuntu 20.04

project(garnet)

# ====================
# public configrations
# ====================

option(GN_BUILD_IS_STATIC "Set to ON to build GNcore as static library. Default is OFF" OFF)
option(GN_BUILD_SAMPLES "Set to ON to build samples. Default is ON." ON)
option(GN_BUILD_TOOLS "Set to ON to build tools. Default is ON." ON)
option(GN_BUILD_TESTS "Set to ON to build tests. Default is ON." ON)
option(GN_BUILD_DISABLE_ALL_WARINGS "Set to ON mute all compiler warnings. Default is OFF." OFF)

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
#     GN_add_source_folder(sources_list folder <RECURSIVE> <VERBOSE> <SOURCE_WILDCARDS ...> <HEADER_WILDCARDS ...> <PCH_SOURCE ...> <PCH_HEADER ...>)
# ------------------------------------------------------------------------------
function(GN_add_source_folder result folder)
    set(input ${${result}})

    # parse optional parameters
    set(options RECURSIVE VERBOSE)
    set(oneValueArgs PCH_SOURCE PCH_HEADER)
    set(multiValueArgs SOURCE_WILDCARDS HEADER_WILDCARDS EXTRA_SOURCES)
    cmake_parse_arguments(arg "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
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
    if (${arg_RECURSIVE})
        file(GLOB_RECURSE sources ${source_patterns})
    else()
        file(GLOB sources ${source_patterns})
    endif()
    if (${arg_VERBOSE})
        GN_print_status("Globbed source files: ${sources}")
    endif()

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
    if (${arg_VERBOSE})
        GN_print_status("Globbed header files: ${headers}")
    endif()

    # glob extra sources
    if(arg_EXTRA_SOURCES)
        file(GLOB extra_sources ${arg_EXTRA_SOURCES})
        if (${arg_VERBOSE})
            GN_print_status("Globbed extra sources: ${extra_sources}")
        endif()
    endif()

    # done
    set(${result} ${input} ${sources} ${headers} ${arg_PCH_SOURCE} ${extra_sources} PARENT_SCOPE)
endfunction()

# Compile GLSL source files to generated SPIR-V C++ headers.
#
# Usage:
#     GN_compile_glsl_shaders(out_headers
#         SOURCES shader.vert shader.frag
#         OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}
#         INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
#         DEPENDS extra-header.h
#         COMMENT "Compiling shaders")
# ------------------------------------------------------------------------------
function(GN_compile_glsl_shaders result)
    set(oneValueArgs OUT_DIR COMMENT)
    set(multiValueArgs SOURCES INCLUDE_DIRS DEPENDS)
    cmake_parse_arguments(arg "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT arg_SOURCES)
        message(FATAL_ERROR "GN_compile_glsl_shaders requires SOURCES")
    endif()

    if (NOT arg_OUT_DIR)
        set(arg_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    if (NOT arg_COMMENT)
        set(arg_COMMENT "Compiling GLSL shaders to SPIR-V headers")
    endif()

    find_package(Python3 REQUIRED)

    set(compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
    set(outputs)
    set(sources)
    foreach(source ${arg_SOURCES})
        if (IS_ABSOLUTE "${source}")
            set(source_path "${source}")
        else()
            set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
        endif()

        get_filename_component(source_name "${source_path}" NAME)
        string(REGEX REPLACE "\\.[^.]*$" "" source_stem "${source_name}")
        string(REGEX MATCH "[^.]*$" source_ext "${source_name}")
        list(APPEND outputs "${arg_OUT_DIR}/${source_stem}-${source_ext}.spv.h")
        list(APPEND sources "${source_path}")
    endforeach()

    set(include_args)
    foreach(include_dir ${arg_INCLUDE_DIRS})
        list(APPEND include_args -I "${include_dir}")
    endforeach()

    add_custom_command(
        OUTPUT ${outputs}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${arg_OUT_DIR}"
        COMMAND ${Python3_EXECUTABLE} "${compile_tool}"
                ${sources}
                --out-dir "${arg_OUT_DIR}"
                ${include_args}
        DEPENDS ${compile_tool} ${sources} ${arg_DEPENDS}
        COMMENT "${arg_COMMENT}"
        VERBATIM
    )

    set(${result} ${outputs} PARENT_SCOPE)
endfunction()

# =======================
# Detect current compiler
# =======================

if(CMAKE_COMPILER_IS_GNUCC)
    set(IS_GCC TRUE)
endif()
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(IS_CLANG TRUE)
endif()
if(IS_GCC OR IS_CLANG)
    set(IS_GNUC TRUE)
endif()

# =================================
# Configure Options and Build Flags
# =================================

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

# Set compiler specific options for garnet library. Note that these
# options are only applied to garnet library and its dependencies.
# They won't get populated to client modules who depends on garnet library.
if (MSVC)
    if (GN_BUILD_DISABLE_ALL_WARINGS)
        add_compile_options(/w)
    else()
        add_compile_options(/W4 /WX)
    endif()
    add_compile_options(
        /MP # enable parallel build
        $<$<CONFIG:Debug>:/GR> # enable rtti, in debug build only.
    )
    # TODO: replace /EHsc with /EHa?
else()
    if (GN_BUILD_DISABLE_ALL_WARINGS)
        add_compile_options(-w)
    else()
        add_compile_options(-Wall -Wextra -Werror)
    endif()
endif()

# setup repository wide cmake properties
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/env/cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# search commonly used packages
# set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
find_package(DirectX)
find_package(Vulkan)
find_package(X11)

# Check for CUDA support
if (NOT ANDROID)
    include(CheckLanguage)
    check_language(CUDA)
    if (CMAKE_CUDA_COMPILER)
        find_package(OptiX)
        find_package(CUDAToolkit)
        enable_language(CUDA OPTIONAL)
    endif()
endif()

if (APPLE AND VULKAN_FOUND)
# Setup LC_RPATH on macos to point to where libvulkan.dylib is. So the executable can load it at runtime.
# FIXME: this ties the executable to the Vulkan SDK install location on the current build machine. It means
# the executable might not run on other machines.
get_filename_component(Vulkan_LIB_DIR ${Vulkan_LIBRARY} DIRECTORY)
set(CMAKE_BUILD_RPATH ${Vulkan_LIB_DIR})
GN_print_status("Setting up LC_RPATH for Vulkan SDK on macOS: ${Vulkan_LIB_DIR}")
endif()

# ===================
# call subdirectories
# ===================
add_subdirectory(src)
