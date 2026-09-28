#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <garnet/GNfiz.h>

#include <cmath>
#include <vector>

using namespace GN::fiz;

TEST_CASE("fiz gel mesh procedural factories generate correct geometry and rest volumes", "[fiz][gel]") {
    SECTION("Cube GelMesh") {
        auto cubeMesh = GelMesh::createCube(4, 2.0f);
        REQUIRE(cubeMesh);
        CHECK(cubeMesh->vertices().size() == 64);              // 4^3
        CHECK(cubeMesh->tetrahedra().size() == 3 * 3 * 3 * 6); // 27 cells * 6 tets = 162
        CHECK(cubeMesh->faces().size() == 6 * (3 * 3 * 2));    // 6 sides * 18 triangles = 108
        CHECK(!cubeMesh->edges().empty());
        CHECK(cubeMesh->restVolume() == Catch::Approx(8.0f)); // 2.0^3
    }

    SECTION("Sphere GelMesh") {
        auto sphereMesh = GelMesh::createSphere(1.0f, 12, 8);
        REQUIRE(sphereMesh);
        CHECK(!sphereMesh->vertices().empty());
        CHECK(!sphereMesh->faces().empty());
        CHECK(!sphereMesh->tetrahedra().empty());
        float expectedVol = (4.0f / 3.0f) * 3.14159265f * 1.0f;
        CHECK(sphereMesh->restVolume() == Catch::Approx(expectedVol).epsilon(0.01f));
    }
}

TEST_CASE("fiz gel solver creates, advances, and cleans up soft bodies", "[fiz][gel]") {
    GelSolverDesc desc;
    desc.gravity          = {0.0f, -9.81f, 0.0f};
    desc.numWorkerThreads = 1;
    auto solver           = GelSolver::create(desc);
    REQUIRE(solver);
    CHECK(solver->gelCount() == 0);
    CHECK(solver->solidCount() == 0);

    // 1. Static floor
    SolidDesc floorDesc;
    floorDesc.hull               = Hull::createBox({20.0f, 1.0f, 20.0f});
    floorDesc.motionType         = MotionType::STATIC;
    floorDesc.layer              = CollisionLayer::NON_MOVING;
    floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
    auto floor                   = solver->createSolid(floorDesc);
    REQUIRE(floor);
    CHECK(solver->solidCount() == 1);

    // 2. Dynamic gel cube at y = 3.0
    auto    mesh = GelMesh::createCube(3, 1.0f);
    GelDesc gelDesc;
    gelDesc.mesh               = mesh;
    gelDesc.transform.position = {0.0f, 3.0f, 0.0f};
    gelDesc.temper.restitution = 0.5f;
    gelDesc.temper.friction    = 0.5f;
    gelDesc.edgeCompliance     = 1e-4f;
    gelDesc.volumeCompliance   = 0.0f; // incompressible
    auto gel                   = solver->createGel(gelDesc);
    REQUIRE(gel);
    CHECK(solver->gelCount() == 1);
    auto initBlob = gel->deformedVertices(true);
    REQUIRE(initBlob);
    CHECK(initBlob->accessor<Gel::DeformedVertex>().size() == 27);
    CHECK(gel->surfaceIndices().size() == 48 * 3);
    CHECK(gel->surfaceIndices().size() / 3 == 48);
    CHECK(!gel->surfaceIndices().empty());
    CHECK(gel->surfaceIndices().data() != nullptr);
    CHECK(gel->position().y == Catch::Approx(3.0f).margin(0.05f));

    // Verify initial volume matches rest volume
    Scalar initialVol = gel->currentVolume();
    CHECK(initialVol == Catch::Approx(1.0f).margin(0.05f));

    // 3. Step simulation for 30 steps (~0.5s)
    UnitOfTime dt(16'666'667);
    for (int i = 0; i < 30; ++i) { solver->step(dt); }

    // Gel should have fallen
    CHECK(gel->position().y < 3.0f);

    // 4. Test vertex deformation query via Blob
    auto geomBlob = gel->deformedVertices(true);
    REQUIRE(geomBlob);
    auto geom = geomBlob->accessor<Gel::DeformedVertex>();
    CHECK(geom.size() == 27);
    for (const auto & v : geom) {
        // All vertices should be above ground (floor top surface is y = 0)
        CHECK(v.position.y >= -0.2f);
        float lenSq = v.normal.x * v.normal.x + v.normal.y * v.normal.y + v.normal.z * v.normal.z;
        CHECK(lenSq == Catch::Approx(1.0f).margin(0.01f));
    }

    // 5. Cleanup
    solver->removeGel(gel.get());
    CHECK(solver->gelCount() == 0);
    solver->removeSolid(floor.get());
    CHECK(solver->solidCount() == 0);
}

