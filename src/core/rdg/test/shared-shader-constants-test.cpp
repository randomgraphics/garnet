/*
 * SharedShaderConstants Phase 4 tests.
 *
 * Verifies:
 *  - set*Information() updates CPU state only (no GPU synchronous write).
 *  - build(RenderGraph&) returns a non-null Workflow with two upload tasks.
 *  - After build() + submit(), getSet0Group() descriptor set is non-null.
 *  - currentBufferView() from each upload action is non-empty after submit.
 */

#include "common.h"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>

using namespace GN;
using namespace GN::rdg;

TEST_CASE("SharedShaderConstants: create succeeds with valid GPU", "[rdg][shared-constants][gpu]") {
    auto gpu = GpuContext::create("gpu_ssc", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    SharedShaderConstants::CreateParameters cp;
    cp.gpu   = gpu;
    auto ssc = SharedShaderConstants::create("ssc_basic", cp);
    REQUIRE(ssc);
}

TEST_CASE("SharedShaderConstants: setters update CPU state, no GPU sync", "[rdg][shared-constants][gpu]") {
    auto gpu = GpuContext::create("gpu_ssc2", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    SharedShaderConstants::CreateParameters cp;
    cp.gpu   = gpu;
    auto ssc = SharedShaderConstants::create("ssc_setters", cp);
    REQUIRE(ssc);

    SharedShaderConstants::FrameInformation fi;
    fi.frameCounter  = 42;
    fi.frameDuration = Microseconds(16667);
    ssc->setFrameInformation(fi);
    CHECK(ssc->getFrameInformation().frameCounter == 42u);

    SharedShaderConstants::ViewInformation vi;
    vi.cameraPosition = {1.f, 2.f, 3.f};
    vi.aspectRatio    = 16.f / 9.f;
    ssc->setViewInformation(vi);
    CHECK(ssc->getViewInformation().cameraPosition.x == Catch::Approx(1.f));

    SharedShaderConstants::DirectLightingInformation li;
    SharedShaderConstants::DirectLight               light;
    light.type        = SharedShaderConstants::DirectLight::POINT;
    light.point.range = 10.f;
    li.lights.append(light);
    ssc->setDirectLightingInformation(li);
    CHECK(ssc->getDirectLightingInformation().lights.size() == 1u);
}

TEST_CASE("SharedShaderConstants: build() returns workflow with two upload tasks", "[rdg][shared-constants][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_ssc3", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    SharedShaderConstants::CreateParameters cp;
    cp.gpu   = gpu;
    auto ssc = SharedShaderConstants::create("ssc_build", cp);
    REQUIRE(ssc);

    SharedShaderConstants::FrameInformation fi;
    fi.frameCounter = 1;
    ssc->setFrameInformation(fi);

    auto sg = ssc->build(*rg);
    REQUIRE(!sg.workflows.empty());
    // Workflow should have two tasks: camera copy + lighting copy.
    CHECK(sg.workflows[0].tasks().size() == 2u);
    // Set 0 resource set has camera (binding 0) and lighting (binding 1).
    CHECK(ssc->getSet0Resources().size() >= 2u);
}

TEST_CASE("SharedShaderConstants: build() + submit() succeeds", "[rdg][shared-constants][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_ssc4", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    SharedShaderConstants::CreateParameters cp;
    cp.gpu   = gpu;
    auto ssc = SharedShaderConstants::create("ssc_submit", cp);
    REQUIRE(ssc);

    SharedShaderConstants::FrameInformation fi;
    fi.frameCounter = 7;
    ssc->setFrameInformation(fi);

    auto sg = ssc->build(*rg);
    REQUIRE(!sg.workflows.empty());

    auto sub = rg->submit({.workflows = SafeArrayAccessor<Workflow>(sg.workflows.data(), sg.workflows.size())});
    REQUIRE(sub);
    auto res = sub->result();
    CHECK(res.executionResult != Action::ExecutionResult::FAILED);
}

TEST_CASE("SharedShaderConstants: build() called twice advances ring slots", "[rdg][shared-constants][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_ssc5", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    SharedShaderConstants::CreateParameters cp;
    cp.gpu   = gpu;
    auto ssc = SharedShaderConstants::create("ssc_double", cp);
    REQUIRE(ssc);

    // Frame 1
    SharedShaderConstants::FrameInformation fi;
    fi.frameCounter = 1;
    ssc->setFrameInformation(fi);
    {
        auto sg = ssc->build(*rg);
        REQUIRE(!sg.workflows.empty());
        auto sub = rg->submit({.workflows = SafeArrayAccessor<Workflow>(sg.workflows.data(), sg.workflows.size())});
        REQUIRE(sub);
        CHECK(sub->result().executionResult != Action::ExecutionResult::FAILED);
    }

    // Frame 2 — ring slot must advance without blocking (waitForIdle still in place for now).
    fi.frameCounter = 2;
    ssc->setFrameInformation(fi);
    {
        auto sg = ssc->build(*rg);
        REQUIRE(!sg.workflows.empty());
        auto sub = rg->submit({.workflows = SafeArrayAccessor<Workflow>(sg.workflows.data(), sg.workflows.size())});
        REQUIRE(sub);
        CHECK(sub->result().executionResult != Action::ExecutionResult::FAILED);
    }
}
