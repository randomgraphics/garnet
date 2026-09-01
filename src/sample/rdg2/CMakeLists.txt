# Regenerate sample SPIR-V headers only when shader sources change.
GN_compile_glsl_shaders(spv_headers
  SOURCES solid-triangle.vert
          solid-triangle.frag
          cube-face.vert
          cube-face.frag
          cube-all-faces.frag
          cube-draw.vert
          cube-draw.frag
  INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src/core/fx2/vk-shaders
  COMMENT "Compiling RDG sample GLSL to SPIR-V headers"
)
add_custom_target(rdg-sample-shaders DEPENDS ${spv_headers})

add_executable(GNsample-rdg2-open-graph-hello-world open-graph-hello-world.cpp)
target_link_libraries(GNsample-rdg2-open-graph-hello-world GNcore)

add_executable(GNsample-rdg2-closed-graph-hello-world closed-graph-hello-world.cpp)
target_link_libraries(GNsample-rdg2-closed-graph-hello-world GNcore)

add_executable(GNsample-rdg2-open-graph-simple-triangle open-graph-simple-triangle.cpp)
target_include_directories(GNsample-rdg2-open-graph-simple-triangle PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(GNsample-rdg2-open-graph-simple-triangle GNcore)
add_dependencies(GNsample-rdg2-open-graph-simple-triangle rdg-sample-shaders)

add_custom_target(rdg2-open-graph-render-to-cube-shaders DEPENDS ${_rdg_sample_shader_hdrs})

add_executable(GNsample-rdg2-open-graph-render-to-cube open-graph-render-to-cube.cpp)
target_include_directories(GNsample-rdg2-open-graph-render-to-cube PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(GNsample-rdg2-open-graph-render-to-cube GNcore)
add_dependencies(GNsample-rdg2-open-graph-render-to-cube rdg2-open-graph-render-to-cube-shaders)
