GN_setup_pch(hello-world.cpp pch.cpp PCH_SOURCE pch.cpp)
add_executable(GNsample-rdg-hellow-world hello-world.cpp pch.cpp)
target_link_libraries(GNsample-rdg-hellow-world GNcore)

# GN_setup_pch(draw-mesh.cpp pch.cpp PCH_SOURCE pch.cpp)
# add_executable(GNsample-rdg-draw-mesh draw-mesh.cpp pch.cpp)
# target_link_libraries(GNsample-rdg-draw-mesh GNcore)
