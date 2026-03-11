# Regenerate PBR SPIR-V headers in build dir when shader sources or script change (builds automatically with GNcore).
if(VULKAN_FOUND)
    find_package(Python3 REQUIRED)
    set(_pbr_vert_h ${CMAKE_CURRENT_BINARY_DIR}/pbr-vert.spv.h)
    set(_pbr_frag_h ${CMAKE_CURRENT_BINARY_DIR}/pbr-frag.spv.h)
    # On Windows run scripts via python.exe; WORKING_DIRECTORY = binary dir so "." is the output dir.
    add_custom_command(
        OUTPUT "${_pbr_vert_h}" "${_pbr_frag_h}"
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/compile_pbr_shaders.py "."
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/compile_pbr_shaders.py
                ${CMAKE_CURRENT_SOURCE_DIR}/vk-shaders/pbr.vert
                ${CMAKE_CURRENT_SOURCE_DIR}/vk-shaders/pbr.frag
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Compiling PBR GLSL to SPIR-V headers"
    )
    add_custom_target(GNcore-rdg-shaders DEPENDS ${_pbr_vert_h} ${_pbr_frag_h})
endif()
