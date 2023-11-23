# =======================
# generate feature header
# =======================

set(GN_BUILD_HAS_OGL ${OPENGL_FOUND})
set(GN_BUILD_HAS_D3D9 ${D3D9_FOUND})
set(GN_BUILD_HAS_D3D11 ${D3D11_FOUND})
set(GN_BUILD_HAS_D3D11_1 ${D3D11_FOUND})
set(GN_BUILD_HAS_D3D12 ${D3D12_FOUND})
set(GN_BUILD_HAS_XNAMATH OFF) # this is legacy flag that is available on pre-vista OS and Xbox2
set(GN_BUILD_HAS_VULKAN ${VULKAN_FOUND})

configure_file(../inc/garnet/base/features.h.in ../inc/garnet/base/features.h)

set(BUILD_HEADER_INC ${CMAKE_CURRENT_BINARY_DIR}/../inc/garnet)

# print configuration summary
# print summary
GN_print_status("CMAKE_MAKE_PROGRAM         = ${CMAKE_MAKE_PROGRAM}")
GN_print_status("CMAKE_BUILD_TYPE           = ${CMAKE_BUILD_TYPE}")
GN_print_status("CMAKE_SIZEOF_VOID_P        = ${CMAKE_SIZEOF_VOID_P}")
GN_print_status("GN_BUILD_IS_STATIC         = ${GN_BUILD_IS_STATIC}")
GN_print_status("GN_BUILD_DEBUG_ENABLED     = ${GN_BUILD_DEBUG_ENABLED}")
GN_print_status("GN_BUILD_PROFILING_ENABLED = ${GN_BUILD_PROFILING_ENABLED}")
GN_print_status("GN_BUILD_HAS_OGL           = ${GN_BUILD_HAS_OGL}")
GN_print_status("GN_BUILD_HAS_D3D9          = ${GN_BUILD_HAS_D3D9}")
GN_print_status("GN_BUILD_HAS_D3D11         = ${GN_BUILD_HAS_D3D11}")
GN_print_status("GN_BUILD_HAS_D3D11_1       = ${GN_BUILD_HAS_D3D11_1}")
GN_print_status("GN_BUILD_HAS_D3D12         = ${GN_BUILD_HAS_D3D12}")
GN_print_status("GN_BUILD_HAS_VULKAN        = ${GN_BUILD_HAS_VULKAN}")

# ====================
# Collect source files
# ====================

