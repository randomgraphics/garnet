#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>        // glm::quat, glm::mat4_cast, glm::angleAxis
#include <glm/gtc/matrix_transform.hpp>  // glm::translate, glm::scale, glm::normalize
#include <glm/ext/matrix_clip_space.hpp> // glm::perspectiveRH_ZO
#include <glm/ext/matrix_transform.hpp>  // glm::lookAtRH

namespace GN::rdg {

using Location    = Vector3<WorldUnit>;
using Orientation = Quaternionf;

/// General affine transform: translation, rotation and non-uniform scaling.
/// Uses GLM internally for SIMD-friendly TRS matrix construction.
/// All methods are defined in the class body and are implicitly inline.
struct AffineTransform {
    void reset() {
        mPosition = glm::vec3(0.f);
        mRotation = glm::quat(1.f, 0.f, 0.f, 0.f);
        mScale    = glm::vec3(1.f);
        mMatrix   = glm::mat4(1.f);
        mDirty    = false;
    }

    void setLocation(glm::vec3 position) {
        mPosition = position;
        mDirty    = true;
    }
    void setRotation(glm::quat rotation) {
        mRotation = rotation;
        mDirty    = true;
    }
    void setRotation(float angle, glm::vec3 axis) {
        mRotation = glm::angleAxis(angle, glm::normalize(axis));
        mDirty    = true;
    }
    void setUniformScale(float scale) {
        mScale = glm::vec3(scale);
        mDirty = true;
    }
    void setNonUniformScale(glm::vec3 scale) {
        mScale = scale;
        mDirty = true;
    }

    /// Returns the TRS matrix (Translate * Rotate * Scale), rebuilt lazily.
    const glm::mat4 & matrix() const {
        if (!mDirty) return mMatrix;
        mDirty  = false;
        mMatrix = glm::translate(glm::mat4(1.f), mPosition) * glm::mat4_cast(mRotation) * glm::scale(glm::mat4(1.f), mScale);
        return mMatrix;
    }

private:
    glm::vec3         mPosition = glm::vec3(0.f);
    glm::quat         mRotation = glm::quat(1.f, 0.f, 0.f, 0.f);
    glm::vec3         mScale    = glm::vec3(1.f);
    mutable glm::mat4 mMatrix   = glm::mat4(1.f);
    mutable bool      mDirty    = false;
};

/// World → view → clip transform chain for a perspective camera.
/// Uses GLM throughout; all methods implicitly inline.
struct WorldToClipTransformChain {
    glm::vec3 eye    = {0.f, 0.f, 5.f};
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec3 up     = {0.f, 1.f, 0.f};
    float     fovy   = glm::radians(60.f); ///< vertical FOV in radians
    float     aspect = 16.f / 9.f;
    float     znear  = 0.1f;
    float     zfar   = 100.f;

    void setCamera(glm::vec3 e, glm::vec3 t, glm::vec3 u) {
        eye    = e;
        target = t;
        up     = u;
    }
    void setPerspective(float f, float a, float n, float r) {
        fovy   = f;
        aspect = a;
        znear  = n;
        zfar   = r;
    }

    /// Returns view-projection matrix. Uses RH coordinates and [0,1] depth (Vulkan).
    /// Row 1 of the projection is negated to flip Y for Vulkan's Y-down NDC.
    glm::mat4 matrix() const {
        glm::mat4 proj = glm::perspectiveRH_ZO(fovy, aspect, znear, zfar);
        proj[1][1] *= -1.f; // Vulkan NDC has Y pointing down
        return proj * glm::lookAtRH(eye, target, up);
    }
};

} // namespace GN::rdg
