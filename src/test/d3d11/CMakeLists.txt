file(GLOB sources *.cpp *.h *.inl)
GN_setup_pch(sources PCH_SOURCE pch.cpp)

# generate shader.h from shader.fx

add_executable(GNtestD3D11 ${sources})
target_link_libraries(GNtestD3D11 GNcore)