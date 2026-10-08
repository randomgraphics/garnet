if (CUDA_FOUND)
    project(GN-sample-optix LANGUAGES C CXX CUDA)
    add_library(GNsample-optix-ptx OBJECT trace.cu)
    target_include_directories(GNsample-optix-ptx PRIVATE $ENV{NVIDIA_OPTIX_INSTALL_ROOT}/include)
    set_target_properties(GNsample-optix-ptx PROPERTIES
        CUDA_PTX_COMPILATION ON # compile .cu into .ptx
    )

    add_custom_command(
        OUTPUT ptx.h
        COMMAND ${CMAKE_COMMAND}
            -DINPUT_FILES=$<TARGET_OBJECTS:GNsample-optix-ptx>
            -DOUTPUT=ptx.h
            -P ${CMAKE_CURRENT_SOURCE_DIR}/str2h.cmake
        DEPENDS trace.cu str2h.cmake
        COMMENT "Converting .ptx files to a C header."
    )

    # build main executable
    set(sources ${CMAKE_CURRENT_BINARY_DIR}/ptx.h main.cpp pch.cpp)
    GN_setup_pch(${sources} PCH_SOURCE pch.cpp)
    add_executable(GNsample-optix ${sources})
    target_include_directories(GNsample-optix PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        $ENV{NVIDIA_OPTIX_INSTALL_ROOT}/include)
    target_link_libraries(GNsample-optix GNcore CUDA::cudart CUDA::cuda_driver)
    add_dependencies(GNsample-optix GNsample-optix-ptx)
endif()