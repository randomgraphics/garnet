GN_setup_pch(main.cpp pch.cpp PCH_SOURCE pch.cpp)
add_executable(GNsampleBezier main.cpp pch.cpp)
target_link_libraries(GNsampleBezier GNcore)
