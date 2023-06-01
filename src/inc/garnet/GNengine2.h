#ifndef __GN_ENGINE2_GNENGINE2_H__
    #define __GN_ENGINE2_GNENGINE2_H__
// *****************************************************************************
/// \file
/// \brief   main interface of engine v2
/// \author  CHEN@@CHENLI-HOMEPC (2015.4.8)
// *****************************************************************************

namespace GN::e2 {

class Transformation {
    Vector3f    translation;
    Quaternionf rotation;
    Vector3f    scaling;
};

class Entity : public RefCounter {
    // basic properties
    uint64_t _id;
    StrA   _name;
};

class SpacialEntity : public Entity {
    // spacial properties
    WeakRef<Entity> _spacialParent;
    Position        _worldPosition; // only when _spacialParent is empty.
    Spheref         _boundingSphere;
    Transformation  _transformation; // local to parent transformation
};

class VisualEntity : SpacialEntity {
public:
    virtual void RenderPass0();
    virtual void RenderPass1();
    virtual void RenderPass2();
};

// Camera object
class Camera : public VisualEntity {};

// The geometry object that never changes
class ImmutableGeometry : public VisualEntity {};

class SceneGraph {
public:
    void AddEntity(SpacialEntity *);
    void RemoveEntity(uint64_t id);
};

class AsyncNofitication : RefCounter {
public:
    enum Status {
        PENDING = -2,
        ABORT   = -1,
        FAILED  = 0,
        DONE    = 1,
    };

    void   Notify(OperationResult);
    Status Pool();
    void   WaitForFinish();
};

/// @brief The visual/rendering system of the engine.
class VisualSystem {
public:
    /// @brief The parametrers to initialize/reboot the visual system.
    struct VisualSubsystemParameters {
        // reserved for future use.
    };

    /// @brief Reboot the visual/rendering system.
    void rebootVisual();
};

/// @brief The top level object of the engine. Represents the world that is currently running.
class Universe {
public:
    /// Reboot the whole universe. Initialize the foundation. Setup an empty universe. If the universe contains anything, it'll be completely removed.
    void reboot();
};

} // namespace GN::e2

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_GNENGINE2_H__
#include "pch.h"
