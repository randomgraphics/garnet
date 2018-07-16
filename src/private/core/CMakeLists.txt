# ==============================
# generate configuration headers
# ==============================

set(GN_BUILD_HAS_OGL ${OPENGL_FOUND})
set(GN_BUILD_HAS_D3D9 ${D3D9_FOUND})
set(GN_BUILD_HAS_D3D11 ${D3D11_FOUND})
set(GN_BUILD_HAS_D3D11_1 ${D3D11_FOUND})
set(GN_BUILD_HAS_XNAMATH OFF) # this is legacy flag that is available on pre-vista OS and Xbox2

configure_file(../inc/garnet/base/build.h.in ../inc/garnet/base/build.h)

set(BUILD_INC ${CMAKE_CURRENT_BINARY_DIR}/../inc/garnet)
set(BUILD_H ${BUILD_INC}/base/build.h)

# print configuration summary
message("GN_BUILD_HAS_OGL     = ${GN_BUILD_HAS_OGL}")
message("GN_BUILD_HAS_D3D9    = ${GN_BUILD_HAS_D3D9}")
message("GN_BUILD_HAS_D3D11   = ${GN_BUILD_HAS_D3D11}")
message("GN_BUILD_HAS_D3D11_1 = ${GN_BUILD_HAS_D3D11_1}")

# ====================
# Collect source files
# ====================

# gather headers
file(GLOB_RECURSE garnet_headers ../inc/*.h ../inc/*.inl ../inc/*.in)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../inc PREFIX "Header Files" FILES ${garnet_headers})

# gather sources
set(sources app/sampleApp.cpp)
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
if (OPENGL_FOUND)
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

if (GN_BUILD_IS_STATIC)
    add_library(GNcore ${sources} ${garnet_headers} ${BUILD_H})
else()
    add_library(GNcore SHARED ${sources} ${garnet_headers} ${BUILD_H})
endif()

# =======================
# setup GNcore properties
# =======================

target_include_directories(GNcore PUBLIC ${BUILD_INC} ../inc)
target_compile_features   (GNcore PUBLIC cxx_std_11)

# directx
if (D3D11_FOUND)
    target_compile_definitions(GNcore PRIVATE HAS_XINPUT=1)
endif()

# X11
if (X11_FOUND)
    target_compile_definitions(GNcore PRIVATE HAS_X11=1)
endif()

#check thread libraries
find_package(Threads REQUIRED)

# link libraries
set(libs GNextern ${DIRECTX_LIBRARIES} ${CMAKE_DL_LIBS} Threads::Threads)
if (GN_BUILD_IS_STATIC)
    target_link_libraries(GNcore PUBLIC ${libs})
else()
    target_link_libraries(GNcore PRIVATE ${libs})
endif()

# compiler specific setup
if (MSVC)
    target_compile_options(GNcore PUBLIC /EHa /MP PRIVATE /utf-8 /W4 /WX)
endif()
