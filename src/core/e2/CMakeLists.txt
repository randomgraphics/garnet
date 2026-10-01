# Note: this file is "included" by the parent core/CMakeLists.txt, so CMAKE_CURRENT_SOURCE_DIR
# still points to src/core (matching the rdg2 setup just above).

GN_add_source_folder(e2_sources e2)
# Simple world is suspended while its design is reconsidered. Keep it out of
# source discovery so it cannot constrain the FX2 kernel refactor.
list(FILTER e2_sources EXCLUDE REGEX "(^|/)simple-world\\.cpp$")

list(APPEND sources ${e2_sources})
