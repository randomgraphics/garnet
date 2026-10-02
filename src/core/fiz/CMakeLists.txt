# Note: this file is "included" by the parent core/CMakeLists.txt, so CMAKE_CURRENT_SOURCE_DIR
# still points to src/core.

GN_add_source_folder(fiz_sources fiz)
list(APPEND sources ${fiz_sources})
