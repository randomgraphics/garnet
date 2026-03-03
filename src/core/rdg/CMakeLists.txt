# Optional: regenerate PBR SPIR-V headers after editing core/rdg/shaders/*.vert|*.frag
if(VULKAN_FOUND)
    add_custom_target(rdg-pbr-shaders
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/compile_pbr_shaders.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Compiling PBR GLSL to SPIR-V headers"
    )
endif()
