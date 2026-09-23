# disable c and c++ warnings
function(disable_warning var_name)
    if (MSVC)
        string(REPLACE "/W4" "" aaa "${${var_name}}")
        string(REPLACE "/W3" "" aaa "${aaa}")
        string(REPLACE "/W2" "" aaa "${aaa}")
        string(REPLACE "/W1" "" aaa "${aaa}")
        string(REPLACE "/WX" "" aaa "${aaa}")
        set(${var_name} "${aaa} /w" PARENT_SCOPE)
    else()
        string(REPLACE "-Wall" "" aaa "${${var_name}}")
        string(REPLACE "-Wextra" "" aaa "${aaa}")
        set(${var_name} "${aaa} -w" PARENT_SCOPE)
    endif()
endfunction()
disable_warning(CMAKE_C_FLAGS)
disable_warning(CMAKE_CXX_FLAGS)

# Build Assimps
OPTION( ASSIMP_BUILD_TESTS "" OFF)
OPTION( ASSIMP_INSTALL "" OFF)
OPTION( ASSIMP_WARNINGS_AS_ERRORS "" OFF)
add_subdirectory(assimp)

# Build FreeType as static library
set(BUILD_SHARED_LIBS OFF)
set(FT_DISABLE_GPL ON)
add_subdirectory(freetype)

# Build GLFW (window/input, cross-platform)
if (NOT ANDROID)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw)
endif()

# Dear ImGui core only. Garnet owns the gpu2/window backend, so upstream demo
# and renderer/platform backends must not become accidental dependencies.
if(GN_BUILD_IS_STATIC)
    set(_imgui_library_type STATIC)
else()
    # GNcore and its clients must share ImGui's current context across the DLL boundary.
    set(_imgui_library_type SHARED)
endif()
add_library(imgui ${_imgui_library_type}
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC imgui)
if(WIN32 AND NOT GN_BUILD_IS_STATIC)
    target_compile_definitions(imgui PRIVATE "IMGUI_API=__declspec(dllexport)" INTERFACE "IMGUI_API=__declspec(dllimport)")
endif()
set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Build Jolt Physics (rigid body simulation for GNfiz)
set(TARGET_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(TARGET_HELLO_WORLD OFF CACHE BOOL "" FORCE)
set(TARGET_SAMPLES OFF CACHE BOOL "" FORCE)
set(TARGET_VIEWER OFF CACHE BOOL "" FORCE)
set(TARGET_PERFORMANCE_TEST OFF CACHE BOOL "" FORCE)
set(ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(ENABLE_ALL_WARNINGS OFF CACHE BOOL "" FORCE)
set(OVERRIDE_CXX_FLAGS OFF CACHE BOOL "" FORCE)
set(INTERPROCEDURAL_OPTIMIZATION OFF CACHE BOOL "" FORCE)
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF CACHE BOOL "" FORCE)
set(JPH_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(DOUBLE_PRECISION OFF CACHE BOOL "" FORCE)
set(CROSS_PLATFORM_DETERMINISTIC ON CACHE BOOL "" FORCE)
set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF CACHE BOOL "" FORCE)
set(JPH_USE_DX12 OFF CACHE BOOL "" FORCE)
set(JPH_USE_VK OFF CACHE BOOL "" FORCE)
set(JPH_USE_MTL OFF CACHE BOOL "" FORCE)
set(JPH_USE_CPU_COMPUTE OFF CACHE BOOL "" FORCE)
add_subdirectory(jolt/Build)
set_target_properties(Jolt PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Build other 3rd party libs
add_subdirectory(src)

