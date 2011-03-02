# ======================================
# detect garnet root directory
# ======================================
GARNET_ROOT=$(cd $(dirname $(dirname ${BASH_SOURCE[0]})); pwd)

# ===========================
# setup build variants
# ===========================
if [ ${OSTYPE} = "cygwin" ] ; then
    export GN_BUILD_TARGET_OS=cygwin
else
    export GN_BUILD_TARGET_OS=posix
fi
export GN_BUILD_TARGET_CPU=x86
export GN_BUILD_COMPILER=gcc
export GN_BUILD_VARIANT=debug

# ===========
# setup scons
# ===========
SCONS_ROOT=${GARNET_ROOT}/env/scons/1.2.0
PATH=${SCONS_ROOT}/scripts:${PATH}
SCONS_LIB_DIR=${SCONS_ROOT}/lib
export PATH SCONS_LIB_DIR
export SCONSFLAGS=-U
echo SCons Directory: ${SCONS_ROOT}

# ===========
# setup alias
# ===========

alias ls='ls --color=auto -F'
alias dir='ls'
alias ll='ls -l'
alias v='ls -l'
alias gnroot="cd ${GARNET_ROOT}/"
alias gnbld="cd ${GARNET_ROOT}/build.tmp/${GN_BUILD_TARGET_OS}.${GN_BUILD_TARGET_CPU}.${GN_BUILD_COMPILER}.${GN_BUILD_VARIANT}/bin/"
alias gnsrc="cd ${GARNET_ROOT}/src/"
alias gnpriv="cd ${GARNET_ROOT}/src/priv/"
alias gnbase="cd ${GARNET_ROOT}/src/priv/base/"
alias gncore="cd ${GARNET_ROOT}/src/priv/core/"
alias gndoc="cd ${GARNET_ROOT}/src/priv/doc/"
alias gneng="cd ${GARNET_ROOT}/src/priv/engine/"
alias gngfx="cd ${GARNET_ROOT}/src/priv/gfx/"
alias gngpu="cd ${GARNET_ROOT}/src/priv/gfx/gpu/"
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

cd ${GARNET_ROOT}

# =========================
# Call user specific script
# =========================

# if exist env/user/${USER}.sh env/user/${USER}.sh

# ================================================
# Garnet build environment setup done successfully
# ================================================
echo USERNAME            = $USER
echo GARNET_ROOT         = $GARNET_ROOT
echo GN_BUILD_COMPILER   = $GN_BUILD_COMPILER
echo GN_BUILD_VARIANT    = $GN_BUILD_VARIANT
echo GN_BUILD_TARGET_OS  = $GN_BUILD_TARGET_OS
echo GN_BUILD_TARGET_CPU = $GN_BUILD_TARGET_CPU
