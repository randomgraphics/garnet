target_sources(${GN_INTERNAL_TEST_TARGET} PRIVATE
    ${GN_INTERNAL_TEST_SOURCE_DIR}/kernel-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/image-kernel-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/lit-kernel-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/imgui-backend-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/shared-shader-constants-test.cpp)
target_sources(${GN_INTERNAL_TEST_TARGET} PRIVATE ${GN_INTERNAL_TEST_SOURCE_DIR}/bindless-ssc-test.cpp
                                                   ${GN_INTERNAL_TEST_SOURCE_DIR}/bindless-sky-test.cpp)
