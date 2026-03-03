#include "pch.h"
#include <garnet/GNrdg.h>

namespace GN::rdg {

const Matrix44f & AffineTransform::matrix() const { return mMatrix; }

} // namespace GN::rdg
