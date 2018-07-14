#setup_pch(SOURCES main.cpp PCHCPP pch.cpp)
add_executable(GNsampleDolphin main.cpp)
target_link_libraries(GNsampleDolphin GNcore)
