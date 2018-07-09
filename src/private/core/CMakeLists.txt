project(GNcore)

# Collect header files
file(GLOB_RECURSE garnet_headers ../inc/*.h ../inc/*.inl ../inc/*.in)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../inc PREFIX "Headers" FILES ${garnet_headers})

# Collect source files
file(GLOB sources
	app/*.cpp app/*.h
	base/*.cpp base/*.h
	win/*.cpp win/*.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Sources" FILES ${sources})

# Build GNcore library
if (GN_BUILD_IS_STATIC)
	add_library(GNcore ${sources} ${garnet_headers})
else()
	add_library(GNcore SHARED ${sources} ${garnet_headers})
endif()
target_include_directories(GNcore PRIVATE ${GN_ROOT_DIR}/src/contrib/inc)
target_include_directories(GNcore PUBLIC ${GN_ROOT_DIR}/src/private/inc)

# generate configuration headers
configure_file(../inc/garnet/base/build.h.in ../inc/garnet/base/build.h)
target_include_directories(GNcore PUBLIC ${GN_BUILD_ROOT_DIR}/src/private/inc/garnet)
