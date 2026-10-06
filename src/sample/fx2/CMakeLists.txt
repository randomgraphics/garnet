# Bindless PBR and unlit samples using the core fx2 module.
add_executable(GNsample-fx2-pbr simple-pbr.cpp)
target_link_libraries(GNsample-fx2-pbr GNcore)

add_executable(GNsample-fx2-unlit simple-unlit.cpp)
target_link_libraries(GNsample-fx2-unlit GNcore)
