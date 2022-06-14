#include "pch.h"

// *****************************************************************************
// SpacialComponent public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static const GN::Guid SPATIAL_COMPONENT_GUID = {
    0xaab1738d,
    0x3bc7,
    0x424d,
    {0x90, 0x35, 0x49, 0x19, 0x47, 0xef, 0xa7, 0x20},
};
GN_ENGINE_IMPLEMENT_ENTITY(GN::engine::SpacialComponent, SPATIAL_COMPONENT_GUID);

//
//
// -----------------------------------------------------------------------------
GN::engine::SpacialComponent::SpacialComponent()
    : mPosition(0, 0, 0), mRotation(0, 0, 0, 1), mScale(1, 1, 1), mSelfBBox(0, 0, 0, 0, 0, 0), mLocal2Parent(Matrix44f::sIdentity()),
      mParent2Local(Matrix44f::sIdentity()), mLocal2Root(Matrix44f::sIdentity()), mRoot2Local(Matrix44f::sIdentity()), mTransformDirty(false),
      mBBoxDirty(false) {
    mTreeNode.owner = this;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::SpacialComponent::~SpacialComponent() {}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::setParent(SpacialComponent * parent, SpacialComponent * prevSibling) {
    if (parent != getParent() || prevSibling != getPrevSibling()) {
        mTreeNode.setParent(sToTreeNode(parent), sToTreeNode(prevSibling));
        invalidateTransform();
        invalidateBoundingBox();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::setPosition(const Vector3f & p) {
    if (p != mPosition) {
        mPosition = p;
        invalidateTransform();
        invalidateBoundingBox();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::setRotation(const Quaternionf & q) {
    if (q != mRotation) {
        mRotation = q;
        invalidateTransform();
        invalidateBoundingBox();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::setScale(const Vector3f & s) {
    if (s != mScale) {
        mScale = s;
        invalidateTransform();
        invalidateBoundingBox();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::setSelfBoundingBox(const Boxf & b) {
    mSelfBBox = b;
    invalidateBoundingBox();
}

// *****************************************************************************
// SpacialComponent private methods
// *****************************************************************************

//
// Invalidate transformation of all nodes down in the subtree.
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::invalidateTransform() {
    if (!mTransformDirty) {
        mTransformDirty = true;
        for (SpacialComponent * child = getFirstChild(); child; child = child->getNextSibling()) { child->invalidateTransform(); }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::calcTransform() {
    GN_ASSERT(mTransformDirty);

    // TODO: optimize with SIMD instructions

    Matrix33f r33;
    Matrix44f r, t, s;

    // object will be:
    //
    //  1. scaled in its local space
    //  2. rotated around its local origin
    //  3. moved to localtion defined by "position" in parent space

    mRotation.toMatrix33(r33);
    r.set(r33);

    t.translate(mPosition);

    s.identity();
    s[0][0] = mScale[0];
    s[1][1] = mScale[1];
    s[2][2] = mScale[2];

    mLocal2Parent = t * r * s;
    mParent2Local = Matrix44f::sInverse(mLocal2Parent);

    SpacialComponent * parent = getParent();
    if (parent) {
        mLocal2Root = parent->getLocal2Root() * mLocal2Parent;
        mRoot2Local = Matrix44f::sInverse(mLocal2Root);
    } else {
        mLocal2Root = mLocal2Parent;
        mRoot2Local = mParent2Local;
    }

    mTransformDirty = false;
}

//
// Invalidate bounding box of all components up in the tree
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::invalidateBoundingBox() {
    if (!mBBoxDirty) {
        mBBoxDirty = true;
        for (SpacialComponent * parent = getParent(); parent; parent = parent->getParent()) { parent->mBBoxDirty = true; }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SpacialComponent::calcBoundingBox() {
    GN_ASSERT(mBBoxDirty);

    mUberBBox = mSelfBBox;
    mUberBBox.normalize();

    // gather child bounding boxes
    for (SpacialComponent * child = getFirstChild(); child; child = child->getNextSibling()) {
        const Boxf & childbb = child->getUberBoundingBox();

        // translate child bounding box into this component's local space
        const Matrix44f & mat = child->getLocal2Parent();

        for (int i = 0; i < 8; ++i) {
            Vector4f v(childbb.corner(i), 1.0f);
            v = mat * v;

            if (v.x < mUberBBox.x) {
                mUberBBox.w += mUberBBox.x - v.x;
                mUberBBox.x = v.x;
            } else if (v.x > (mUberBBox.x + mUberBBox.w)) {
                mUberBBox.w = v.x - mUberBBox.x;
            }

            if (v.y < mUberBBox.y) {
                mUberBBox.h += mUberBBox.y - v.y;
                mUberBBox.y = v.y;
            } else if (v.y > (mUberBBox.y + mUberBBox.h)) {
                mUberBBox.h = v.y - mUberBBox.y;
            }

            if (v.z < mUberBBox.z) {
                mUberBBox.d += mUberBBox.z - v.z;
                mUberBBox.z = v.z;
            } else if (v.z > (mUberBBox.z + mUberBBox.d)) {
                mUberBBox.d = v.z - mUberBBox.z;
            }
        }
    }

    mBBoxDirty = false;
}
