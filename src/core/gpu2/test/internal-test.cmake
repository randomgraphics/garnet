target_sources(${GN_INTERNAL_TEST_TARGET} PRIVATE
    ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-buffer-test.cpp
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
    find_package(Python3 REQUIRED)

    set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
    set(_shader_out_dir ${GN_INTERNAL_TEST_BINARY_DIR}/shaders)

    set(_rtt_vert_hdr ${_shader_out_dir}/rtt-fullscreen-vert.spv.h)
    set(_rtt_frag_hdr ${_shader_out_dir}/rtt-fullscreen-frag.spv.h)
    set(_rtt_mrt_frag_hdr ${_shader_out_dir}/rtt-mrt-frag.spv.h)
    add_custom_command(
        OUTPUT ${_rtt_vert_hdr} ${_rtt_frag_hdr} ${_rtt_mrt_frag_hdr}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_shader_out_dir}
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-fullscreen.vert
                ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-fullscreen.frag
                ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-mrt.frag
                --out-dir ${_shader_out_dir}
        DEPENDS ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-fullscreen.vert
                ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-fullscreen.frag
                ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-mrt.frag
                ${_compile_tool}
        COMMENT "Compiling gpu2 rtt-test GLSL shaders to SPIR-V headers"
    )
    add_custom_target(gpu2-rtt-test-shaders DEPENDS ${_rtt_vert_hdr} ${_rtt_frag_hdr} ${_rtt_mrt_frag_hdr})
    add_dependencies(${GN_INTERNAL_TEST_TARGET} gpu2-rtt-test-shaders)

    set(_mixed_comp_hdr ${_shader_out_dir}/mixed-cnc-raster-comp.spv.h)
    add_custom_command(
        OUTPUT ${_mixed_comp_hdr}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_shader_out_dir}
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${GN_INTERNAL_TEST_SOURCE_DIR}/mixed-cnc-raster.comp
                --out-dir ${_shader_out_dir}
        DEPENDS ${GN_INTERNAL_TEST_SOURCE_DIR}/mixed-cnc-raster.comp
                ${_compile_tool}
        COMMENT "Compiling mixed-cnc-raster compute shader to SPIR-V header"
    )
    add_custom_target(gpu2-mixed-cnc-raster-shaders DEPENDS ${_mixed_comp_hdr})
    add_dependencies(${GN_INTERNAL_TEST_TARGET} gpu2-mixed-cnc-raster-shaders)

    set_source_files_properties(
        ${GN_INTERNAL_TEST_SOURCE_DIR}/gpu-cnc-raster-test.cpp
        ${GN_INTERNAL_TEST_SOURCE_DIR}/rtt-test.cpp
        PROPERTIES INCLUDE_DIRECTORIES ${_shader_out_dir})
endif()
