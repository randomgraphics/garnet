#include <catch2/catch_session.hpp>

#include "renderdoc-capture.h"

int main(int argc, char * argv[]) {
    // RenderDoc documents that the API must be acquired before initializing
    // any graphics API. The actual capture starts inside the selected test,
    // after that test has created its device.
    GN::test::initializeRenderDoc();
    return Catch::Session().run(argc, argv);
}
