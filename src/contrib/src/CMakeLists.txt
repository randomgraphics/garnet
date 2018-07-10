# TODO: Generate private config header

# ====================
# Collect source files
# ====================
file(GLOB sources
    assimp/*.cpp # TODO: assimp compilation is slow like ass! Enable pch!
    #expat/*.c
    jpeg/*.c
    lua/*.c
	pcre/*.c
    png/*.c
    tinyxml/*.c
    zlib/*.c
)
if (${GN_BUILD_HAS_OGL})
    list(APPEND sources glew/glew.c)
endif()

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

add_library(GNextern ${sources})
target_compile_definitions(GNextern PRIVATE
    ASSIMP_BUILD_BOOST_WORKAROUND
    ASSIMP_BUILD_NO_OWN_ZLIB
    FT2_BUILD_LIBRARY
    GLEW_STATIC
)
target_include_directories(GNextern PUBLIC  ${GN_ROOT_DIR}/src/contrib/inc)
target_include_directories(GNextern PRIVATE ${GN_ROOT_DIR}/src/contrib/inc/assimp assimp/BoostWorkaround)
if (MSVC)
    target_compile_options(GNextern PRIVATE /w) # don't care about warnings in 3rd party sources.
endif()
