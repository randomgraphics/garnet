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
alias gnsrc="cd ${GARNET_ROOT}/src/"
alias gnpriv="cd ${GARNET_ROOT}/src/priv/"
alias gnbase="cd ${GARNET_ROOT}/src/priv/base/"
alias gncore="cd ${GARNET_ROOT}/src/priv/core/"
alias gngfx="cd ${GARNET_ROOT}/src/priv/gfx/"
alias gntest="cd ${GARNET_ROOT}/src/priv/test/"
alias gnsample="cd ${GARNET_ROOT}/src/priv/sample/"
alias gndoc="cd ${GARNET_ROOT}/src/priv/doc/"
alias gnman="cd ${GARNET_ROOT}/src/priv/manual/"
alias gnmisc="cd ${GARNET_ROOT}/src/priv/misc/"
alias gntool="cd ${GARNET_ROOT}/src/priv/tool/"
alias gninc="cd ${GARNET_ROOT}/src/priv/inc/garnet/"
alias gnextern="cd ${GARNET_ROOT}/src/extern/"
alias gnmedia="cd ${GARNET_ROOT}/src/media/"
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

