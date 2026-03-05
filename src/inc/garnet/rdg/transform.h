#pragma once

namespace GN::rdg {

using Location    = Vector3<WorldUnit>;
using Orientation = Quaternionf;

/// General affine transform: translation, rotation and scaling. It can be represented by a 4x4 matrix.
/// But internally, it could use other more efficient representations, such as quaternion, etc.
struct AffineTransform {
    /// reset to identity transform.
    void reset() {
        mPosition = {0.0f, 0.0f, 0.0f};
        mRotation = Quaternionf::sIdentity();
        mScale    = {1.0f, 1.0f, 1.0f};
        mMatrix   = Matrix44f::sIdentity();
        mIsDirty  = false;
    }

    /// Set the translation component.
    GN_API void setLocation(Vector3f position);

    /// Set the rotation component from a quaternion.
    GN_API void setRotation(Quaternionf rotation);

    /// Set the rotation component from an angle (radians) and axis.
    GN_API void setRotation(float angle, Vector3f axis);

    /// Set uniform scaling on all axes.
    GN_API void setUniformScale(float scale);

    /// Set non-uniform scaling per axis.
    GN_API void setNonUniformScale(Vector3f scale);

    /// Get the 4×4 matrix representation of this transform (lazily rebuilt).
    GN_API const Matrix44f & matrix() const;

private:
    Vector3f            mPosition = {0.0f, 0.0f, 0.0f};
    Quaternionf         mRotation = Quaternionf::sIdentity();
    Vector3f            mScale    = {1.0f, 1.0f, 1.0f};
    mutable Matrix44f   mMatrix   = Matrix44f::sIdentity();
    mutable bool        mIsDirty  = false;
};

/// The conventional world -> view -> clip transformation.
struct WorldToClipTransformChain {
    Vector3f eye    = {0.0f, 0.0f, 5.0f};
    Vector3f target = {0.0f, 0.0f, 0.0f};
    Vector3f up     = {0.0f, 1.0f, 0.0f};
    float    fovy   = GN_PI / 3.0f; ///< vertical field of view in radians
    float    aspect = 16.0f / 9.0f;
    float    znear  = 0.1f;
    float    zfar   = 100.0f;

    void setCamera(Vector3f eye_, Vector3f target_, Vector3f up_) {
        eye    = eye_;
        target = target_;
        up     = up_;
    }

    void setPerspective(float fovy_, float aspect_, float znear_, float zfar_) {
        fovy   = fovy_;
        aspect = aspect_;
        znear  = znear_;
        zfar   = zfar_;
    }

    /// Returns the combined world-to-clip matrix (view * projection).
    /// Projection uses D3D-style depth [0, 1] with Y-axis negated for Vulkan NDC.
    Matrix44f matrix() const {
        Matrix44f view;
        view.lookAtRh(eye, target, up);
        Matrix44f proj;
        proj.perspectiveD3DRh(fovy, aspect, znear, zfar);
        // Vulkan NDC has Y pointing down; negate projection row 1 to flip Y.
        proj.rows[1] = -proj.rows[1];
        return proj * view;
    }
};

} // namespace GN::rdg