TEST_CASE("fiz gel hydrostatic tetrahedral volume preservation under impact", "[fiz][gel]") {
    GelSolverDesc desc;
    desc.gravity          = {0.0f, -9.81f, 0.0f};
    desc.numWorkerThreads = 1;
    auto solver           = GelSolver::create(desc);
    REQUIRE(solver);

    // Static floor at y = -1.0 (top surface at y = 0.0)
    SolidDesc floorDesc;
    floorDesc.hull               = Hull::createBox({20.0f, 1.0f, 20.0f});
    floorDesc.motionType         = MotionType::STATIC;
    floorDesc.layer              = CollisionLayer::NON_MOVING;
    floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
    solver->createSolid(floorDesc);

    // Soft cube with 0 volume compliance (hydrostatic incompressibility)
    auto    mesh = GelMesh::createCube(4, 1.0f);
    GelDesc gelDesc;
    gelDesc.mesh               = mesh;
    gelDesc.transform.position = {0.0f, 2.0f, 0.0f};
    gelDesc.temper.restitution = 0.3f;
    gelDesc.temper.friction    = 0.4f;
    gelDesc.edgeCompliance     = 1e-4f;
    gelDesc.volumeCompliance   = 0.0f; // strictly incompressible
    gelDesc.solverIterations   = 15;
    auto gel                   = solver->createGel(gelDesc);
    REQUIRE(gel);

    Scalar restVol = gel->restVolume();
    CHECK(restVol == Catch::Approx(1.0f));

    Scalar     maxVolError = 0.0f;
    UnitOfTime dt(16'666'667);

    // Simulate through fall, impact, and squash (60 steps)
    for (int i = 0; i < 60; ++i) {
        solver->step(dt);
        Scalar currVol = gel->currentVolume();
        Scalar err     = std::abs(currVol - restVol) / restVol;
        if (err > maxVolError) maxVolError = err;
    }

    // Asserts hydrostatic tetrahedral volume preservation: total volume under extreme impact load
    // remains constant within +-0.5% (0.005 relative error).
    CHECK(maxVolError <= 0.005f);
}

