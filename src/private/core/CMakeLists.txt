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

# utilities to glob cxx source files
function(add_headers result folder)
    set(input ${${result}})
    file(GLOB sources ${folder}/*.hpp ${folder}/*.h ${folder}/*.h.in ${folder}/*.inl)
    set(${result} ${input} ${sources} PARENT_SCOPE)
endfunction()

function(add_sources result folder)
    set(input ${${result}})
    file(GLOB sources ${folder}/*.cpp ${folder}/*.c ${folder}/*.cxx ${folder}/*.hpp ${folder}/*.h ${folder}/*.h.in ${folder}/*.inl)
    set(${result} ${input} ${sources} PARENT_SCOPE)
endfunction()

function(add_files result pattern)
    set(input ${${result}})
    file(GLOB sources ${pattern})
    message(${sources})
    set(${result} ${input} ${sources} PARENT_SCOPE)
endfunction()

# gather sources
set(sources app/SampleApp.cpp)
add_sources(sources base)
add_sources(sources engine)
add_sources(sources gfx/base)
add_sources(sources gfx/d3d)
add_sources(sources gfx/gpures)
add_sources(sources gfx/misc)
add_sources(sources gpu/common)
add_headers(sources gpu/util)
add_files  (sources gpu/util/gpu*.cpp)
add_sources(sources input)
add_sources(sources util)
add_sources(sources win)

# OpenGL GPU Sources
if (GN_BUILD_HAS_OGL)
    add_sources(sources gpu/ogl)
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

# TODO: Gather libraries that GNcore or its dependents need to link to

if (GN_BUILD_IS_STATIC)
	add_library(GNcore ${sources} ${garnet_headers} ${BUILD_H})
else()
	add_library(GNcore SHARED ${sources} ${garnet_headers} ${BUILD_H})
endif()

if (GN_BUILD_HAS_XINPUT)
    target_compile_definitions(GNcore PRIVATE HAS_INPUT)
endif()
if (GN_BUILD_HAS_XINPUT2)
    target_compile_definitions(GNcore PRIVATE HAS_INPUT2)
endif()
if (GN_BUILD_HAS_DINPUT)
    target_compile_definitions(GNcore PRIVATE HAS_DINPUT)
endif()

target_include_directories(GNcore PUBLIC ../inc ${BUILD_INC})

target_link_libraries(GNcore PUBLIC GNextern)

target_compile_features(GNcore PUBLIC cxx_std_11)

if (MSVC)
    target_compile_options(GNcore PUBLIC /EHa /MP)
    target_compile_options(GNcore PRIVATE /utf-8 /W4 /WX)
endif()
