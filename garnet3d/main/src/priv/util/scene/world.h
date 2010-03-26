#ifndef __GN_SCENE_WORLD_H__
#define __GN_SCENE_WORLD_H__
// *****************************************************************************
/// \file
/// \brief   World implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

namespace GN { namespace util
{
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

        Impl( World & owner, gfx::GpuResourceDatabase & gdb )
            : mOwner(owner)
            , mGpuResourceDatabase( gdb )
            , mVisualGraph( gdb )
        {
        }

        ~Impl() { Reset(); }

        //@}

        //@{

        World & owner() const { return mOwner; }

        //@}

        //@{

        gfx::GpuResourceDatabase & gdb() const { return mGpuResourceDatabase; }
        SpatialGraph             & spatialGraph() const { return mSpatialGraph; }
        VisualGraph              & visualGraph() const { return mVisualGraph; }

        void          Reset();
        Entity      * createEntity( const char * name = NULL );
        void          deleteEntity( const char * name );
        void          deleteEntity( int id );
        void          deleteEntity( Entity * entity );
        void          deleteAllEntities();
        Entity      * findEntity( const char * name );
        Entity      * findEntity( int id );
        const char  * getEntityName( int id ) const;

        //@}

        // *****************************
        // private types
        // *****************************

    private:

        typedef NamedHandleManager<Entity*,int> EntityMap;

        // *****************************
        // private data
        // *****************************

    private:

        //@{

        World                    & mOwner;
        gfx::GpuResourceDatabase & mGpuResourceDatabase;
        EntityMap                  mEntities;
        mutable SpatialGraph       mSpatialGraph;
        mutable VisualGraph        mVisualGraph;

        //@}

        // *****************************
        // private methods
        // *****************************

    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_WORLD_H__