TEST_CASE("fiz gel elastic recovery to rest shape after compressive impact", "[fiz][gel]") {
    GelSolverDesc desc;
    desc.gravity          = {0.0f, -9.81f, 0.0f};
    desc.numWorkerThreads = 1;
    auto solver           = GelSolver::create(desc);
    REQUIRE(solver);

    SolidDesc floorDesc;
    floorDesc.hull               = Hull::createBox({20.0f, 1.0f, 20.0f});
    floorDesc.motionType         = MotionType::STATIC;
    floorDesc.layer              = CollisionLayer::NON_MOVING;
    floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
    solver->createSolid(floorDesc);

    auto    mesh = GelMesh::createCube(3, 1.0f);
    GelDesc gelDesc;
    gelDesc.mesh               = mesh;
    gelDesc.transform.position = {0.0f, 2.0f, 0.0f};
    gelDesc.temper.restitution = 0.85f; // highly bouncy rubber
    gelDesc.edgeCompliance     = 1e-4f;
    gelDesc.volumeCompliance   = 0.0f;
    auto gel                   = solver->createGel(gelDesc);
    REQUIRE(gel);

    UnitOfTime dt(16'666'667);

    float minY     = 100.0f;
    float reboundY = 0.0f;

    // Simulate for 80 steps: falls, impacts floor (~step 35-45), squashes, then rebounds (~step 60-80)
    for (int i = 0; i < 80; ++i) {
        solver->step(dt);
        float y = gel->position().y;
        if (y < minY) {
            minY = y;
        } else if (minY < 0.8f && y > minY + 0.1f && y > reboundY) {
            reboundY = y;
        }
    }

    // Impact compression: squashed below half-extent rest height
    CHECK(minY < 0.8f);

    // Rebound back up into air
    CHECK(reboundY > minY);

    // Asserts complete elastic recovery to rest shape after compressive release
    auto geomBlob = gel->deformedVertices(false);
    REQUIRE(geomBlob);
    auto geom = geomBlob->accessor<Gel::DeformedVertex>();
    REQUIRE(geom.size() == mesh->vertices().size());

    Scalar maxEdgeStrain = 0.0f;
    for (const auto & e : mesh->edges()) {
        uint32_t i0      = e.indices[0];
        uint32_t i1      = e.indices[1];
        Vector3  p0      = mesh->vertices()[i0].position;
        Vector3  p1      = mesh->vertices()[i1].position;
        Scalar   restLen = std::sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y) + (p1.z - p0.z) * (p1.z - p0.z));
        Vector3  curr0   = geom[i0].position;
        Vector3  curr1   = geom[i1].position;
        Scalar   currLen =
            std::sqrt((curr1.x - curr0.x) * (curr1.x - curr0.x) + (curr1.y - curr0.y) * (curr1.y - curr0.y) + (curr1.z - curr0.z) * (curr1.z - curr0.z));
        if (restLen > 1e-4f) {
            Scalar strain = std::abs(currLen - restLen) / restLen;
            if (strain > maxEdgeStrain) maxEdgeStrain = strain;
        }
    }
    // Edge lengths recover within 5% of rest shape
    CHECK(maxEdgeStrain < 0.05f);

    // And volume is conserved
    Scalar vol = gel->currentVolume();
    CHECK(vol == Catch::Approx(1.0f).margin(0.005f));
}

TEST_CASE("fiz gel forward determinism produces bit-identical trajectories", "[fiz][gel]") {
    auto runSim = [](std::vector<Gel::DeformedVertex> & outFinalGeom) {
        GelSolverDesc desc;
        desc.gravity          = {0.0f, -9.81f, 0.0f};
        desc.numWorkerThreads = 1;
        auto solver           = GelSolver::create(desc);

        SolidDesc floorDesc;
        floorDesc.hull               = Hull::createBox({20.0f, 1.0f, 20.0f});
        floorDesc.motionType         = MotionType::STATIC;
        floorDesc.layer              = CollisionLayer::NON_MOVING;
        floorDesc.transform.position = {0.0f, -1.0f, 0.0f};
        solver->createSolid(floorDesc);

        auto    mesh = GelMesh::createCube(3, 1.0f);
        GelDesc gelDesc;
        gelDesc.mesh               = mesh;
        gelDesc.transform.position = {0.0f, 2.5f, 0.0f};
        gelDesc.linearVelocity     = {1.0f, 0.0f, 0.5f};
        gelDesc.temper.restitution = 0.5f;
        gelDesc.edgeCompliance     = 1e-4f;
        gelDesc.volumeCompliance   = 0.0f;
        auto gel                   = solver->createGel(gelDesc);

        UnitOfTime dt(16'666'667);
        for (int i = 0; i < 40; ++i) { solver->step(dt); }

        auto geomBlob = gel->deformedVertices(true);
        REQUIRE(geomBlob);
        auto geom = geomBlob->accessor<Gel::DeformedVertex>();
        outFinalGeom.assign(geom.begin(), geom.end());
    };

    std::vector<Gel::DeformedVertex> run1, run2;
    runSim(run1);
    runSim(run2);

    REQUIRE(run1.size() == run2.size());
    for (size_t i = 0; i < run1.size(); ++i) {
        CHECK(run1[i].position.x == run2[i].position.x);
        CHECK(run1[i].position.y == run2[i].position.y);
        CHECK(run1[i].position.z == run2[i].position.z);
        CHECK(run1[i].normal.x == run2[i].normal.x);
        CHECK(run1[i].normal.y == run2[i].normal.y);
        CHECK(run1[i].normal.z == run2[i].normal.z);
    }
}
