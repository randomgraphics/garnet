// Compile the viewer's private model code only into this test translation unit.
// This keeps the types out of GNcore while using the common internal test runner.
#include "../model-scene.cpp"
#include "../model-geometry.cpp"
#include "model-scene-test.cpp"
