GN_add_source_folder(rdg2_sources rdg2 PCH_SOURCE rdg2/pch.cpp)
set_property(SOURCE ${rdg2_sources} APPEND_STRING PROPERTY COMPILE_FLAGS " -I${CMAKE_CURRENT_BINARY_DIR}/rdg2")
list(APPEND sources ${rdg2_sources})

if(VULKAN_FOUND)
    find_package(Python3 REQUIRED)
    set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
    set(_shader_dir   ${CMAKE_CURRENT_SOURCE_DIR}/rdg2/vk-shaders)
    set(_shader_out_dir ${CMAKE_CURRENT_BINARY_DIR}/rdg2)

    set(_pbr_vert_h    ${_shader_out_dir}/pbr-vert.spv.h)
    set(_pbr_frag_h    ${_shader_out_dir}/pbr-frag.spv.h)
    set(_sky_vert_h    ${_shader_out_dir}/skybox-vert.spv.h)
    set(_sky_frag_h    ${_shader_out_dir}/skybox-frag.spv.h)

    file(GLOB _shader_headers "${_shader_dir}/*.h")

    add_custom_command(
        OUTPUT "${_pbr_vert_h}" "${_pbr_frag_h}"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_shader_out_dir}
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_shader_dir}/pbr.vert ${_shader_dir}/pbr.frag
                --out-dir ${_shader_out_dir} -I ${_shader_dir}
        DEPENDS ${_compile_tool} ${_shader_dir}/pbr.vert ${_shader_dir}/pbr.frag ${_shader_headers}
        COMMENT "Compiling rdg2 PBR GLSL to SPIR-V"
    )
    add_custom_command(
        OUTPUT "${_sky_vert_h}" "${_sky_frag_h}"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_shader_out_dir}
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_shader_dir}/skybox.vert ${_shader_dir}/skybox.frag
                --out-dir ${_shader_out_dir} -I ${_shader_dir}
        DEPENDS ${_compile_tool} ${_shader_dir}/skybox.vert ${_shader_dir}/skybox.frag ${_shader_headers}
        COMMENT "Compiling rdg2 Skybox GLSL to SPIR-V"
    )

    add_custom_target(GNcore-rdg2-shaders DEPENDS
        ${_pbr_vert_h} ${_pbr_frag_h} ${_sky_vert_h} ${_sky_frag_h})
endif()
