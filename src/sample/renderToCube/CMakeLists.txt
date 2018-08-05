GN_setup_pch(main.cpp pch.cpp PCH_SOURCE pch.cpp)
add_executable(GNsampleRenderToCube main.cpp pch.cpp)
target_link_libraries(GNsampleRenderToCube GNcore)
