#!/bin/bash
dir="$(cd $(dirname "${BASH_SOURCE[0]}");pwd)"
cd ${dir}/../../..
./setupenv.sh $@
