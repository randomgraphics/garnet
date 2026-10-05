# Suspended with E2 Simple world; do not make this sample a gate for FX2 work.
# add_executable(GNsample-e2-simple-world simple-world.cpp)
# target_link_libraries(GNsample-e2-simple-world GNcore)

add_executable(GNsample-e2-world-runtime world-runtime.cpp)
target_link_libraries(GNsample-e2-world-runtime GNcore)
