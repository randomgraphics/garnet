target_sources(${GN_INTERNAL_TEST_TARGET} PRIVATE
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-buffer-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-cnc-async-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-cnc-raster-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-cnc-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-raster-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-texture-staging-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-texture-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/raster-aspect-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/raster-cross-payload-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/raster-hazard-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/swapchain-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/vk-format-utils-test.cpp
    ${GN_INTERNAL_TEST_SOURCE_DIR}/vk-gpu-payload-test.cpp)

if (VULKAN_FOUND)
    set(_shader_out_dir ${GN_INTERNAL_TEST_BINARY_DIR}/shaders)

    GN_compile_glsl_shaders(_shader_hdrs
        SOURCES ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-fullscreen.vert
                ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-fullscreen.frag
                ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-mrt.frag
                ${GN_INTERNAL_TEST_SOURCE_DIR}/mixed-cnc-raster.comp
        OUT_DIR ${_shader_out_dir}
        COMMENT "Compiling gpu2 internal-test GLSL shaders to SPIR-V headers"
    )
    add_custom_target(gpu2-test-shaders DEPENDS ${_shader_hdrs})
    add_dependencies(${GN_INTERNAL_TEST_TARGET} gpu2-test-shaders)

    set_source_files_properties(
        ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-cnc-raster-test.cpp
        ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-test.cpp
        PROPERTIES INCLUDE_DIRECTORIES ${_shader_out_dir})
endif()
