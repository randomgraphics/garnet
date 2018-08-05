set(sources
    bsptree.cpp
    clipfrustum.cpp
    level.cpp
    level_collide.cpp
    level_pvs.cpp
    lvlFileStruct.cpp
    math_clip.cpp
    math_intersection.cpp
    pch.cpp
    portal.cpp
    portaldemo.cpp
    sector.cpp
)
file(GLOB headers *.h *.inl)
GN_setup_pch(sources PCHCPP pch.cpp)
add_executable(GNsamplePortal ${sources} ${headers})
target_link_libraries(GNsamplePortal GNcore)
