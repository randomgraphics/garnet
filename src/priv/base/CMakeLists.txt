MACRO( USE_PCH dir header source )
    IF( ${CMAKE_CXX_COMPILER} MATCHES "cl" )
        SET_SOURCE_FILES_PROPERTIES(
            ${ARGN}
            PROPERTIES
            COMPILE_FLAGS "/Yu${header} /Fp$(IntDir)\\${dir}_pch.pch" )
        SET_SOURCE_FILES_PROPERTIES(
            ${source}
            PROPERTIES
            COMPILE_FLAGS "/Yc${header} /Fp$(IntDir)\\${dir}_pch.pch" )
    ENDIF( ${CMAKE_CXX_COMPILER} MATCHES "cl" )
ENDMACRO( USE_PCH )


# ====
# base
# ====
SET( BASE_SRC
    base/boundingVolume.cpp
    base/clock.cpp
    base/debug.cpp
    base/file.cpp
    base/log.cpp
    base/misc.cpp
    base/path.cpp
    base/plugin.cpp
    base/profiler.cpp
    base/registry.cpp
    base/sharedLib.cpp
    base/string.cpp
    base/variant.cpp
    base/xml.cpp )
SOURCE_GROUP( base FILES ${BASE_SRC} base/pch.cpp )
USE_PCH( base pch.h base/pch.cpp ${BASE_SRC} )

# =====
# input
# =====
SET( INPUT_SRC
    input/basicInput.cpp
    input/basicInputMsw.cpp
    input/basicInputXInput.cpp
    input/input.cpp
    input/inputDInput.cpp
    input/inputMsw.cpp
    input/inputX11.cpp )
SOURCE_GROUP( input FILES ${INPUT_SRC} input/pch.cpp )
USE_PCH( input pch.h input/pch.cpp ${INPUT_SRC} )

# ===
# win
# ===
SET( WIN_SRC
    win/win.cpp
    win/windowMsw.cpp )
SOURCE_GROUP( win FILES ${WIN_SRC} win/pch.cpp )
USE_PCH( win pch.h win/pch.cpp ${WIN_SRC} )



ADD_LIBRARY( GNbase ${BASE_SRC} base/pch.cpp ${INPUT_SRC} input/pch.cpp ${WIN_SRC} win/pch.cpp )
