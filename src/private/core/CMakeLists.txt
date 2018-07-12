# ==============================
# generate configuration headers
# ==============================

# Build type
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

configure_file(../inc/garnet/base/build.h.in ../inc/garnet/base/build.h)
set(BUILD_INC ${CMAKE_CURRENT_BINARY_DIR}/../inc/garnet)
set(BUILD_H ${BUILD_INC}/base/build.h)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message("GN_BUILD_DEBUG_ENABLED = ${GN_BUILD_DEBUG_ENABLED}")
message("GN_BUILD_PROFILING_ENABLED = ${GN_BUILD_PROFILING_ENABLED}")
message("GN_BUILD_HAS_OGL = ${GN_BUILD_HAS_OGL}")
message("GN_BUILD_HAS_D3D9 = ${GN_BUILD_HAS_D3D9}")
message("GN_BUILD_HAS_D3D11 = ${GN_BUILD_HAS_D3D11}")
message("GN_BUILD_HAS_D3D11_1 = ${GN_BUILD_HAS_D3D11_1}")

# ====================
# Collect source files
# ====================

# gather headers
file(GLOB_RECURSE garnet_headers ../inc/*.h ../inc/*.inl ../inc/*.in)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../inc PREFIX "Header Files" FILES ${garnet_headers})

# gather sources
set(sources app/SampleApp.cpp)
add_sources(sources base PCH_SOURCE base/pch.cpp)
add_sources(sources engine PCH_SOURCE engine/pch.cpp)
add_sources(sources gfx/base PCH_SOURCE gfx/base/pch.cpp)
add_sources(sources gfx/d3d PCH_SOURCE gfx/d3d/pch.cpp)
add_sources(sources gfx/gpures PCH_SOURCE gfx/gpures/pch.cpp)
add_sources(sources gfx/misc PCH_SOURCE gfx/misc/pch.cpp)
add_sources(sources gpu/common PCH_SOURCE gpu/common/pch.cpp)
add_sources(sources gpu/util PCH_SOURCE gpu/util/pch.cpp SOURCE_WILDCARDS gpu*.cpp)
add_sources(sources input PCH_SOURCE input/pch.cpp)
add_sources(sources util PCH_SOURCE util/pch.cpp)
add_sources(sources win PCH_SOURCE win/pch.cpp)

# OpenGL GPU Sources
if (GN_BUILD_HAS_OGL)
    add_sources(sources gpu/ogl PCH_SOURCE gpu/ogl/pch.cpp)
endif()

# Qt5
find_package(Qt5 COMPONENTS Core Widgets)
if (NOT HAS_QT5_Widgets)
	message("Qt5 not found. Remove windowQt.cpp from sources")
	list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/win/windowQt.cpp)
endif()

 # Split everything into groups
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${sources})

# ====================
# Build GNcore library
# ====================

# initialize local compile properties.
# this is to ensures we are not accidentally inheriting them from outer cmake files.
set(private_incs)
set(public_incs ../inc ${BUILD_INC})
set(private_libs)
set(public_libs GNextern)
set(private_defs)
set(public_defs)

# check xinput/dinput
if (GN_BUILD_HAS_XINPUT)
    list(APPEND private_defs HAS_INPUT=1)
endif()
if (GN_BUILD_HAS_XINPUT2)
    list(APPEND private_defs HAS_INPUT2=1)
endif()
if (GN_BUILD_HAS_DINPUT)
    list(APPEND private_defs HAS_DINPUT=1)
endif()

# build the library
if (GN_BUILD_IS_STATIC)
	add_library(GNcore ${sources} ${garnet_headers} ${BUILD_H})
    list(APPEND public_libs ${OPENGL_LIBRARIES})
else()
	add_library(GNcore SHARED ${sources} ${garnet_headers} ${BUILD_H})
    list(APPEND private_libs ${OPENGL_LIBRARIES})
endif()

# setup compile properties
target_compile_definitions(GNcore PUBLIC ${public_defs} PRIVATE ${private_defs})
target_include_directories(GNcore PUBLIC ${public_incs} PRIVATE ${private_incs})
target_link_libraries     (GNcore PUBLIC ${public_libs} PRIVATE ${private_libs})
target_compile_features   (GNcore PUBLIC cxx_std_11)
if (MSVC)
    target_compile_options(GNcore PUBLIC /EHa /MP PRIVATE /utf-8 /W4 /WX)
endif()
