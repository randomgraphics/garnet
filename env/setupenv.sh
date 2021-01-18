#!/bin/bash
dir=$(dirname $(realpath ${BASH_SOURCE[0]}))
bash --rcfile $dir/garnet.rc

