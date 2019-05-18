find_package(Python3)

if (NOT Python3_FOUND)
    message(WARNING "Python3 is required to generate unit tests.")
    return()
endif()

file(GLOB sources base/*.cpp gfx/*.cpp)

add_custom_command(
    OUTPUT ut.cpp
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/cxxtestgen.py --error-printer --include="garnet/GNbase.h" -o ${CMAKE_CURRENT_BINARY_DIR}/ut.cpp ${sources}
    DEPENDS ${sources})

add_executable(GNtestUnitTests ut.cpp testCommon.cpp testCommon.h)
target_link_libraries(GNtestUnitTests GNcore GNextern)
