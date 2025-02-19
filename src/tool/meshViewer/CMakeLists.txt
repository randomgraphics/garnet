GN_setup_pch(main.cpp pch.cpp PCH_SOURCE pch.cpp)
add_executable(GNtool-mesh-viewer main.cpp pch.cpp)
target_link_libraries(GNtool-mesh-viewer GNcore)
