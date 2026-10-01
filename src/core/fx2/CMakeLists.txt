# Note: this file is included by the parent CMake file, so CMAKE_CURRENT_SOURCE_DIR
# still points to src/core. This keeps generated-header properties in the parent target scope.

GN_add_source_folder(fx2_sources fx2 PCH_SOURCE fx2/pch.cpp)
set_property(SOURCE ${fx2_sources} APPEND_STRING PROPERTY COMPILE_FLAGS " -I${CMAKE_CURRENT_BINARY_DIR}/fx2")
list(APPEND sources ${fx2_sources})

if(VULKAN_FOUND)
    set(_fx2_shader_dir ${CMAKE_CURRENT_SOURCE_DIR}/fx2/vk-shaders)
    set(_fx2_shader_out_dir ${CMAKE_CURRENT_BINARY_DIR}/fx2)

    file(GLOB _fx2_shader_headers "${_fx2_shader_dir}/*.h")

    set(_fx2_lit_shaders)
    foreach(_effect model cel-model cel-outline)
        foreach(_variant RANGE 0 7)
            list(APPEND _fx2_lit_shaders ${_fx2_shader_dir}/${_effect}-kernel-${_variant}.vert)
        endforeach()
    endforeach()

    foreach(_variant RANGE 0 6)
        list(APPEND _fx2_lit_shaders ${_fx2_shader_dir}/cel-model-kernel-${_variant}.frag)
    endforeach()

    GN_compile_glsl_shaders(_fx2_shader_hdrs
        SOURCES ${_fx2_lit_shaders}
                ${_fx2_shader_dir}/image-kernel.vert
                ${_fx2_shader_dir}/blur-kernel.frag ${_fx2_shader_dir}/blur-kernel.comp
                ${_fx2_shader_dir}/mipmap-kernel.frag ${_fx2_shader_dir}/mipmap-kernel.comp
                ${_fx2_shader_dir}/unlit-kernel-plain.vert ${_fx2_shader_dir}/unlit-kernel-color.vert
                ${_fx2_shader_dir}/unlit-kernel-texture.vert ${_fx2_shader_dir}/unlit-kernel-texture-color.vert
                ${_fx2_shader_dir}/unlit-kernel-plain.frag ${_fx2_shader_dir}/unlit-kernel-texture.frag
                ${_fx2_shader_dir}/imgui.vert ${_fx2_shader_dir}/imgui.frag
                ${_fx2_shader_dir}/model.frag
                ${_fx2_shader_dir}/lambertian.frag
                ${_fx2_shader_dir}/cel-model.frag
                ${_fx2_shader_dir}/cel-outline.frag
                ${_fx2_shader_dir}/skybox.vert ${_fx2_shader_dir}/skybox.frag
        OUT_DIR ${_fx2_shader_out_dir}
        INCLUDE_DIRS ${_fx2_shader_dir}
        DEPENDS ${_fx2_shader_headers}
        COMMENT "Compiling fx2 GLSL to SPIR-V"
    )

    add_custom_target(GNcore-fx2-shaders DEPENDS ${_fx2_shader_hdrs})
endif()
