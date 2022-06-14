#ifndef __GN_ENGINE_MESH_H__
#define __GN_ENGINE_MESH_H__
// *****************************************************************************
/// \file
/// \brief   Mesh entities
/// \author  chen@@chenli-homepc (2011.2.28)
// *****************************************************************************

// Forward declaration.
namespace GN {
namespace gfx {
struct FatModel;
struct FatMeshSubset;
} // namespace gfx
} // namespace GN

namespace GN {
namespace engine {
///
/// Static mesh entity
///
class GN_API StaticMesh : public Entity {
    GN_ENGINE_DECLARE_ENTITY(StaticMesh, Entity);

public:
    /// constructor
    StaticMesh();

    /// destructor
    ~StaticMesh();

    /// get the spacial component
    const SpacialComponent & spacial() const { return mSpacial; }

    /// get the spacial component
    SpacialComponent & spacial() { return mSpacial; }

    /// get the visual component
    const VisualComponent & visual() const { return mVisual; }

    /// get the visual component
    VisualComponent & visual() { return mVisual; }

    /// Load the whole model hierarchy as a single static mesh.
    /// Existing content in the mesh will be discarded.
    bool loadFromModelHierarchy(const gfx::ModelHierarchyDesc &);

    /// Load the whole fat model as single static mesh
    bool loadFromFatModel(const GN::gfx::FatModel &);

    /// Load from file
    bool loadFromFile(const StrA & filename);

private:
    SpacialComponent mSpacial;
    VisualComponent  mVisual;
};

struct SkinnedAnimationInfo {
    StrA   name;
    double duration;
};

///
/// Skinned Mesh Entity
///
class GN_API SkinnedMesh : public Entity {
    GN_ENGINE_DECLARE_ENTITY(SkinnedMesh, Entity);

public:
    /// Get joint count threshold per draw.
    static uint32 sGetMaxJointsPerDraw();

    /// constructor
    SkinnedMesh();

    /// destructor
    ~SkinnedMesh();

    /// Clear to an empty mesh.
    void clear();

    /// get the spacial component of the skeleton root.
    const SpacialComponent & spacial() const { return mRootSpacial; }

    /// get the spacial component
    SpacialComponent & spacial() { return mRootSpacial; }

    /// get the visual component
    const VisualComponent & visual() const { return *mVisual; }

    /// get the visual component
    VisualComponent & visual() { return *mVisual; }

    /// get number of animations
    size_t getAnimationCount() const { return mAnimations.size(); }

    /// get information about specific animation.
    bool getAnimationInfo(size_t animationIndex, SkinnedAnimationInfo & info);

    /// Set the animation and the time stamp.
    /// \param animationIndex
    ///     Specify index of the animation in the animation arrary.
    ///     Set this to -1 to render the skeleton in bind pose.
    /// \param seconds
    ///     Specify time stamp in the animation. Ignored when
    ///     animationIndex is -1.
    void setAnimation(size_t animationIndex, float seconds);

    /// Load the whole fat model as single skinned mesh
    bool loadFromFatModel(const GN::gfx::FatModel &);

    /// Load from file
    bool loadFromFile(const StrA & filename);

    /// Draw skeletons as line segments.
    void drawSkeletons(uint32 colorInRGBA, const Matrix44f & transform);

private:
    class SkinnedVisualComponent;

    struct SkinnedAnimation;

    struct SkinnedSubset {
        /// Index of the skeleton into mSkeletons.
        uint32 skeleton;

        /// Joints used by the subset.
        DynaArray<uint32, uint32> joints;
    };

    struct JointHierarchy {
        uint32 parent;
        uint32 child;
        uint32 sibling;
    };

    struct JointBindPose {
        // Global transformation from model space to joint space.
        Matrix44f model2joint;

        // Local transformation (relative to parent joint)
        Vector3f    position; //< Position in parent space
        Quaternionf rotation; //< Rotation in local space
        Vector3f    scaling;  //< Scaling in local space.
    };

    struct Skeleton {
        uint32 jointCount;

        /// Joint hierarchy. The first joint is always the root.
        JointHierarchy * hierarchy;

        /// Spacial components for each joint. Array size equals joint count.
        /// The first spacial component always represents the root joint.
        SpacialComponent ** spacials;

        /// Stores bind pose of the skeleton. Array size equals joint count.
        /// This array contains constant transform information for each joint
        /// that are initialized when the mesh is loaded.
        JointBindPose * bindPose;

        /// Store inverse of the active rest pose of the skeleton. Array size equals joint count.
        /// Matrices in this array are updated by animation.
        Matrix44f * invRestPose;

        /// Bind pose to rest pose transformations for each joint. Array size equals joint count.
        /// Matrices in this array are updated by animation.
        Matrix44f * bind2rest;

        /// Uniform resource that stores bind pose to rest pose matrices for the current subset.
        /// Note that uniform size might small than joint count.
        gfx::UniformResource * matrices;
    };

    SpacialComponent              mRootSpacial;
    VisualComponent *             mVisual;
    AutoRef<gfx::EffectResource>  mSkinnedEffect;
    DynaArray<Skeleton>           mSkeletons;
    DynaArray<SkinnedAnimation *> mAnimations;
    DynaArray<SkinnedSubset>      mSubsets; // One subset for each model in visual component.
};
} // namespace engine
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_MESH_H__
