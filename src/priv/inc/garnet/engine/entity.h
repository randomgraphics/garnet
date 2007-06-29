#ifndef __GN_ENGINE_ENTITY_H__
#define __GN_ENGINE_ENTITY_H__
// *****************************************************************************
/// \file
/// \brief   entity class
/// \author  chenli@@FAREAST (2007.5.9)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// entity type ID.
    ///
    typedef UInt32 EntityTypeId;

    class EntityManager;

    ///
    /// Basic entity structure
    ///
    struct Entity : public NoCopy
    {
        /// entity properties
        //@{
        EntityManager    & manager;
        const StrA         name;
        const EntityTypeId type;
        const UIntPtr      id; ///< this is used internally by Entity manager to identify the entity.
        //@}

    protected:

        //@{
        Entity( EntityManager & m, const StrA & n, EntityTypeId t, UIntPtr i )
            : manager(m)
            , name(n)
            , type(t)
            , id(i)
        {}

        virtual ~Entity() {}
        //@}
    };

    ///
    /// templated entity structure
    ///
    template<class T>
    struct EntityT : public Entity
    {
        T                  data; ///< user data
        Delegate1<void,T&> dtor; ///< Optional data destructor, that is used to destroy user data, while entity is deleted.

    protected:

        //@{

        EntityT( EntityManager & m, const StrA & n, EntityTypeId t, UIntPtr i, const T & d, const Delegate1<void,T&> & dt )
            : Entity( m, n, t, i ), data(d), dtor(dt)
        {}

        ~EntityT()
        {
            if( dtor ) dtor( data );
        }

        //@}
    };

    ///
    /// entity manager
    ///
    class EntityManager
    {
    public:

        //@{

        EntityManager();

        ~EntityManager();

        //@}

        //@{

        ///
        /// create new entity type. Name should be unique
        ///
        EntityTypeId createEntityType( const StrA & name );
        
        ///
        /// create new entity. Name must be unique.
        ///
        template<class T>
        EntityT<T> *
        createEntity(
            EntityTypeId               type,
            const StrA               & name,
            const T                  & data,
            const Delegate1<void,T&> & dtor );// = Delegate1<void,T&>::NULL_DELEGATE );

        // delete
        void deleteEntity( const Entity * );
        void deleteEntityByName( const StrA & name );
        void deleteAllEntities();

        // check
        bool checkEntityType( EntityTypeId, bool silence = false ) const;
        bool checkEntity( const Entity *, bool silence = false ) const;

        // get
        Entity * getEntityByName( const StrA & name, bool silence = false ) const;
        EntityTypeId getEntityTypeByName( const StrA & name, bool silence = false ) const;

        // iterate
        Entity * getFirstEntity() const;
        Entity * getFirstEntity( EntityTypeId ) const;

        Entity * getNextEntity( const Entity * );
        Entity * getNextEntityWithSameType( const Entity * );

        EntityTypeId getFirstEntityType() const;
        EntityTypeId getNextEntityType( EntityTypeId ) const;

        //@}

    private:

        static Logger * sLogger;

        struct EntityDeletor
        {
            virtual ~EntityDeletor() {}
        };

        template<typename T>
        struct EntityItemT : public EntityT<T>, public EntityDeletor
        {
            EntityItemT( EntityManager & m, const StrA & n, EntityTypeId t, UIntPtr i, const T & d, const Delegate1<void,T&> & dt )
                : EntityT<T>( m, n, t, i, d, dt )
            {}
        };

        struct EntityCategory
        {
            HandleManager<Entity*,UIntPtr> entities;
        };

        NamedHandleManager<EntityCategory,EntityTypeId> mEntityTypes; // type -> category
        std::map<StrA,Entity*>                          mEntityNames; // name -> entity
    };

    //@{

    ///
    /// convert entity to the object that it represents. Return 'nil' object for invalid entity.
    ///
    template< class T>
    const T & entity2Object( const Entity * e, const T & nil );

    ///
    /// delete specific entity
    ///
    void safeDeleteEntity( const Entity * & );

    ///
    /// delete specific entity
    ///
    void safeDeleteEntity( Entity * & );

    // interation
    Entity * getNextEntity( const Entity * );
    Entity * getNextEntityWithSameType( const Entity * );

    //@}
}}

#include "entity.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_ENTITY_H__
