# Build Assimps
function(build_assimp)
    # build inside a function so that we can modify build variables freely
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 ") # enable optimization all the time.
    endif()
    OPTION( ASSIMP_BUILD_TESTS "" OFF)
    OPTION( ASSIMP_INSTALL "" OFF)
    OPTION( ASSIMP_WARNINGS_AS_ERRORS "" OFF)
    add_subdirectory(assimp)
endfunction()
build_assimp()

add_subdirectory(src)