# Regenerate SPIR-V headers (PBR + Skybox) in build dir when shader sources or tool change.
if(VULKAN_FOUND)
    find_package(Python3 REQUIRED)
    set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
    set(_shader_dir   ${CMAKE_CURRENT_SOURCE_DIR}/vk-shaders)

    set(_pbr_vert_h     ${CMAKE_CURRENT_BINARY_DIR}/pbr-vert.spv.h)
    set(_pbr_frag_h     ${CMAKE_CURRENT_BINARY_DIR}/pbr-frag.spv.h)
    set(_skybox_vert_h  ${CMAKE_CURRENT_BINARY_DIR}/skybox-vert.spv.h)
    set(_skybox_frag_h  ${CMAKE_CURRENT_BINARY_DIR}/skybox-frag.spv.h)

    file(GLOB _shader_headers "${_shader_dir}/*.h")
    add_custom_command(
        OUTPUT "${_pbr_vert_h}" "${_pbr_frag_h}"
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_shader_dir}/pbr.vert ${_shader_dir}/pbr.frag
                --out-dir . -I ${_shader_dir}
        DEPENDS ${_compile_tool}
                ${_shader_dir}/pbr.vert
                ${_shader_dir}/pbr.frag
                ${_shader_headers}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Compiling PBR GLSL to SPIR-V headers"
    )

    add_custom_command(
        OUTPUT "${_skybox_vert_h}" "${_skybox_frag_h}"
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_shader_dir}/skybox.vert ${_shader_dir}/skybox.frag
                --out-dir . -I ${_shader_dir}
        DEPENDS ${_compile_tool}
                ${_shader_dir}/skybox.vert
                ${_shader_dir}/skybox.frag
                ${_shader_headers}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Compiling Skybox GLSL to SPIR-V headers"
    )

    add_custom_target(GNcore-rdg-shaders DEPENDS
        ${_pbr_vert_h} ${_pbr_frag_h}
        ${_skybox_vert_h} ${_skybox_frag_h})
endif()
