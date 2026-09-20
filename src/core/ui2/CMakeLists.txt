# Included by the parent core CMake file.
GN_add_source_folder(ui2_sources ui2)
set_property(SOURCE ${ui2_sources} APPEND_STRING PROPERTY COMPILE_FLAGS " -I${CMAKE_CURRENT_BINARY_DIR}/ui2")
list(APPEND sources ${ui2_sources})

if(VULKAN_FOUND)
    set(_ui2_shader_dir ${CMAKE_CURRENT_SOURCE_DIR}/ui2/vk-shaders)
    GN_compile_glsl_shaders(_ui2_shader_hdrs
        SOURCES ${_ui2_shader_dir}/imgui.vert ${_ui2_shader_dir}/imgui.frag
        OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/ui2
        COMMENT "Compiling ui2 GLSL to SPIR-V"
    )
    add_custom_target(GNcore-ui2-shaders DEPENDS ${_ui2_shader_hdrs})
endif()
