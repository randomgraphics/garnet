#include "pch.h"
#include <garnet/GNrdg.h>

namespace GN::rdg {

void AffineTransform::setLocation(Vector3f position) {
    mPosition = position;
    mIsDirty  = true;
}

void AffineTransform::setRotation(Quaternionf rotation) {
    mRotation = rotation;
    mIsDirty  = true;
}

void AffineTransform::setRotation(float angle, Vector3f axis) {
    mRotation.fromRotation(axis, angle);
    mIsDirty = true;
}

void AffineTransform::setUniformScale(float scale) {
    mScale   = {scale, scale, scale};
    mIsDirty = true;
}

void AffineTransform::setNonUniformScale(Vector3f scale) {
    mScale   = scale;
    mIsDirty = true;
}

const Matrix44f & AffineTransform::matrix() const {
    if (!mIsDirty) return mMatrix;
    mIsDirty = false;
    // Build TRS = Translate * Rotate * Scale (column-vector convention)
    mRotation.toMatrix44(mMatrix);
    for (int row = 0; row < 3; ++row) {
        mMatrix.rows[row][0] *= mScale.x;
        mMatrix.rows[row][1] *= mScale.y;
        mMatrix.rows[row][2] *= mScale.z;
        mMatrix.rows[row][3] = (&mPosition.x)[row];
    }
    mMatrix.rows[3].set(0.f, 0.f, 0.f, 1.f);
    return mMatrix;
}

} // namespace GN::rdg
