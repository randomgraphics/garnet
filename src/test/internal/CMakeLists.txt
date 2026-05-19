set(GN_INTERNAL_TEST_TARGET GNtest-internal)

add_executable(${GN_INTERNAL_TEST_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/catch-failed-tests-listener.cpp)
target_include_directories(${GN_INTERNAL_TEST_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/src/core)

target_link_libraries(${GN_INTERNAL_TEST_TARGET} PRIVATE
    GNcore-objects
    GNextern
    Catch2::Catch2WithMain)
if (IS_GNUC)
    target_compile_options(${GN_INTERNAL_TEST_TARGET} PRIVATE -Wno-sign-compare)
endif()
if (VULKAN_FOUND)
    target_compile_definitions(${GN_INTERNAL_TEST_TARGET} PRIVATE GN_BUILD_HAS_VULKAN=1)
    target_include_directories(${GN_INTERNAL_TEST_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/rapid-vulkan/inc)
endif()

file(GLOB_RECURSE internal_test_units CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/core/*/test/internal-test.cmake)
list(SORT internal_test_units)

foreach(internal_test_cmake IN LISTS internal_test_units)
    # internal-test.cmake fragments are included in this directory scope, so provide
    # explicit per-fragment source and binary roots instead of relying on CMAKE_CURRENT_*.
    get_filename_component(GN_INTERNAL_TEST_SOURCE_DIR ${internal_test_cmake} DIRECTORY)
    file(RELATIVE_PATH _test_rel ${CMAKE_SOURCE_DIR}/src/core ${GN_INTERNAL_TEST_SOURCE_DIR})
    string(REPLACE "/" "-" _test_bin_name ${_test_rel})
    set(GN_INTERNAL_TEST_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${_test_bin_name})
    include(${internal_test_cmake})
endforeach()
# add_test(NAME GNtest-internal COMMAND GNtest-internal)
