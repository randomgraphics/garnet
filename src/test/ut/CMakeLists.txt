find_package(PythonInterp)

if (NOT PYTHONINTERP_FOUND)
    message(WARNING "Python interpolator is required to generate unit tests.")
    return()
endif()

file(GLOB sources base/*.cpp gfx/*.cpp)

add_custom_command(
    OUTPUT ut.cpp
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/cxxtestgen.py --error-printer --include="garnet/GNbase.h" -o ${CMAKE_CURRENT_BINARY_DIR}/ut.cpp ${sources}
    DEPENDS ${sources})

add_executable(GNtestUnitTests ut.cpp testCommon.cpp testCommon.h)
target_link_libraries(GNtestUnitTests GNcore GNextern)
