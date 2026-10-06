# Note: this file is included by the parent CMake file, so CMAKE_CURRENT_SOURCE_DIR
# still points to src/core. This keeps generated-header properties in the parent target scope.

GN_add_source_folder(fx2_sources fx2 PCH_SOURCE fx2/pch.cpp)
set_property(SOURCE ${fx2_sources} APPEND_STRING PROPERTY COMPILE_FLAGS " -I${CMAKE_CURRENT_BINARY_DIR}/fx2")
list(APPEND sources ${fx2_sources})

if(VULKAN_FOUND)
    set(_fx2_shader_dir ${CMAKE_CURRENT_SOURCE_DIR}/fx2/vk-shaders)
    set(_fx2_shader_out_dir ${CMAKE_CURRENT_BINARY_DIR}/fx2)

    file(GLOB _fx2_shader_headers "${_fx2_shader_dir}/*.h")

    GN_compile_glsl_shaders(_fx2_shader_hdrs
        SOURCES ${_fx2_shader_dir}/bindless-unlit.vert
                ${_fx2_shader_dir}/bindless-unlit-texture.vert
                ${_fx2_shader_dir}/bindless-unlit.frag
                ${_fx2_shader_dir}/bindless-lambertian.vert
                ${_fx2_shader_dir}/bindless-lambertian-texture.vert
                ${_fx2_shader_dir}/bindless-lambertian.frag
                ${_fx2_shader_dir}/bindless-pbr.vert
                ${_fx2_shader_dir}/bindless-pbr.frag
                ${_fx2_shader_dir}/bindless-cel.vert
                ${_fx2_shader_dir}/bindless-cel-texture.vert
                ${_fx2_shader_dir}/bindless-cel.frag
                ${_fx2_shader_dir}/bindless-cel-outline.vert
                ${_fx2_shader_dir}/bindless-cel-outline-texture.vert
                ${_fx2_shader_dir}/bindless-cel-outline.frag
                ${_fx2_shader_dir}/bindless-sky.vert
                ${_fx2_shader_dir}/bindless-sky.frag
                ${_fx2_shader_dir}/imgui.vert
                ${_fx2_shader_dir}/imgui.frag
        OUT_DIR ${_fx2_shader_out_dir}
        INCLUDE_DIRS ${_fx2_shader_dir}
        DEPENDS ${_fx2_shader_headers}
        COMMENT "Compiling fx2 GLSL to SPIR-V"
    )

    add_custom_target(GNcore-fx2-shaders DEPENDS ${_fx2_shader_hdrs})
endif()
