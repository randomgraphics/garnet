# ==================================
# Generate expat build config header
# ==================================

INCLUDE(CheckIncludeFile)
INCLUDE(CheckFunctionExists)

check_function_exists(memmove HAVE_MEMMOVE)
check_function_exists(bcopy HAVE_BCOPY)
check_function_exists(mmap HAVE_MMAP)
check_function_exists(getpagesize HAVE_GETPAGESIZE)
check_include_file(dlfcn.h HAVE_FCNTL_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)

configure_file(expat_build.h.in expat/expat_build.h)

# ====================
# Collect source files
# ====================

file(GLOB sources
    expat/*.c
    jpeg/*.c
    lua/*.c
    pcre/*.c
    pcre/*.cc
    png/*.c
    tinyxml/*.c
    zlib/*.c
)

if (${OPENGL_FOUND})
    list(APPEND sources glew/glew.c)
endif()

# split sources into groups
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${sources})

# =================
# Build the library
# =================

add_library(GNextern STATIC ${sources} expat_build.h.in ${CMAKE_CURRENT_BINARY_DIR}/expat/expat_build.h)

target_compile_definitions(GNextern PRIVATE
    # ASSIMP_BUILD_BOOST_WORKAROUND
    # ASSIMP_BUILD_NO_OWN_ZLIB
    FT2_BUILD_LIBRARY
    GLEW_STATIC
    _SCL_SECURE_NO_WARNINGS # disable MSVC security warnings
)
target_include_directories(GNextern PUBLIC  ../inc)
target_include_directories(GNextern PRIVATE
    jpeg
    ${CMAKE_CURRENT_BINARY_DIR}/expat # for expat_build.h
)

# # compiler specific options
# if (MSVC)
#     target_compile_options(GNextern PRIVATE /EHa) # why?
# endif()

# OpenGL and X11 options
if (${OPENGL_FOUND})
    target_link_libraries(GNextern PUBLIC ${OPENGL_LIBRARIES})
    if (${X11_FOUND})
        message("X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
        message("X11_LIBRARIES   = ${X11_LIBRARIES}")
        # note: this x11 folder needs to be included after ../inc to ensure
        # local headers (like png.h and jpeg.h) are included over system
        # installed headers with same names.
        target_include_directories(GNextern PUBLIC ${X11_INCLUDE_DIR})
        target_link_libraries(GNextern PUBLIC ${X11_LIBRARIES})
		if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
			target_compile_definitions(GNextern PUBLIC GLEW_APPLE_GLX)
		endif()
    else()
        message("X11 not found.")
    endif()
    if (NOT ${OPENGL_GLU_FOUND})
        message("GLEW is compiled w/o GLU support.")	
        target_compile_definitions(GNextern PUBLIC GLEW_NO_GLU)
    endif()
endif()
