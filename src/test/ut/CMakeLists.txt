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

add_executable(GNtest-unit-tests ut.cpp testCommon.cpp testCommon.h)
target_link_libraries(GNtest-unit-tests GNcore GNextern)
if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(GNtest-unit-tests PRIVATE -Wno-ignored-qualifiers -Wno-sign-compare)
endif()