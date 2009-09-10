#ifndef __GN_SCENE_WORLD_H__
#define __GN_SCENE_WORLD_H__
// *****************************************************************************
/// \file
/// \brief   World implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

#include "scene.h"

namespace GN { namespace scene
{
    ///
    /// Entity ID class
    ///
    class EntityID
    {
        union
        {
            SInt32     mI32;
            struct
            {
                UInt32 mIndexPlusOne   : 8;
                UInt32 mInternalHandle : 24;
            };
        };

    public:

        //@{

        enum { MAX_TYPES = 2^8-1 };

        explicit EntityID( UInt32 u32 )
            : mI32( u32 )
        {
        }

        EntityID( size_t managerIndex, UInt32 internalHandle )
        {
            mIndexPlusOne = managerIndex + 1;
            mInternalHandle = internalHandle;
        }

        void  set( UInt32 managerIndex, UInt32 internalHandle )
        {
            mIndexPlusOne = managerIndex + 1;
            mInternalHandle = internalHandle;
        }

        SInt32 i32()            const { return mI32; }
        UInt32 managerIndex()   const { return mIndexPlusOne - 1; }
        UInt32 internalHandle() const { return mInternalHandle; }

        //@}
    };

    ///
    /// World implementation class
    ///
    class World::Impl
    {
        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( World & owner ) : mOwner(owner) {}
        ~Impl() { clear(); }

        //@}

        //@{

        World & owner() const { return mOwner; }

        //@}

        //@{

        void          clear();

        bool          hasEntityFactory( const Guid & type );
        bool          registerEntityFactory( const Guid & type, const char * desc, EntityFactory factory, const void * factoryParameter );
        void          unregisterEntityFactory( const Guid & type );
        EntityFactory getEntityFactory( const Guid & type );

        Entity      * createEntity( const Guid & type, const char * name = NULL );
        void          deleteEntity( const Guid & type, const char * name );
        void          deleteEntity( int id );
        void          deleteEntity( Entity * entity );
        void          deleteAllEntities();
        Entity      * findEntity( const Guid & type, const char * name );
        Entity      * findEntity( int id );

        //@}

        // *****************************
        // private types
        // *****************************

    private:

        typedef NamedHandleManager<Entity*,UInt32> EntityMap;

        struct EntityManager
        {
            Guid          guid;
            StrA          desc;
            size_t        index; // index into manager array
            EntityFactory factory;
            EntityMap     entities;
        };

        typedef StackArray<EntityManager, EntityID::MAX_TYPES> EntityArray;

        // *****************************
        // private data
        // *****************************

    private:

        //@{

        World      & mOwner;
        EntityArray  mManagers;
        SpatialGraph mSpatialGraph;
        VisualGraph  mVisualGraph;

        //@}

        // *****************************
        // private methods
        // *****************************

    private:

        //@{

        static bool sInitSpatialEntity( Entity & entity, const void * parameters );
        static bool sInitVisualEntity( Entity & entity, const void * parameters );
        static bool sInitLightEntity( Entity & entity, const void * parameters );

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_WORLD_H__
