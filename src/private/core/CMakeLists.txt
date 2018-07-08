# TODO: generate platform configure header

# build GNcore library.
# todo: shared or static?
file(GLOB sources base/*.cpp)
add_library(GNcore ${sources})
message("root is ${GARNET_ROOT}")
target_include_directories(GNcore PRIVATE ${GARNET_ROOT_DIR}/src/contrib/inc)
target_include_directories(GNcore PUBLIC ${GARNET_ROOT_DIR}/src/private/inc)
