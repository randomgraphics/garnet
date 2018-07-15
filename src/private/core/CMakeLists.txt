# ==============================
# generate configuration headers
# ==============================

configure_file(../inc/garnet/base/build.h.in ../inc/garnet/base/build.h)
set(BUILD_INC ${CMAKE_CURRENT_BINARY_DIR}/../inc/garnet)
set(BUILD_H ${BUILD_INC}/base/build.h)

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

# D3D GPU Sources
if (GN_BUILD_HAS_D3D11)
    add_sources(sources gpu/d3d11 PCH_SOURCE gpu/d3d11/pch.cpp)
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
set(private_incs ../../contrib/inc)
set(public_incs ../inc ${BUILD_INC})
set(private_libs)
set(public_libs GNextern)
set(private_defs)
set(public_defs)

# check xinput/dinput
if (D3D11_FOUND)
    list(APPEND private_defs HAS_INPUT=1)
    list(APPEND private_defs HAS_INPUT2=1)
endif()

#check X11
find_package(X11)
if (X11_FOUND)
    message("X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
    message("X11_LIBRARIES   = ${X11_LIBRARIES}")
    list(APPEND private_incs ${X11_INCLUDE_DIR})
    list(APPEND private_defs HAS_X11=1)
endif()

# build the library
set(deps )
if (GN_BUILD_IS_STATIC)
	add_library(GNcore ${sources} ${garnet_headers} ${BUILD_H})
    list(APPEND public_libs ${OPENGL_LIBRARIES} ${DIRECTX_LIBRARIES} ${X11_LIBRARIES})
else()
	add_library(GNcore SHARED ${sources} ${garnet_headers} ${BUILD_H})
    list(APPEND private_libs ${OPENGL_LIBRARIES} ${DIRECTX_LIBRARIES} ${X11_LIBRARIES})
endif()

# setup compile properties
target_compile_definitions(GNcore PRIVATE ${private_defs} PUBLIC ${public_defs})
target_include_directories(GNcore PRIVATE ${private_incs} PUBLIC ${public_incs})
target_link_libraries     (GNcore PRIVATE ${private_libs} PUBLIC ${public_libs})
target_compile_features   (GNcore PUBLIC cxx_std_11)
if (MSVC)
    target_compile_options(GNcore PUBLIC /EHa /MP PRIVATE /utf-8 /W4 /WX)
endif()
