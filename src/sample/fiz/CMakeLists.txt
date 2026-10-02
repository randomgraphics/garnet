# GNsample-fiz-solids: Visual rigid body simulation demonstrating multi-threaded physics solving
add_executable(GNsample-fiz-solids solids.cpp)
target_link_libraries(GNsample-fiz-solids GNcore)

# GNsample-fiz-gel: Visual soft body & volumetric gel simulation
add_executable(GNsample-fiz-gel gel.cpp)
target_link_libraries(GNsample-fiz-gel GNcore)
