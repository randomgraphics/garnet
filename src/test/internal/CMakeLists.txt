# Collect all *.cpp files under src/core/, then keep only those that live
# inside a directory named "test" (e.g. src/core/rdg/test/foo.cpp).
file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
list(FILTER test_sources INCLUDE REGEX ".*/test/[^/]+\\.cpp$")
add_executable(GNtest-internal ${test_sources})
target_include_directories(GNtest-internal PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/3rdparty/rapid-vulkan/inc)
target_link_libraries(GNtest-internal PRIVATE
    GNcore-objects
    GNextern
    Catch2::Catch2WithMain)
if(IS_GNUC)
    target_compile_options(GNtest-internal PRIVATE -Wno-sign-compare)
endif()
# add_test(NAME GNtest-internal COMMAND GNtest-internal)
