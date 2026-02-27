#pragma once

namespace GN::rdg {

using Location = Vector3<WorldUnit>;

/// General affine transform: translation, rotation and scaling. It can be represented by a 4x4 matrix.
/// But internally, it could use other more efficient representations, such as quaternion, etc.
struct AffineTransform {
    /// reset to identity transform.
    void reset() {
        mPosition = {0.0f, 0.0f, 0.0f};
        mRotation = Quaternionf::identity();
        mScale    = {1.0f, 1.0f, 1.0f};
        mMatrix   = Matrix44f::identity();
        mIsDirty  = false;
    }

    /// move the transform by a delta vector.
    void setLocation(Vector3f position);    

    /// rotate the transform by a angle around an axis.
    void setRotation(Quaternionf rotation);

    void setRotation(float angle, Vector3f axis);

    /// uniform scaling
    void setUniformScale(float scale);

    /// non uniform scaling
    void setNonUniformScale(Vector3f scale);

    /// get the matrix representation of the transform.
    const Matrix44f & matrix() const;

private:
    Vector3f    mPosition = {0.0f, 0.0f, 0.0f};
    Quaternionf mRotation = Quaternionf::identity();
    Vector3f    mScale    = {1.0f, 1.0f, 1.0f};
    Matrix44f   mMatrix   = Matrix44f::identity();
    bool        mIsDirty  = false;
};

/// The conventional world -> view -> clip transformation.
struct WorldToClipTransform {
    /// \todo: to be implemented.
} // namespace GN::rdg