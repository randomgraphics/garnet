GN_setup_pch(main.cpp pch.cpp PCH_SOURCE pch.cpp)
add_executable(GNsample-bezier main.cpp pch.cpp)
target_link_libraries(GNsample-bezier GNcore)
