GN_setup_pch(main.cpp PCH_SOURCE pch.cpp)
add_executable(GNsampleThickLine main.cpp pch.cpp)
target_link_libraries(GNsampleThickLine GNcore)
