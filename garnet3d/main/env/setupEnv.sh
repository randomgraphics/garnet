# ===========
# setup common variables
# ===========
GN_BUILD_TARGET_OS=posix
GN_BUILD_TARGET_CPU=x86
GN_BUILD_COMPILER=gcc

# ===========
# setup scons
# ===========
echo SCons Directory : ${GARNET_ROOT}/env/scons
PATH=${GARNET_ROOT}/env/scons/Scripts:${PATH}
SCONS_LIB_DIR=${GARNET_ROOT}/env/scons/Lib
export PATH SCONS_LIB_DIR

# ===========
# setup alias
# ===========

alias gnroot="cd ${GARNET_ROOT}/"
alias gnbld="cd ${GARNET_ROOT}/build.tmp/scons/${GN_BUILD_TARGET_OS}/${GN_BUILD_TARGET_CPU}/${GN_BUILD_COMPILER}/${GN_BUILD_VARIANT}/bin/"
alias gnsrc="cd ${GARNET_ROOT}/src/"
alias gnpriv="cd ${GARNET_ROOT}/src/priv/"
alias gnbase="cd ${GARNET_ROOT}/src/priv/base/"
alias gncore="cd ${GARNET_ROOT}/src/priv/core/"
alias gndoc="cd ${GARNET_ROOT}/src/priv/doc/"
alias gngfx="cd ${GARNET_ROOT}/src/priv/gfx/"
alias gnrndr="cd ${GARNET_ROOT}/src/priv/gfx/rndr/"
alias gninc="cd ${GARNET_ROOT}/src/priv/inc/garnet/"
alias gnmedia="cd ${GARNET_ROOT}/src/priv/media/"
alias gnmisc="cd ${GARNET_ROOT}/src/priv/misc/"
alias gnsample="cd ${GARNET_ROOT}/src/priv/sample/"
alias gntest="cd ${GARNET_ROOT}/src/priv/test/"
alias gntool="cd ${GARNET_ROOT}/src/priv/tool/"
alias gnutil="cd ${GARNET_ROOT}/src/priv/util/"
alias gnextern="cd ${GARNET_ROOT}/src/extern/"
alias gnbin="cd ${GARNET_ROOT}/bin/"
alias gnmsvc="cd ${GARNET_ROOT}/msvc"
alias gnenv="cd ${GARNET_ROOT}/env/"

# =====
# Misc.
# =====

unset INCLUDE
unset LIBS

# ============
# End of setup
# ============

echo Garnet root : ${GARNET_ROOT}
cd ${GARNET_ROOT}

# =========================
# Call user specific script
# =========================

echo User name: ${USER}
# if exist env/user/${USER}.sh env/user/${USER}.sh

