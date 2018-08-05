GN_setup_pch(main.cpp pch.cpp PCH_SOURCE pch.cpp)
add_executable(GNtoolMeshViewer main.cpp pch.cpp)
target_link_libraries(GNtoolMeshViewer GNcore)
