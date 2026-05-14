# Collect all *.cpp files under src/core/, then keep only those that live
# inside a directory named "test" (e.g. src/core/rdg/test/foo.cpp).
file(GLOB_RECURSE test_sources CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
list(FILTER test_sources INCLUDE REGEX ".*/test/[^/]+\\.cpp$")
list(APPEND test_sources ${CMAKE_CURRENT_SOURCE_DIR}/catch-failed-tests-listener.cpp)
add_executable(GNtest-internal ${test_sources})
target_include_directories(GNtest-internal PRIVATE ${CMAKE_SOURCE_DIR}/src/core)

target_link_libraries(GNtest-internal PRIVATE
    GNcore-objects
    GNextern
    Catch2::Catch2WithMain)
if (IS_GNUC)
    target_compile_options(GNtest-internal PRIVATE -Wno-sign-compare)
endif()
if (VULKAN_FOUND)
    target_compile_definitions(GNtest-internal PRIVATE GN_BUILD_HAS_VULKAN=1)
    target_include_directories(GNtest-internal PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/rapid-vulkan/inc)
endif()
# add_test(NAME GNtest-internal COMMAND GNtest-internal)
