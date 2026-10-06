add_subdirectory(3rdparty)
add_subdirectory(core)
add_subdirectory(taixu)

if (GN_BUILD_SAMPLES)
    add_subdirectory(sample)
endif()

if (GN_BUILD_TOOLS)
    add_subdirectory(tool)
endif()

if (GN_BUILD_TESTS)
    add_subdirectory(test)
endif()
