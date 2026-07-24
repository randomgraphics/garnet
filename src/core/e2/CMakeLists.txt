# Note: this file is "included" by the parent core/CMakeLists.txt, so CMAKE_CURRENT_SOURCE_DIR
# still points to src/core (matching the rdg2 setup just above).

GN_add_source_folder(e2_sources e2)

# The official VisualDomain implementation includes generated SPIR-V headers (box-*.spv.h).
set_property(SOURCE ${e2_sources} APPEND_STRING PROPERTY COMPILE_FLAGS " -I${CMAKE_CURRENT_BINARY_DIR}/e2")
list(APPEND sources ${e2_sources})

if(VULKAN_FOUND)
    set(_e2_shader_dir     ${CMAKE_CURRENT_SOURCE_DIR}/e2/vk-shaders)
    set(_e2_shader_out_dir ${CMAKE_CURRENT_BINARY_DIR}/e2)

    GN_compile_glsl_shaders(_e2_shader_hdrs
        SOURCES ${_e2_shader_dir}/box.vert ${_e2_shader_dir}/box.frag
        OUT_DIR ${_e2_shader_out_dir}
        COMMENT "Compiling e2 GLSL to SPIR-V"
    )

    add_custom_target(GNcore-e2-shaders DEPENDS ${_e2_shader_hdrs})
endif()
