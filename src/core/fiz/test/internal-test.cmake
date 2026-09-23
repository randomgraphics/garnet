target_sources(${GN_INTERNAL_TEST_TARGET} PRIVATE
    ${GN_INTERNAL_TEST_SOURCE_DIR}/solid-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/deterministic-forward-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/t-reversal-symmetry-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/integer-time-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/scale-and-coordinate-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/solid-collision-test.cpp)
