target_sources(${GN_INTERNAL_TEST_TARGET} PRIVATE
    ${GN_INTERNAL_TEST_SOURCE_DIR}/internal-test.cpp)
# The wrapper compiles the viewer's private Assimp importer into the test runner.
target_link_libraries(${GN_INTERNAL_TEST_TARGET} PRIVATE assimp)
