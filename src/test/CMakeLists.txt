function(add_simple_test folder)
    # determine target name
    if (ARGV1)
        set(test_name ${ARGV1})
    else()
        set(test_name ${folder})
    endif()

    # create target executable
    #message("create simple app = ${folder} ${test_name}")
    GN_setup_pch(${folder}/main.cpp PCH_SOURCE ${folder}/pch.cpp)
    string(CONCAT target_name GNtest- ${test_name})
    add_executable(${target_name} ${folder}/main.cpp ${folder}/pch.cpp ${folder}/pch.h)
    target_link_libraries(${target_name} GNcore)
endfunction()

if (GLUT_FOUND)
    add_simple_test(assimp)
endif()
if (D3D9_FOUND)
    add_simple_test(d3d9)
endif()
#add_subdirectory(d3d11)
if (D3D12_FOUND)
    add_simple_test(d3d12)
endif()
add_simple_test(engine)
add_simple_test(font)
if (GLUT_FOUND)
    add_simple_test(ft2)
endif()
add_simple_test(gpu)
add_simple_test(gpu2)
add_simple_test(input)
add_simple_test(pcre)
add_simple_test(renderToTexture render2texture)
add_simple_test(resdb)
add_simple_test(rt)
#add_simple_test(simd)
add_simple_test(sprite)
add_subdirectory(ut)
add_simple_test(xml)
