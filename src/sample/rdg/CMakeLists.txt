set(_compile_tool ${CMAKE_SOURCE_DIR}/env/bin/compile_glsl_shader.py)

# Regenerate solid-triangle SPIR-V headers only when shader sources change.
set(_vert_src ${CMAKE_CURRENT_SOURCE_DIR}/solid-triangle.vert)
set(_frag_src ${CMAKE_CURRENT_SOURCE_DIR}/solid-triangle.frag)
set(_vert_hdr ${CMAKE_CURRENT_BINARY_DIR}/solid-triangle-vert.spv.h)
set(_frag_hdr ${CMAKE_CURRENT_BINARY_DIR}/solid-triangle-frag.spv.h)
add_custom_command(
  OUTPUT ${_vert_hdr} ${_frag_hdr}
  COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
          ${_vert_src} ${_frag_src}
          --out-dir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${_vert_src} ${_frag_src} ${_compile_tool}
  COMMENT "Compiling solid-triangle GLSL to SPIR-V headers"
)
add_custom_target(rdg-simple-triangle-shaders DEPENDS ${_vert_hdr} ${_frag_hdr})

add_executable(GNsample-rdg2-open-graph-hello-world open-graph-hello-world.cpp)
target_link_libraries(GNsample-rdg2-open-graph-hello-world GNcore)

add_executable(GNsample-rdg2-simple-triangle simple-triangle2.cpp)
target_include_directories(GNsample-rdg2-simple-triangle PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(GNsample-rdg2-simple-triangle GNcore)
add_dependencies(GNsample-rdg2-simple-triangle rdg-simple-triangle-shaders)

# render-to-cube2: solid-color cube-face shaders (impl 1 push-constant, impl 2 MRT).
set(_cube_face_vert_src ${CMAKE_CURRENT_SOURCE_DIR}/cube-face.vert)
set(_cube_face_frag_src ${CMAKE_CURRENT_SOURCE_DIR}/cube-face.frag)
set(_cube_all_frag_src  ${CMAKE_CURRENT_SOURCE_DIR}/cube-all-faces.frag)
set(_cube_face_vert_hdr ${CMAKE_CURRENT_BINARY_DIR}/cube-face-vert.spv.h)
set(_cube_face_frag_hdr ${CMAKE_CURRENT_BINARY_DIR}/cube-face-frag.spv.h)
set(_cube_all_frag_hdr  ${CMAKE_CURRENT_BINARY_DIR}/cube-all-faces-frag.spv.h)
add_custom_command(
  OUTPUT ${_cube_face_vert_hdr} ${_cube_face_frag_hdr}
  COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
          ${_cube_face_vert_src} ${_cube_face_frag_src}
          --out-dir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${_cube_face_vert_src} ${_cube_face_frag_src} ${_compile_tool}
  COMMENT "Compiling cube-face GLSL to SPIR-V headers"
)
add_custom_command(
  OUTPUT ${_cube_all_frag_hdr}
  COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
          ${_cube_all_frag_src}
          --out-dir ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${_cube_all_frag_src} ${_compile_tool}
  COMMENT "Compiling cube-all-faces GLSL to SPIR-V headers"
)
# cube-draw shaders: vertex (MVP + face index) and fragment (hardcoded face colors).
set(_cube_draw_vert_src ${CMAKE_CURRENT_SOURCE_DIR}/cube-draw.vert)
set(_cube_draw_frag_src ${CMAKE_CURRENT_SOURCE_DIR}/cube-draw.frag)
set(_cube_draw_vert_hdr ${CMAKE_CURRENT_BINARY_DIR}/cube-draw-vert.spv.h)
set(_cube_draw_frag_hdr ${CMAKE_CURRENT_BINARY_DIR}/cube-draw-frag.spv.h)
add_custom_command(
  OUTPUT ${_cube_draw_vert_hdr} ${_cube_draw_frag_hdr}
  COMMAND ${Python3_EXECUTABLE} ${_compile_tool}
          ${_cube_draw_vert_src} ${_cube_draw_frag_src}
          --out-dir ${CMAKE_CURRENT_BINARY_DIR}
          -I ${CMAKE_SOURCE_DIR}/src/core/rdg2/vk-shaders
  DEPENDS ${_cube_draw_vert_src} ${_cube_draw_frag_src} ${_compile_tool}
  COMMENT "Compiling cube-draw GLSL to SPIR-V headers"
)

add_custom_target(rdg2-render-to-cube-shaders
  DEPENDS ${_cube_face_vert_hdr} ${_cube_face_frag_hdr} ${_cube_all_frag_hdr}
          ${_cube_draw_vert_hdr} ${_cube_draw_frag_hdr})

add_executable(GNsample-rdg2-render-to-cube render-to-cube2.cpp)
target_include_directories(GNsample-rdg2-render-to-cube PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(GNsample-rdg2-render-to-cube GNcore)
add_dependencies(GNsample-rdg2-render-to-cube rdg2-render-to-cube-shaders)

# simple-pbr2: rdg2 PBR + skybox demo using SharedShaderConstants, PbrShading, SkyBox.
# No per-sample shader compile step — PBR/skybox shaders are part of GNcore-rdg2-shaders.
add_executable(GNsample-rdg2-pbr simple-pbr2.cpp)
target_link_libraries(GNsample-rdg2-pbr GNcore)
