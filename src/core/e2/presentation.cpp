#include <garnet/GNengine2.h>

namespace GN::e2 {

namespace {
Logger * sPresentationLogger = getLogger("GN.e2.Presentation");
} // namespace

basis::Visual::Tableau extractTableau(const PrimeView & prime, const basis::Visual::Camera & camera, PhysicalScale scale) {
    basis::Visual::Tableau tableau;
    tableau.camera = camera;
    tableau.scale = scale;
    for (auto id : prime.query<TransformFacet, VisualFacet>()) {
        WorldTransform transform;
        if (!resolveWorldTransform(prime, id, transform)) {
            GN_ERROR(sPresentationLogger, "extractTableau: Form {} has an invalid Spatial hierarchy; instance omitted from tick {}.", id, prime.tick());
            continue;
        }
        LocalVector3 cameraRelative;
        if (!spatial::tryToLocal(camera.position, transform.position, cameraRelative)) {
            GN_ERROR(sPresentationLogger,
                     "extractTableau: Form {} cannot be represented in the camera-local frame; world position raw halves are x=({}, {}), y=({}, {}), z=({}, {}).",
                     id, transform.position.x.hi(), transform.position.x.lo(), transform.position.y.hi(), transform.position.y.lo(), transform.position.z.hi(),
                     transform.position.z.lo());
            continue;
        }
        const auto & visual = *prime.get<VisualFacet>(id);
        basis::Visual::Object object;
        object.meshId = visual.meshId;
        object.transform.position = transform.position;
        object.transform.orientation = transform.orientation;
        object.transform.scale = visual.scale * transform.uniformScale;
        object.transform.affineFallback = transform.affineFallback;
        if (transform.affineFallback) {
            object.transform.affineLinear = glm::mat3(transform.affineLinear);
            object.transform.scale = visual.scale;
        }
        tableau.objects.append(object);
    }
    return tableau;
}

} // namespace GN::e2
