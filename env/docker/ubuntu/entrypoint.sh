#!/bin/bash
dir="$(cd $(dirname "${BASH_SOURCE[0]}");pwd)"
cd ${dir}/../../..
export ANDROID_SDK_ROOT=/android/sdk
export ANDROID_NDK_ROOT=/android/sdk/ndk/25.2.9519653
export JAVA_HOME=/android/studio/jbr
./setupenv.sh $@
