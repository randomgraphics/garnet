function(add_simple_test folder)
    # determine target name
    if (ARGV1)
        set(test_name ${ARGV1})
    else()
        set(test_name ${folder})
    endif()

    string(CONCAT target_name GNtest- ${test_name})
    set(folder_path ${CMAKE_CURRENT_SOURCE_DIR}/${folder})
    set(pch_h ${folder_path}/pch.h)
    set(pch_cpp ${folder_path}/pch.cpp)

    if (EXISTS ${pch_h} AND EXISTS ${pch_cpp})
        # use precompiled header
        add_executable(${target_name} ${folder}/main.cpp ${folder}/pch.cpp ${folder}/pch.h)
        GN_setup_pch(${folder}/main.cpp PCH_SOURCE ${folder}/pch.cpp)
    else()
        # no PCH: compile main.cpp only
        add_executable(${target_name} ${folder}/main.cpp)
    endif()
    target_link_libraries(${target_name} GNcore)
endfunction()

add_subdirectory(3rdparty)

# Deliberately after the Catch2 subdirectory: vendored test framework code must not be
# instrumented, or its lines would dominate the report.
GN_enable_code_coverage()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fmt/include)

# this is needed for fmt to build correctly with unicode support on Windows platform.
if (MSVC)
    add_compile_options("/utf-8")
endif()

if (GLUT_FOUND)
    add_simple_test(assimp)
endif()
add_simple_test(stack-dump)
if (GLUT_FOUND)
    add_simple_test(ft2)
endif()
add_simple_test(pcre)
add_subdirectory(ut)
add_subdirectory(internal)
add_simple_test(xml)
add_subdirectory(foo)
