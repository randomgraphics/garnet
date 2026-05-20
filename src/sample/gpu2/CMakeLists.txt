add_executable(GNsample-gpu2-hello-world gpu-hello-world.cpp)
target_link_libraries(GNsample-gpu2-hello-world GNcore)

add_executable(GNsample-gpu2-simple-copy simple-copy.cpp)
target_link_libraries(GNsample-gpu2-simple-copy GNcore)

if(VULKAN_FOUND)
    GN_compile_glsl_shaders(
        spv_headers
        SOURCES shaders/simple-compute.comp
        COMMENT "Compiling gpu2 sample shaders"
    )
    add_custom_target(GNsample-gpu2-shaders DEPENDS "${spv_headers}")

    add_executable(GNsample-gpu2-simple-compute simple-compute.cpp)
    target_include_directories(GNsample-gpu2-simple-compute PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(GNsample-gpu2-simple-compute GNcore)
    add_dependencies(GNsample-gpu2-simple-compute GNsample-gpu2-shaders)
endif()
