# Note: this file is "included" by the parent cmake file. So CMAKE_CURRENT_SOURCE_DIR still points to parent folder.

GN_add_source_folder(rdg2_sources rdg2 PCH_SOURCE rdg2/pch.cpp)
list(APPEND sources ${rdg2_sources})
