if (CUDA_FOUND)
    project(GN-sample-cuda LANGUAGES C CXX CUDA)
    add_executable(GNsample-cuda main.cu)
endif()