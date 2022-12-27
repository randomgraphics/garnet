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

# collect assimp sources
GN_add_source_folder(sources assimp PCH_SOURCE assimp/AssimpPCH.cpp PCH_HEADER AssimpPCH.h)
file(GLOB_RECURSE assimp_contrib_sources assimp/contrib/*.c assimp/contrib/*.cpp)
list(APPEND sources ${assimp_contrib_sources})

# freetype sources. Copied from <freetype_package>/doc/INSTALL.ANY
list(APPEND sources
	freetype-2.8.1/autofit/autofit.c
	freetype-2.8.1/base/ftsystem.c
	freetype-2.8.1/base/ftinit.c
	freetype-2.8.1/base/ftdebug.c
	freetype-2.8.1/base/ftbase.c
	freetype-2.8.1/base/ftbbox.c
	freetype-2.8.1/base/ftglyph.c
	freetype-2.8.1/base/ftbdf.c
	freetype-2.8.1/base/ftbitmap.c
	freetype-2.8.1/base/ftcid.c
	freetype-2.8.1/base/ftfntfmt.c
	freetype-2.8.1/base/ftfstype.c
	freetype-2.8.1/base/ftgasp.c
	freetype-2.8.1/base/ftgxval.c
	freetype-2.8.1/base/ftlcdfil.c
	freetype-2.8.1/base/ftmm.c
	freetype-2.8.1/base/ftotval.c
	freetype-2.8.1/base/ftpatent.c
	freetype-2.8.1/base/ftpfr.c
	freetype-2.8.1/base/ftstroke.c
	freetype-2.8.1/base/ftsynth.c
	freetype-2.8.1/base/fttype1.c
	freetype-2.8.1/base/ftwinfnt.c
	freetype-2.8.1/base/ftmac.c
	freetype-2.8.1/bdf/bdf.c
	freetype-2.8.1/cache/ftcache.c
	freetype-2.8.1/cff/cff.c
	freetype-2.8.1/cid/type1cid.c
	freetype-2.8.1/pcf/pcf.c
	freetype-2.8.1/pfr/pfr.c
	freetype-2.8.1/sfnt/sfnt.c
	freetype-2.8.1/truetype/truetype.c
	freetype-2.8.1/type1/type1.c
	freetype-2.8.1/type42/type42.c
	freetype-2.8.1/winfonts/winfnt.c
	freetype-2.8.1/raster/raster.c
	freetype-2.8.1/smooth/smooth.c
	freetype-2.8.1/gzip/ftgzip.c
	freetype-2.8.1/lzw/ftlzw.c
	freetype-2.8.1/bzip2/ftbzip2.c
	freetype-2.8.1/gxvalid/gxvalid.c
	freetype-2.8.1/otvalid/otvalid.c
	freetype-2.8.1/psaux/psaux.c
	freetype-2.8.1/pshinter/pshinter.c
	freetype-2.8.1/psnames/psnames.c
)

# split sources into groups
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${sources})

# =================
# Build the library
# =================

add_library(GNextern ${sources} expat_build.h.in ${CMAKE_CURRENT_BINARY_DIR}/expat/expat_build.h)

target_compile_definitions(GNextern PRIVATE
    ASSIMP_BUILD_BOOST_WORKAROUND
    ASSIMP_BUILD_NO_OWN_ZLIB
    FT2_BUILD_LIBRARY
    GLEW_STATIC
    _SCL_SECURE_NO_WARNINGS # disable MSVC security warnings
)
target_include_directories(GNextern PUBLIC  ../inc)
target_include_directories(GNextern PRIVATE
    ../inc/assimp
    jpeg
    assimp
    assimp/BoostWorkaround
    ${CMAKE_CURRENT_BINARY_DIR}/expat # for expat_build.h
)

# compiler specific options
if (MSVC)
    target_compile_options(GNextern PRIVATE
        /w # don't care about warnings in 3rd party sources.
        /EHa
        /MP # enable parallel build.
    )
else()
    message("Current compiler is: ${CMAKE_CXX_COMPILER_ID}")
    target_compile_options(GNextern PRIVATE
        -w # disable all warnings
    )
endif()

# target_compile_features(GNextern PRIVATE cxx_std_11)

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
    else()
        message("X11 not found.")
    endif()
    if (NOT ${OPENGL_GLU_FOUND})
        message("GLEW is compiled w/o GLU support.")	
        target_compile_definitions(GNextern PUBLIC GLEW_NO_GLU)
    endif()
endif()
