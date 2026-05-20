if(VULKAN_FOUND)
    # Regenerate SPIR-V headers (PBR + Skybox) in build dir when shader sources or tool change.
    set(_shader_dir   ${CMAKE_CURRENT_SOURCE_DIR}/vk-shaders)

    file(GLOB _shader_headers "${_shader_dir}/*.h")

    GN_compile_glsl_shaders(_pbr_hdrs
        SOURCES ${_shader_dir}/pbr.vert ${_shader_dir}/pbr.frag
        INCLUDE_DIRS ${_shader_dir}
        DEPENDS ${_shader_headers}
        COMMENT "Compiling PBR GLSL to SPIR-V headers"
    )
    GN_compile_glsl_shaders(_skybox_hdrs
        SOURCES ${_shader_dir}/skybox.vert ${_shader_dir}/skybox.frag
        INCLUDE_DIRS ${_shader_dir}
        DEPENDS ${_shader_headers}
        COMMENT "Compiling Skybox GLSL to SPIR-V headers"
    )

    add_custom_target(GNcore-rdg-shaders DEPENDS ${_pbr_hdrs} ${_skybox_hdrs})
endif()
