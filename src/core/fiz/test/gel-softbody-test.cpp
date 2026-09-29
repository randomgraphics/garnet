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
        CHECK(cubeMesh->vertices().size() == 64); // 4^3
        CHECK(cubeMesh->faceVertexRange().offset == 0);
        CHECK(cubeMesh->faceVertexRange().count == 56); // 64 - 8 interior
        CHECK(cubeMesh->faceVertices().size() == 56);
        for (const auto & f : cubeMesh->faces()) {
            CHECK(f.indices[0] < 56);
            CHECK(f.indices[1] < 56);
            CHECK(f.indices[2] < 56);
        }
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
        size_t surfaceCount = sphereMesh->vertices().size() - 1; // 1 center vertex
        CHECK(sphereMesh->faceVertexRange().offset == 0);
        CHECK(sphereMesh->faceVertexRange().count == surfaceCount);
        CHECK(sphereMesh->faceVertices().size() == surfaceCount);
        for (const auto & f : sphereMesh->faces()) {
            CHECK(f.indices[0] < surfaceCount);
            CHECK(f.indices[1] < surfaceCount);
            CHECK(f.indices[2] < surfaceCount);
        }
        float expectedVol = (4.0f / 3.0f) * 3.14159265f * 1.0f;
        CHECK(sphereMesh->restVolume() == Catch::Approx(expectedVol).epsilon(0.01f));
    }

    SECTION("Custom GelMesh with ArrayView and vertex re-arranging") {
        // Vertex 0: interior, 1: surface, 2: interior, 3: surface, 4: surface, 5: surface
        std::vector<GelMesh::Vertex> verts = {
            {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, 1.0f}, // interior (index 0)
            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f}, // surface (index 1)
            {{0.2f, 0.2f, 0.2f}, {0.0f, 0.0f, 0.0f}, 1.0f}, // interior (index 2)
            {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f}, // surface (index 3)
            {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 1.0f}, // surface (index 4)
            {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, 1.0f}, // surface (index 5)
        };
        std::vector<GelMesh::Face> faces = {
            {{1, 4, 3}},
            {{1, 3, 5}},
            {{1, 5, 4}},
            {{3, 4, 5}},
        };
        std::vector<GelMesh::Tetrahedron> tets = {
            {{1, 3, 4, 5}, 0.0f},
            {{0, 1, 3, 4}, 0.0f},
        };
        auto customMesh = GelMesh::create(verts, faces, tets);
        REQUIRE(customMesh);
        CHECK(customMesh->vertices().size() == 6);
        CHECK(customMesh->faceVertexRange().offset == 0);
        CHECK(customMesh->faceVertexRange().count == 4); // exactly 4 surface vertices
        CHECK(customMesh->faceVertices().size() == 4);
        for (const auto & f : customMesh->faces()) {
            CHECK(f.indices[0] < 4);
            CHECK(f.indices[1] < 4);
            CHECK(f.indices[2] < 4);
        }
        CHECK(customMesh->tetrahedra().size() == 2);
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
    auto initBlob = gel->surfaceVertices(true);
    REQUIRE(initBlob);
    CHECK(initBlob->accessor<Gel::SurfaceVertex>().size() == 26);
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
    auto geomBlob = gel->surfaceVertices(true);
    REQUIRE(geomBlob);
    auto geom = geomBlob->accessor<Gel::SurfaceVertex>();
    CHECK(geom.size() == 26);
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
    auto geomBlob = gel->surfaceVertices(false);
    REQUIRE(geomBlob);
    auto geom = geomBlob->accessor<Gel::SurfaceVertex>();
    REQUIRE(geom.size() == mesh->faceVertices().size());

    Scalar maxEdgeStrain = 0.0f;
    auto   checkEdge     = [&](uint32_t i0, uint32_t i1) {
        Vector3 p0      = mesh->faceVertices()[i0].position;
        Vector3 p1      = mesh->faceVertices()[i1].position;
        Scalar  restLen = std::sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y) + (p1.z - p0.z) * (p1.z - p0.z));
        Vector3 curr0   = geom[i0].position;
        Vector3 curr1   = geom[i1].position;
        Scalar  currLen =
            std::sqrt((curr1.x - curr0.x) * (curr1.x - curr0.x) + (curr1.y - curr0.y) * (curr1.y - curr0.y) + (curr1.z - curr0.z) * (curr1.z - curr0.z));
        if (restLen > 1e-4f) {
            Scalar strain = std::abs(currLen - restLen) / restLen;
            if (strain > maxEdgeStrain) maxEdgeStrain = strain;
        }
    };
    for (const auto & f : mesh->faces()) {
        checkEdge(f.indices[0], f.indices[1]);
        checkEdge(f.indices[1], f.indices[2]);
        checkEdge(f.indices[2], f.indices[0]);
    }
    // Surface edge lengths recover within 5% of rest shape
    CHECK(maxEdgeStrain < 0.05f);

    // And volume is conserved
    Scalar vol = gel->currentVolume();
    CHECK(vol == Catch::Approx(1.0f).margin(0.005f));
}

TEST_CASE("fiz gel forward determinism produces bit-identical trajectories", "[fiz][gel]") {
    auto runSim = [](std::vector<Gel::SurfaceVertex> & outFinalGeom) {
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

        auto geomBlob = gel->surfaceVertices(true);
        REQUIRE(geomBlob);
        auto geom = geomBlob->accessor<Gel::SurfaceVertex>();
        outFinalGeom.assign(geom.begin(), geom.end());
    };

    std::vector<Gel::SurfaceVertex> run1, run2;
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

TEST_CASE("fiz gel inflatable balloon shell with pneumatic pressure", "[fiz][gel]") {
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

    // Hollow inflatable sphere shell: zero interior tetrahedra
    auto hollowMesh = GelMesh::createHollowSphere(1.0f, 12, 8);
    REQUIRE(hollowMesh);
    CHECK(hollowMesh->tetrahedra().empty());
    CHECK(!hollowMesh->faces().empty());
    CHECK(!hollowMesh->edges().empty());

    GelDesc gelDesc;
    gelDesc.mesh               = hollowMesh;
    gelDesc.transform.position = {0.0f, 3.0f, 0.0f};
    gelDesc.temper.restitution = 0.9f;
    gelDesc.temper.density     = 0.1f;
    gelDesc.edgeCompliance     = 1e-4f;
    gelDesc.pressure           = 2500.0f; // inflated balloon
    auto balloon               = solver->createGel(gelDesc);
    REQUIRE(balloon);

    CHECK(balloon->pressure() == Catch::Approx(2500.0f));

    // Dynamic pressure modification
    balloon->setPressure(3500.0f);
    CHECK(balloon->pressure() == Catch::Approx(3500.0f));

    // Simulate drop and rebound
    UnitOfTime dt(16'666'667);
    float      minY     = 100.0f;
    float      reboundY = 0.0f;

    for (int i = 0; i < 70; ++i) {
        solver->step(dt);
        float y = balloon->position().y;
        if (y < minY) {
            minY = y;
        } else if (minY < 1.0f && y > minY + 0.1f && y > reboundY) {
            reboundY = y;
        }
    }

    // Balloon squashes against the floor and rebounds
    CHECK(minY < 1.0f);
    CHECK(reboundY > minY + 0.5f);
}
