# Note: this file is "included" by the parent cmake file. So CMAKE_CURRENT_SOURCE_DIR still points to parent folder.

GN_add_source_folder(rdg2_sources rdg2 PCH_SOURCE rdg2/pch.cpp)
set_property(SOURCE ${rdg2_sources} APPEND_STRING PROPERTY COMPILE_FLAGS " -I${CMAKE_CURRENT_BINARY_DIR}/rdg2")
list(APPEND sources ${rdg2_sources})

if(VULKAN_FOUND)
    set(_shader_dir   ${CMAKE_CURRENT_SOURCE_DIR}/rdg2/vk-shaders)
    set(_shader_out_dir ${CMAKE_CURRENT_BINARY_DIR}/rdg2)

    file(GLOB _shader_headers "${_shader_dir}/*.h")

    GN_compile_glsl_shaders(_shader_hdrs
        SOURCES ${_shader_dir}/pbr.vert ${_shader_dir}/pbr.frag
                ${_shader_dir}/skybox.vert ${_shader_dir}/skybox.frag
        OUT_DIR ${_shader_out_dir}
        INCLUDE_DIRS ${_shader_dir}
        DEPENDS ${_shader_headers}
        COMMENT "Compiling rdg2 GLSL to SPIR-V"
    )

    add_custom_target(GNcore-rdg2-shaders DEPENDS ${_shader_hdrs})
endif()
