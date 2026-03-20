# Optional: regenerate solid-triangle SPIR-V headers via the general compile tool.
set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)
add_custom_target(rdg-simple-triangle-shaders
  COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
          ${CMAKE_CURRENT_SOURCE_DIR}/solid-triangle.vert
          ${CMAKE_CURRENT_SOURCE_DIR}/solid-triangle.frag
          --out-dir ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${_compile_tool}
  COMMENT "Compiling solid-triangle GLSL to SPIR-V headers"
)

add_executable(GNsample-rdg-simple-triangle simple-triangle.cpp)
target_include_directories(GNsample-rdg-simple-triangle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GNsample-rdg-simple-triangle GNcore)

# PBR / draw-mesh sample (uses PbrShading, SharedShaderConstants, Material::load)
add_executable(GNsample-rdg-draw-mesh draw-mesh.cpp)
target_include_directories(GNsample-rdg-draw-mesh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GNsample-rdg-draw-mesh GNcore)
