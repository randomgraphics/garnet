#ifndef __GN_ENGINE2_GNENGINE2_H__
#define __GN_ENGINE2_GNENGINE2_H__
// *****************************************************************************
/// \file
/// \brief   main interface of engine v2
/// \author  CHEN@@CHENLI-HOMEPC (2015.4.8)
// *****************************************************************************

namespace GN { namespace e2
{
    class Position
    {
        sint64 x, y, z;
    };

    class Transformation
    {
        Vector3f    translation;
        Quaternionf rotation;
        Vector3f    scaling;
    };

    class Entity : public RefCounter
    {
        // basic properties
        uint64          _id;
        StrA            _name;

        // spacial properties
        WeakRef<Entity> _spacialParent;
        Position        _worldPosition;  // only when _spacialParent is empty.
        Spheref         _boundingSphere;
        Transformation  _trahsformation; // local to parent transformation
    };

    class Scene : public StdClass
    {
    public:

        uint64 GetUniqueId();

    };

    extern Scene gScene; ///< global scene instance
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_GNENGINE2_H__
#include "pch.h"
