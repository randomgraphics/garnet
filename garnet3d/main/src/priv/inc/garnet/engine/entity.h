#ifndef __GN_ENGINE_ENTITY_H__
#define __GN_ENGINE_ENTITY_H__
// *****************************************************************************
//! \file    engine/entity.h
//! \brief   entity class
//! \author  chenli@@FAREAST (2007.5.9)
// *****************************************************************************

namespace GN
{
    ///
    /// expension to HandleManager class, that object can be referenced by both handle and name.
    ///
    template< class T, class H>
    class NamedHandleManager
    {
        typedef std::map<StrA,H> NameMap;

        struct NamedItem
        {
            NamedHandleManager & mgr;
            const H              handle;
            const StrA           name;
            T                    data;

            NamedItem( NamedHandleManager & m, H h, const StrA & n, const T & d )
                : mgr(m), handle(h), name(n), data(d) {}

            NamedItem( NamedHandleManager & m, H h, const StrA & n ) 
                : mgr(m), handle(h), name(n) {}
        };

        NameMap                     mNames; // name -> handle
        HandleManager<NamedItem*,H> mItems; // handle -> name/data

    public:

        //@{

        typedef typename H ItemHandle;

        ///
        /// clear all handles
        ///
        void clear()
        {
            for( size_t i = mItems.first(); i != 0; i = mItems.next( i ) )
            {
                delete mItems[i];
            }
            mItems.clear();
            mNames.clear();
        }

        ///
        /// Get number of handles
        ///
        size_t size() const
        {
            GN_ASSERT( mItems.size() == mNames.size() );
            return mItems.size();
        }

        ///
        /// Is the manager empty or not.
        ///
        bool empty() const
        {
            GN_ASSERT( mItems.size() == mNames.size() );
            return mItems.empty();
        }

        ///
        /// return first handle
        ///
        H first() const { return mItems.first(); }

        ///
        /// return next handle
        ///
        H next( H h ) const { return mItems.next( h ); }

        ///
        /// name must be unique.
        ///
        H add( const StrA & name )
        {
            if( mNames.end() != mNames.find( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.cptr() );
                return 0;
            }

            H handle = mItems.newItem();
            if( 0 == handle ) return 0;

            mItems[handle] = new NamedItem(*this,handle,name);

            mNames.insert( std::make_pair(name,handle) );

            return handle;
        }

        ///
        /// name must be unique.
        ///
        H add( const StrA & name, const T & data )
        {
            if( mNames.end() != mNames.find( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.cptr() );
                return 0;
            }

            size_t handle = mItems.newItem();
            if( 0 == handle ) return 0;

            mItems[handle] = new NamedItem(*this,handle,name,data);

            mNames.insert( std::make_pair(name,handle) );

            return handle;
        }

        void remove( H h )
        {
            if( !validHandle( h ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid handle : %d.", h );
                return;
            }

            NamedItem * item = mItems[h];
            GN_ASSERT( item && item->handle == h );

            mNames.erase( item->name );

            mItems.remove( item->handle );

            delete item;
        }

        void remove( const StrA & name )
        {
            if( !validName( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid name: %s.", name.cptr() );
                return;
            }

            size_t handle = mNames[name];

            NamedItem * item = mItems[handle];

            mNames.erase( name );

            mItems.remove( handle );

            delete item;
        }

        bool validHandle( H h ) const { return mItems.validHandle( h ); }

        bool validName( const StrA & name ) const { return mNames.end() != mNames.find( name ); }

        H name2handle( const StrA & name ) const
        {
            NameMap::const_iterator i = mNames.find( name );
            if( mNames.end() == i )
            {
                return (H)0;
            }
            else
            {
                return i->second;
            }
        }

        StrA handle2name( H h ) const
        {
            if( !mItems.validHandle( h ) )
                return StrA::EMPTYSTR;
            else
                return mItems[h]->name;
        }

        T & get( H h ) const { return mItems[h]->data; }

        T & get( const StrA & name ) const
        {
            GN_ASSERT( validName(name) );
            NameMap::const_iterator i = mNames.find( name );
            return mItems[mItems.get( i->second )]->data;
        }

        T & operator[]( H h ) const { return get(h); }

        T & operator[]( const StrA & name ) const { return get(name); }

        //@}
    };
};

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
        EntityT<T> * createEntity( EntityTypeId type, const StrA & name, const T & data, const Delegate1<void,T&> & dtor = Delegate1<void,T&>() );

        // delete
        void deleteEntity( const Entity * );
        void deleteEntityByName( const StrA & name );

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
                : EntityT( m, n, t, i, d, dt )
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
    void deleteEntity( Entity * );

    // interation
    Entity * getNextEntity( const Entity * );
    Entity * getNextEntityWithSameType( const Entity * );

    //@}
}}

#include "entity.inl"

// *****************************************************************************
//                           End of entity.h
// *****************************************************************************
#endif // __GN_ENGINE_ENTITY_H__
