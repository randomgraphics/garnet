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

    # Compile gpu2 rtt-test shaders; the generated headers land in the binary dir which is
    # added to the include path so rtt-test.cpp can #include them.
    set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
    set(_rtt_shader_dir ${CMAKE_SOURCE_DIR}/src/core/gpu2/test)
    set(_rtt_vert_hdr ${CMAKE_CURRENT_BINARY_DIR}/rtt-fullscreen-vert.spv.h)
    set(_rtt_frag_hdr ${CMAKE_CURRENT_BINARY_DIR}/rtt-fullscreen-frag.spv.h)
    set(_rtt_mrt_frag_hdr ${CMAKE_CURRENT_BINARY_DIR}/rtt-mrt-frag.spv.h)
    add_custom_command(
        OUTPUT ${_rtt_vert_hdr} ${_rtt_frag_hdr} ${_rtt_mrt_frag_hdr}
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_rtt_shader_dir}/rtt-fullscreen.vert
                ${_rtt_shader_dir}/rtt-fullscreen.frag
                ${_rtt_shader_dir}/rtt-mrt.frag
                --out-dir ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${_rtt_shader_dir}/rtt-fullscreen.vert
                ${_rtt_shader_dir}/rtt-fullscreen.frag
                ${_rtt_shader_dir}/rtt-mrt.frag
                ${_compile_tool}
        COMMENT "Compiling gpu2 rtt-test GLSL shaders to SPIR-V headers"
    )
    add_custom_target(gpu2-rtt-test-shaders DEPENDS ${_rtt_vert_hdr} ${_rtt_frag_hdr} ${_rtt_mrt_frag_hdr})
    add_dependencies(GNtest-internal gpu2-rtt-test-shaders)
    target_include_directories(GNtest-internal PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    # Compile mixed-cnc-raster test compute shader
    set(_mixed_comp_hdr ${CMAKE_CURRENT_BINARY_DIR}/mixed-cnc-raster-comp.spv.h)
    add_custom_command(
        OUTPUT ${_mixed_comp_hdr}
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_rtt_shader_dir}/mixed-cnc-raster.comp
                --out-dir ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${_rtt_shader_dir}/mixed-cnc-raster.comp
                ${_compile_tool}
        COMMENT "Compiling mixed-cnc-raster compute shader to SPIR-V header"
    )
    add_custom_target(gpu2-mixed-cnc-raster-shaders DEPENDS ${_mixed_comp_hdr})
    add_dependencies(GNtest-internal gpu2-mixed-cnc-raster-shaders)
    # Note: target_include_directories for ${CMAKE_CURRENT_BINARY_DIR} was already set by the
    # rtt-test block above; the generated mixed-cnc-raster-comp.spv.h lands in the same directory.
endif()
# add_test(NAME GNtest-internal COMMAND GNtest-internal)
