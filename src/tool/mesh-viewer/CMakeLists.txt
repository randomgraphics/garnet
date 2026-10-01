add_executable(GNtool-mesh-viewer main.cpp model-scene.cpp model-geometry.cpp scene-renderer.cpp navigation.cpp)
target_link_libraries(GNtool-mesh-viewer GNcore assimp)
