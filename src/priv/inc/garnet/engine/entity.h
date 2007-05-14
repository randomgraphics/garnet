#ifndef __GN_ENGINE_ENTITY_H__
#define __GN_ENGINE_ENTITY_H__
// *****************************************************************************
//! \file    engine/entity.h
//! \brief   entity class
//! \author  chenli@@FAREAST (2007.5.9)
// *****************************************************************************

namespace GN
{
    template< class T>
    class NamedHandleManager
    {
        typedef std::map<StrA,size_t> NameMap;

        struct NamedItem
        {
            NamedHandleManager & mgr;
            const size_t         id;
            const StrA           name;
            T                    data;

            NamedItem( NamedHandleManager & m, size_t i, const StrA & n, const T & d )
                : mgr(m), id(i), name(n), data(d) {}

            NamedItem( NamedHandleManager & m, size_t i, const StrA & n ) 
                : mgr(m), id(i), name(n) {}
        };

        NameMap                          mNames; // name -> handle
        HandleManager<NamedItem*,size_t> mItems; // handle -> name/data

    public:

        ///
        /// public handle type
        ///
        typedef NamedItem * ItemHandle;

        //@{

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
        ItemHandle first() const { return mItems[mItems.first()]; }

        ///
        /// return next handle
        ///
        ItemHandle next( ItemHandle h ) const
        {
            GN_ASSERT( validHandle( h ) );
            size_t n = mItems.next( h->id );
            return n ? mItems[n] : 0;
        }

        ///
        /// name must be unique.
        ///
        ItemHandle add( const StrA & name )
        {
            if( mNames.end() != mNames.find( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.cptr() );
                return 0;
            }

            size_t id = mItems.newItem();
            if( 0 == id ) return 0;

            AutoObjPtr<NamedItem> item( new NamedItem(*this,id,name) );

            mNames.insert( std::make_pair(name,id) );

            return item.detach();
        }

        ///
        /// name must be unique.
        ///
        ItemHandle add( const StrA & name, const T & data )
        {
            if( mNames.end() != mNames.find( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "name '%s' is not unique.", name.cptr() );
                return 0;
            }

            size_t id = mItems.newItem();
            if( 0 == id ) return 0;

            AutoObjPtr<NamedItem> item( new NamedItem(*this,id,name,data) );

            mNames.insert( std::make_pair(name,id) );

            return item.detach();
        }

        void remove( ItemHandle h )
        {
            if( !validHandle( h ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid handle: %d.", h );
                return;
            }

            NamedItem * item = mItems[h->id];

            mNames.erase( item->name );

            mItems.remove( item->id );

            delete item;
        }

        void remove( const StrA & name )
        {
            if( !validName( name ) )
            {
                GN_ERROR(getLogger("GN.base.NamedHandleManager"))( "invalid name: %s.", name.cptr() );
                return;
            }

            size_t id = mNames[name];

            NamedItem * item = mItems[id];

            mNames.erase( name );

            mItems.remove( id );

            delete item;
        }

        bool validHandle( ItemHandle h ) const
        {
            if( 0 == h ) return 0;
            return this == &h->mgr && mItems.validHandle( h->id ) && mItems[h->id] == h;
        }

        bool validName( const StrA & name ) const
        {
            return mNames.end() != mNames.find( name );
        }

        T & get( ItemHandle h ) const
        {
            GN_ASSERT( validHandle( h ) );
            return h->data;
        }

        T & get( const StrA & name ) const
        {
            GN_ASSERT( validName(name) );
            NameMap::const_iterator i = mNames.find( name );
            return mItems[mItems.get( i->second )]->data;
        }

        T & operator[]( ItemHandle h ) const { return get(h); }

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
        EntityManager    & manager;
        const EntityTypeId type;
        const StrA         name;

    protected:

        //@{
        Entity( EntityManager & m, EntityTypeId t, const StrA & n )
            : manager(m)
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

        EntityT( EntityManager & m, EntityTypeId t, const StrA & n )
            : Entity( m, t, n )
        {}

        EntityT( EntityManager & m, EntityTypeId t, const StrA & n, const T & d )
            : Entity( m, t, n ), data(d)
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
        EntityT<T> * newEntity( EntityTypeId type, const T & data, const StrA & name );

        ///
        /// delete specific entity
        ///
        void eraseEntityByName( const StrA & name );

        Entity * getEntityByName( const StrA & name ) const;

        // iteration
        Entity * getFirst() const;
        Entity * getFirst( Entity * ) const;
        Entity * getNext( Entity * ) const;
        Entity * getNextWithSameType( Entity * ) const;

        //@}

    private:

        NamedHandleManager<Entity*> mEntities;
    };

    //@{

    ///
    /// convert entity to the object that it represents.
    ///
    template< class T>
    T entity2Object( const Entity * );

    ///
    /// delete specific entity
    ///
    void eraseEntity( Entity * );

    //@}
}}

#include "entity.inl"

// *****************************************************************************
//                           End of entity.h
// *****************************************************************************
#endif // __GN_ENGINE_ENTITY_H__
