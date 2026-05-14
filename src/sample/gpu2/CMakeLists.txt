add_executable(GNsample-gpu2-hello-world gpu-hello-world.cpp)
target_link_libraries(GNsample-gpu2-hello-world GNcore)

add_executable(GNsample-gpu2-simple-copy simple-copy.cpp)
target_link_libraries(GNsample-gpu2-simple-copy GNcore)

if(VULKAN_FOUND)
    find_package(Python3 REQUIRED)
    set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
    set(_shader_dir   ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
    set(_compute_spv_h ${CMAKE_CURRENT_BINARY_DIR}/simple-compute-comp.spv.h)

    add_custom_command(
        OUTPUT "${_compute_spv_h}"
        COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
                ${_shader_dir}/simple-compute.comp
                --out-dir .
        DEPENDS ${_compile_tool} ${_shader_dir}/simple-compute.comp
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Compiling simple-compute.comp to SPIR-V header"
    )
    add_custom_target(GNsample-gpu2-shaders DEPENDS "${_compute_spv_h}")

    add_executable(GNsample-gpu2-simple-compute simple-compute.cpp)
    target_include_directories(GNsample-gpu2-simple-compute PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(GNsample-gpu2-simple-compute GNcore)
    add_dependencies(GNsample-gpu2-simple-compute GNsample-gpu2-shaders)
endif()
