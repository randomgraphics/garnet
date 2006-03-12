# ===========
# setup scons
# ===========
echo SCons Directory : ${GARNET_ROOT}/env/scons
set PATH=${GARNET_ROOT}/env/scons/Scripts:${PATH}
set SCONS_LIB_DIR=${GARNET_ROOT}/env/scons/Lib
export PATH SCONS_LIB_DIR

# ===========
# setup alias
# ===========

alias j_root="cd ${GARNET_ROOT}/"
alias j_src="cd ${GARNET_ROOT}/src/"
alias j_priv="cd ${GARNET_ROOT}/src/priv/"
alias j_base="cd ${GARNET_ROOT}/src/priv/base/"
alias j_core="cd ${GARNET_ROOT}/src/priv/core/"
alias j_gfx="cd ${GARNET_ROOT}/src/priv/gfx/"
alias j_test="cd ${GARNET_ROOT}/src/priv/test/"
alias j_sample="cd ${GARNET_ROOT}/src/priv/sample/"
alias j_doc="cd ${GARNET_ROOT}/src/priv/doc/"
alias j_man="cd ${GARNET_ROOT}/src/priv/manual/"
alias j_misc="cd ${GARNET_ROOT}/src/priv/misc/"
alias j_tool="cd ${GARNET_ROOT}/src/priv/tool/"
alias j_inc="cd ${GARNET_ROOT}/src/priv/inc/garnet/"
alias j_extern="cd ${GARNET_ROOT}/src/extern/"
alias j_media="cd ${GARNET_ROOT}/src/media/"
alias j_bin="cd ${GARNET_ROOT}/bin/"
alias j_msvc="cd ${GARNET_ROOT}/msvc"
alias j_env="cd ${GARNET_ROOT}/env/"
alias -p

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

