GN_setup_pch(simple-triangle.cpp pch.cpp PCH_SOURCE pch.cpp)
add_executable(GNsample-rdg-simple-triangle simple-triangle.cpp pch.cpp)
target_link_libraries(GNsample-rdg-simple-triangle GNcore)
