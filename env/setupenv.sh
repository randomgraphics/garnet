#!/bin/bash
# ======================================
# detect garnet root directory
# ======================================
export GARNET_ROOT=$(cd $(dirname $(realpath ${BASH_SOURCE[0]}))/..; pwd)

export GN_BUILD_DIR=build.linux.x64

# ===========
# setup alias
# ===========
while read -r line
do
    line=${line/\$\*/}  # remove $*
    line=${line//\\/\/}  # replace "\" with "/"
    line=${line/cd \/d/cd} # replace "cd /d" with "cd"
    line=${line/% \"/\"} # remove trailing space

    # replace all %..% with ${..}
    line2=${line/\%/\$\{} && line3=${line2/\%/\}}
    while [ "${line}" != "${line3}" ]; do
        line=${line3}
        line2=${line/\%/\$\{} && line3=${line2/\%/\}} #replace one pair of %...%
    done

    eval x=($line) # split line into array of words. x[0] is alias name, x[1] is alias value
    alias ${x[0]}="${x[1]}"
done < ${GARNET_ROOT}/env/alias.txt

# =====
# Misc.
# =====

unset INCLUDE
unset LIBS
PATH=${GARNET_ROOT}/env/bin/bash:${PATH}


# ============
# End of setup
# ============

#cd ${GARNET_ROOT}

# =========================
# Call user specific script
# =========================

# if exist env/user/${USER}.sh env/user/${USER}.sh

# ================================================
# Garnet build environment setup done successfully
# ================================================
echo USERNAME             = $USER
echo GARNET_ROOT          = $GARNET_ROOT
