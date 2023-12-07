# Build Assimps
function(build_assimp)
    # build inside a function so that we can modify build variables freely
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 ") # enable optimization all the time.
        # disable all warnings
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /w")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
    endif()
    if (IS_GNUC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w") # disable all warnings
    endif()
    add_subdirectory(assimp)
endfunction()
build_assimp()

add_subdirectory(src)