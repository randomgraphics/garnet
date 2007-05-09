#ifndef __GN_ENGINE_ENTITY_H__
#define __GN_ENGINE_ENTITY_H__
// *****************************************************************************
//! \file    engine/entity.h
//! \brief   entity class
//! \author  chenli@@FAREAST (2007.5.9)
// *****************************************************************************

namespace GN { namespace engine
{
    typedef int EntityId;
    typedef int EntityTypeId;

    class EntityManager;

    struct Entity : public NoCopy
    {
        EntityManager    & manager;
        const EntityId     id;
        const EntityTypeId type;
        const StrA         name;

    protected:

        Entity( EntityManager & m, EntityId i, EntityTypeId t, const StrA & n )
            : manager(m)
            , id(i)
            , type(t)
            , name(n)
        {}

        ~Entity() {}
    };

    template<class T>
    struct EntityT : public Entity
    {
        T data;

    protected:

        EntityT( EntityManager & m, EntityId i, EntityTypeId t, const StrA & n )
            : Entity( m, i, t, n )
        {}

        ~EntityT() {}
    };

    class EntityManager
    {
    public:

        EntityTypeId createEntityType();

        template<class T>
        EntityId newEntity( EntityTypeId type, const T & data, const StrA & name );

        void removeEntity( EntityId );
        void removeEntity( const StrA & name );

        Entity * getEntity( EntityId );
        Entity * getEntity( const StrA & name );
    };
}}

// *****************************************************************************
//                           End of entity.h
// *****************************************************************************
#endif // __GN_ENGINE_ENTITY_H__
