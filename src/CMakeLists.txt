add_subdirectory(3rdparty/src)
add_subdirectory(core)

if (GN_BUILD_SAMPLES)
    add_subdirectory(sample)
endif()

if (GN_BUILD_TOOLS)
    add_subdirectory(tool)
endif()

if (GN_BUILD_TESTS)
    add_subdirectory(test)
endif()
