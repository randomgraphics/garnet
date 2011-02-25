#ifndef __GN_ENGINE_PHYSICS_COMPONENT_H__
#define __GN_ENGINE_PHYSICS_COMPONENT_H__
// *****************************************************************************
/// \file
/// \brief   Physics component
/// \author  chen@@chenli-homepc (2011.2.24)
// *****************************************************************************

namespace GN { namespace engine
{
    /// Physics component. Define object location, shape and other physical properties.
    class PhysicalComponent : public Component
    {
    public:

        static const Guid & sGetType();

        const Vector3    & getScale() const;
        const Vector3    & getPosition() const;
        const Quaternion & getRotation() const;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_PHYSICS_COMPONENT_H__
