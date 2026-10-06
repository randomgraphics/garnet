GN_setup_pch(main.cpp PCH_SOURCE pch.cpp)
add_executable(GNsample-thickline main.cpp pch.cpp)
target_link_libraries(GNsample-thickline GNcore)
