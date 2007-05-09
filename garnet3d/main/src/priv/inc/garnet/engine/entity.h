#ifndef __GN_ENGINE_ENTITY_H__
#define __GN_ENGINE_ENTITY_H__
// *****************************************************************************
//! \file    engine/entity.h
//! \brief   entity class
//! \author  chenli@@FAREAST (2007.5.9)
// *****************************************************************************

namespace GN
{
    template< class T, class H>
    class NamedHandleManager
    {
        typedef std::map<StrA,H> NameMap;

        struct NamedItem
        {
            T    data;
            StrA name;

            NamedItem( const T & d, const StrA & n ) : data(d), name(n) {}

            NamedItem( const StrA & n ) : name(n) {}
        };

        NameMap                    mNames; // name -> handle
        HandleManager<NamedItem,H> mItems; // handle -> name/data

    public:

        //@{

        ///
        /// clear all handles
        ///
        void clear()
        {
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
        H next( H h ) const { return mItems.next(); }

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

            H h = mItems.add( NamedItem(name) );
            if( 0 == h ) return 0;

            mNames.insert( std::make_pair(name,h) );

            return h;
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

            H h = mItems.add( NamedItem(name,data) );
            if( 0 == h ) return 0;

            mNames.insert( std::make_pair(name,h) );

            return h;
        }

        void remove( H h )
        {
            if( !validHandle( h ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid handle: %d.", h );
                return;
            }

            NamedItem & item = mItems[h];

            mNames.erase( item.name );

            mItems.remove( h );
        }

        void remove( const StrA & name )
        {
            if( !validName( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid name: %s.", name.cptr() );
                return;
            }

            H h( mNames[name] );

            mNames.erase( name );
            mItems.remove( h );
        }

        bool validHandle( H h ) const
        {
            return mItems.validHandle( h );
        }

        bool validName( const StrA & name ) const
        {
            return mNames.end() != mNames.find( name );
        }

        T & get( H h ) const
        {
            return mItems.get( h );
        }

        T & get( const StrA & name ) const
        {
            GN_ASSERT( validName(name) );
            NameMap::const_iterator i = mNames.find( name );
            return mItems.get( i->second );
        }

        T & operator[]( H h ) const { return get(h); }

        T & operator[]( const StrA & name ) const { return get(name); }

        //@}
    };
};

namespace GN { namespace engine
{
    ///
    /// entity ID
    ///
    typedef UInt32 EntityId;

    ///
    /// entity type ID.
    ///
    typedef UInt32 EntityTypeId;

    class EntityManager;

    ///
    /// Entity structure
    ///
    struct Entity : public NoCopy
    {
        EntityManager    & manager;
        const EntityId     id;
        const EntityTypeId type;
        const StrA         name;

    protected:

        //@{
        Entity( EntityManager & m, EntityId i, EntityTypeId t, const StrA & n )
            : manager(m)
            , id(i)
            , type(t)
            , name(n)
        {}

        ~Entity() {}
        //@}
    };

    ///
    /// templated entity structure
    ///
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
        /// name should be unique
        ///
        EntityTypeId createEntityType( const StrA & name );

        ///
        /// add new entity. name must be unique.
        ///
        template<class T>
        EntityId newEntity( EntityTypeId type, const T & data, const StrA & name );

        void removeEntity( EntityId );
        void removeEntity( const StrA & name );

        Entity * getEntity( EntityId );
        Entity * getEntity( const StrA & name );
        EntityId getEntityId( const StrA & name );

        // iteration
        EntityId getFirst() const;
        EntityId getFirst( EntityTypeId ) const;
        EntityId getNext( EntityId ) const;
        EntityId getNextWithSameType( EntityId ) const;

        //@]

    private:

        template<class T>
        struct EntityItem : public EntityT<T>, public DoubleLinkedItem<Entity*>
        {
        };

        struct EntityTypeItem
        {
            DoubleLinkedList<Entity*> mItems;
        };

        static Logger                                 * sLogger;
        NamedHandleManager<Entity*,EntityId>            mEntities;
        NamedHandleManager<EntityTypeItem,EntityTypeId> mTypes;
    };
}}

#include "entity.inl"


// *****************************************************************************
//                           End of entity.h
// *****************************************************************************
#endif // __GN_ENGINE_ENTITY_H__
