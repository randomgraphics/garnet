#ifndef __GN_ENGINE2_GNENGINE2_H__
    #define __GN_ENGINE2_GNENGINE2_H__
// *****************************************************************************
/// \file
/// \brief   main interface of engine v2
/// \author  CHEN@@CHENLI-HOMEPC (2015.4.8)
// *****************************************************************************

namespace GN {
namespace e2 {
class Position {
    sint64 x, y, z;
};

class Transformation {
    Vector3f    translation;
    Quaternionf rotation;
    Vector3f    scaling;
};

class Entity : public RefCounter {
    // basic properties
    uint64 _id;
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
    void RemoveEntity(uint64 id);
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

// self contained world object.
class World {
public:
    void LoadAsync(File &);
    void SaveAsync(File &);
    void PurgeAsync();
    void Render(UINT pass);
};

class Universe {
    uint64 GenerateUniqueId();
};
} // namespace e2
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_GNENGINE2_H__
#include "pch.h"
