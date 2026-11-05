#pragma once

#include <map>

namespace GN::e2::detail {

bool       resolveWorldTransform(const PrimeView &, FormId, WorldTransform &, const std::map<FormId, WorldTransform> * solved);
glm::dmat3 affineLinear(const WorldTransform &);
bool       supportedAffineCoefficients(const glm::dmat3 &);

} // namespace GN::e2::detail
