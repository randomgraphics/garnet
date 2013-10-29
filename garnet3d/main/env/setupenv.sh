#!/usr/bin/bash
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
SCONS_ROOT=${GARNET_ROOT}/env/scons/2.2.0
SCONS_LIB_DIR=${SCONS_ROOT}/engine
export SCONS_ROOT
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
alias gnext="cd ${GARNET_ROOT}/src/extern/"
alias gnpriv="cd ${GARNET_ROOT}/src/priv/"
alias gninc="cd ${GARNET_ROOT}/src/priv/inc/garnet/"
alias gncore="cd ${GARNET_ROOT}/src/priv/core/"
alias gnbase="cd ${GARNET_ROOT}/src/priv/core/base/"
alias gneng="cd ${GARNET_ROOT}/src/priv/core/engine/"
alias gnutil="cd ${GARNET_ROOT}/src/priv/core/util/"
alias gngfx="cd ${GARNET_ROOT}/src/priv/gfx/"
alias gngpu="cd ${GARNET_ROOT}/src/priv/gpu/"
alias gnmedia="cd ${GARNET_ROOT}/src/priv/media/"
alias gnmisc="cd ${GARNET_ROOT}/src/priv/misc/"
alias gnsample="cd ${GARNET_ROOT}/src/priv/sample/"
alias gntest="cd ${GARNET_ROOT}/src/priv/test/"
alias gntool="cd ${GARNET_ROOT}/src/priv/tool/"
alias gndoc="cd ${GARNET_ROOT}/src/priv/doc/"
alias gnmsvc="cd ${GARNET_ROOT}/msvc"
alias gnenv="cd ${GARNET_ROOT}/env/"

# =====
# Misc.
# =====

unset INCLUDE
unset LIBS
PATH=${GARNET_ROOT}/env/bin/bash:${PATH}


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
