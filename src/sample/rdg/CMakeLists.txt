# Optional: run compile_shaders.py to regenerate SPIR-V headers after editing .vert/.frag
# Headers are committed so the build does not depend on glslc.
add_custom_target(rdg-simple-triangle-shaders
  COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/compile_shaders.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Compiling solid-triangle GLSL to SPIR-V headers"
)

add_executable(GNsample-rdg-simple-triangle simple-triangle.cpp)
target_include_directories(GNsample-rdg-simple-triangle PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GNsample-rdg-simple-triangle GNcore)

# PBR / draw-mesh sample (uses PbrShading, SharedShaderConstants, Material::load)
add_executable(GNsample-rdg-draw-mesh draw-mesh.cpp)
target_include_directories(GNsample-rdg-draw-mesh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GNsample-rdg-draw-mesh GNcore)
