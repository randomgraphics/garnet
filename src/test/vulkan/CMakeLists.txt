if (Vulkan_FOUND)
    
    add_executable(GNtest-vulkan main.cpp)
    target_link_libraries(GNtest-vulkan PRIVATE GNcore)
    target_compile_features(GNtest-vulkan PRIVATE cxx_std_20)
endif()
