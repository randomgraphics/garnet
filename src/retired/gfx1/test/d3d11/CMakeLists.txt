if (D3D11_FOUND)
    file(GLOB sources *.cpp *.h *.inl)
    GN_setup_pch(sources PCH_SOURCE pch.cpp)
    # TODO: generate shader.h from shader.fx
    add_executable(GNtestD3D11 ${sources})
    target_link_libraries(GNtestD3D11 GNcore)
endif()
