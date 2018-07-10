# generate configuration headers
configure_file(../inc/garnet/base/build.h.in ../inc/garnet/base/build.h)

# Collect header files
file(GLOB_RECURSE garnet_headers ../inc/*.h ../inc/*.inl ../inc/*.in)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../inc PREFIX "Header Files" FILES ${garnet_headers})

# Collect source files
file(GLOB sources
	app/SampleApp.cpp app/*.h
	base/*.cpp base/*.h
	win/*.cpp win/*.h
)
find_package(Qt5 COMPONENTS Core Widgets)
if (NOT HAS_QT5_Widgets)
	message("Qt5 not found. Remove windowQt.cpp from sources")
	list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/win/windowQt.cpp")
endif()
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${sources})

# Build GNcore library
if (GN_BUILD_IS_STATIC)
	add_library(GNcore ${sources} ${garnet_headers} ${GN_BUILD_ROOT_DIR}/src/private/inc/garnet/base/build.h)
else()
	add_library(GNcore SHARED ${sources} ${garnet_headers} ${GN_BUILD_ROOT_DIR}/src/private/inc/garnet/base/build.h)
endif()

# Define GNcore properties
target_include_directories(GNcore
    PUBLIC ${GN_ROOT_DIR}/src/private/inc ${GN_BUILD_ROOT_DIR}/src/private/inc/garnet)
target_link_libraries(GNcore PUBLIC GNextern)
target_compile_features(GNcore PUBLIC cxx_std_11)
if (MSVC)
    target_compile_options(GNcore PUBLIC "/utf-8" "/EHsc")
    target_compile_options(GNcore PRIVATE "/W4" "/WX" "/Zi" "/MP")
endif()
