# Build Assimps
function(build_assimp)
    # build inside a function so that we can modify build variables freely
    if (MSVC)
        # enable optimziation, disable all warnings.
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /w /O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w /O2")
    else()
        # disable all warnings and enable optimization
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -O2")
    endif()
    OPTION( ASSIMP_BUILD_TESTS "" OFF)
    OPTION( ASSIMP_INSTALL "" OFF)
    OPTION( ASSIMP_WARNINGS_AS_ERRORS "" OFF)
    add_subdirectory(assimp)
endfunction()
build_assimp()

add_subdirectory(src)