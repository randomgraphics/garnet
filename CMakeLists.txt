# TODO:
#   - build configurations: debug|profile|retail, x64|x86, mswin|darwin|linux, static|shared
#   - copy 3rd party binaries to build folder: add_custom_command() file(COPY ..)
#   - build 

cmake_minimum_required(VERSION 3.11)

project(garnet)

# configure build variant
#set(CMAKE_BUILD_TYPE debug)
#set(CMAKE_BUILD_DIRECTORY build.tmp/${CMAKE_SYSTEM_NAME}.${CMAKE_SYSTEM_PROCESSOR}.${COMPILERNAME}${COMPILER_VERSION}.${CMAKE_BUILD_TYPE})

# set common global variables
set(GARNET_ROOT_DIR ${PROJECT_SOURCE_DIR})

add_subdirectory(sconstest)
add_subdirectory(src/private/core)
