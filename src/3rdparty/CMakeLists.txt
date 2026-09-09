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
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC imgui)
set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Build other 3rd party libs
add_subdirectory(src)
