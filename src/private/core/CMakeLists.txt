# ==============================
# generate configuration headers
# ==============================

configure_file(../inc/garnet/base/build.h.in ../inc/garnet/base/build.h)

# ====================
# Collect source files
# ====================

# headers
file(GLOB_RECURSE garnet_headers ../inc/*.h ../inc/*.inl ../inc/*.in)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../inc PREFIX "Header Files" FILES ${garnet_headers})

# Sources
file(GLOB sources
    app/SampleApp.cpp
    base/*.cpp base/*.h
    engine/*.cpp engine/*.h
    gfx/base/*.cpp gfx/base/*.h
    #gfx/d3d/*.cpp gfx/d3d/*.h
    gfx/gpures/*.cpp gfx/gpures/*.h
    gfx/misc/*.cpp gfx/misc/*.h
    input/*.cpp input/*.h
    util/*.cpp util/*.h
    win/*.cpp win/*.h
)

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
	add_library(GNcore ${sources} ${garnet_headers} ${GN_BUILD_ROOT_DIR}/src/private/inc/garnet/base/build.h)
else()
	add_library(GNcore SHARED ${sources} ${garnet_headers} ${GN_BUILD_ROOT_DIR}/src/private/inc/garnet/base/build.h)
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

target_include_directories(GNcore PUBLIC ${GN_ROOT_DIR}/src/private/inc ${GN_BUILD_ROOT_DIR}/src/private/inc/garnet)

target_link_libraries(GNcore PUBLIC GNextern)

target_compile_features(GNcore PUBLIC cxx_std_11)

if (MSVC)
    target_compile_options(GNcore PUBLIC /EHa /MP)
    target_compile_options(GNcore PRIVATE /utf-8 /W4 /WX)
endif()