# gather headers
file(GLOB_RECURSE garnet_headers ../inc/*.h ../inc/*.inl ../inc/*.in)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../inc PREFIX "Header Files" FILES ${garnet_headers})
list(APPEND garnet_headers ${BUILD_HEADER_INC}/base/features.h)

# gather sources
set(sources app/sampleApp.cpp)
GN_add_source_folder(sources base PCH_SOURCE base/pch.cpp)
GN_add_source_folder(sources engine PCH_SOURCE engine/pch.cpp)
GN_add_source_folder(sources gfx/base PCH_SOURCE gfx/base/pch.cpp)
GN_add_source_folder(sources gfx/d3d PCH_SOURCE gfx/d3d/pch.cpp)
GN_add_source_folder(sources gfx/gpures PCH_SOURCE gfx/gpures/pch.cpp)
GN_add_source_folder(sources gfx/misc PCH_SOURCE gfx/misc/pch.cpp)
GN_add_source_folder(sources gfx/rt PCH_SOURCE gfx/rt/pch.cpp)
GN_add_source_folder(sources gpu/common PCH_SOURCE gpu/common/pch.cpp)
GN_add_source_folder(sources gpu/util PCH_SOURCE gpu/util/pch.cpp SOURCE_WILDCARDS gpu*.cpp)
GN_add_source_folder(sources gpu2)
GN_add_source_folder(sources input PCH_SOURCE input/pch.cpp)
GN_add_source_folder(sources util PCH_SOURCE util/pch.cpp)
GN_add_source_folder(sources win PCH_SOURCE win/pch.cpp)

# OpenGL GPU Sources
if(OPENGL_FOUND)
    GN_add_source_folder(sources gpu/ogl PCH_SOURCE gpu/ogl/pch.cpp)
    GN_add_source_folder(sources gfx/ogl PCH_SOURCE gfx/ogl/pch.cpp)
endif()

# D3D11 GPU Sources
if(D3D11_FOUND)
    GN_add_source_folder(sources gpu/d3d11 PCH_SOURCE gpu/d3d11/pch.cpp)
endif()

# D3D12 GPU Sources
if(D3D12_FOUND)
    GN_add_source_folder(sources gpu2/d3d12 PCH_SOURCE gpu2/d3d12/pch.cpp)
endif()

# if(VULKAN_FOUND)
#     GN_add_source_folder(sources vulkan RECURSIVE PCH_SOURCE vulkan/pch.cpp)
# endif()

# Split everything into groups
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${sources})

# ====================
# Build GNcore library
# ====================
if(GN_BUILD_IS_STATIC)
    add_library(GNcore ${sources} ${garnet_headers})
else()
    add_library(GNcore SHARED ${sources} ${garnet_headers})
endif()

# =======================
# setup GNcore properties
# =======================
function(add_core_libs)
    if(GN_BUILD_IS_STATIC)
        target_link_libraries(GNcore PUBLIC ${ARGN})
    else()
        target_link_libraries(GNcore PRIVATE ${ARGN})
    endif()
endfunction()

target_include_directories(GNcore PUBLIC
    ${BUILD_HEADER_INC}
    ../inc
    ../3rdparty/Eigen
)
target_link_libraries(GNcore PUBLIC GNextern)
target_compile_features(GNcore PUBLIC cxx_std_17)

# Qt6
if(Qt6Widgets_FOUND)
    add_core_libs(Qt::Widgets)
    target_compile_definitions(GNcore PRIVATE HAS_QT=1)
else()
    message("QT not found. QT window is disabled")
endif()

# directx
if(D3D11_FOUND)
    target_compile_definitions(GNcore PRIVATE HAS_XINPUT=1)
    add_core_libs(${DIRECTX_LIBRARIES})
endif()

# X11
if(X11_FOUND)
    target_compile_definitions(GNcore PRIVATE HAS_X11=1)
endif()

# FBX
if(WIN32)
    set(fbx_win_root_dir ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/fbx/2014.1)

    if(8 EQUAL CMAKE_SIZEOF_VOID_P)
        set(fbx_win_lib_dir ${fbx_win_root_dir}/lib/vs2010/x64/release)
    else()
        set(fbx_win_lib_dir ${fbx_win_root_dir}/lib/vs2010/x86/release)
    endif()

    target_include_directories(GNcore PRIVATE ${fbx_win_root_dir}/include)
    target_compile_definitions(GNcore PRIVATE HAS_FBX=1)
    add_core_libs(${fbx_win_lib_dir}/libfbxsdk.lib)
    file(COPY ${fbx_win_lib_dir}/libfbxsdk.dll DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")
    file(COPY ${fbx_win_lib_dir}/libfbxsdk.dll DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release")
endif()

# Vulkan
if(VULKAN_FOUND)
    target_compile_definitions(GNcore PRIVATE HAS_VULKAN=1)
    # only include Vulkan headers. Do not link to Vulkan libraries, since it
    # will be conflict with the symbols defined in volk.h
    target_include_directories(GNcore PUBLIC ${Vulkan_INCLUDE_DIRS})
endif()

# thread libraries
find_package(Threads REQUIRED)
add_core_libs(Threads::Threads)

# Assimp
add_core_libs(assimp)

# compiler specific setup
if(MSVC)
    target_compile_definitions(GNcore PUBLIC _UNICODE UNICODE)
    target_compile_options(GNcore PUBLIC /EHa /MP /W4 /WX
        /Wv:18 # fix eigen warning
    )
else()
    target_compile_options(GNcore PUBLIC -Wall -Wextra -Werror -Wfatal-errors)
endif()
