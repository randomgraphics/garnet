#setup_pch(SOURCES main.cpp PCHCPP pch.cpp)
add_executable(GNsampleDolphin main.cpp)
target_link_libraries(GNsampleDolphin GNcore)

if (NOT GN_BUILD_IS_STATIC)
	#TODO: this doesn't work when only GNcore got recompied.
	add_custom_command(
		TARGET GNsampleDolphin
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GNcore> $<TARGET_FILE_DIR:GNsampleDolphin>
		DEPENDS $<TARGET_FILE:GNcore>
	)
endif()
