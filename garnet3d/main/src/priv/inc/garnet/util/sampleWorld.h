#ifndef __GN_UTIL_SAMPLEWORLD_H__
#define __GN_UTIL_SAMPLEWORLD_H__
// *****************************************************************************
/// \file
/// \brief   Sample world
/// \author  chen@@chenli-homepc (2009.9.6)
// *****************************************************************************

#include <garnet/GNengine.h>

namespace GN { namespace util
{
    ///
    /// A simple world descriptor for the sample applications
    ///
    struct SampleWorldDesc : public NoCopy
    {
        //@{
        struct SpatialDesc
        {
            StrA        parent;
            Vector3f    position;
            Quaternionf orientation;
            Boxf        bbox; // bounding box of this spatial node itself (children are not included)
        };

        struct EntityDesc
        {
            SpatialDesc     spatial;
            DynaArray<StrA> models;
        };

        StringMap<char,gfx::MeshResourceDesc>  meshes;
        DynaArray<AutoRef<Blob> >              meshdata;
        StringMap<char,gfx::ModelResourceDesc> models;
        StringMap<char,EntityDesc>             entities;
        Boxf                                   bbox;     ///< bounding box of the whole world.
        //@}

        ///
        /// clear the description
        ///
        void clear();

        ///
        /// load world description from external file
        ///
        bool loadFromFile( const char * filename );

        ///
        /// Save the world to specific directory
        ///
        bool saveToFile( const char * filename );
    };

    class SampleSpacialEntity : public engine::Entity
    {
        engine::SpacialComponent * mComp;

    public:

        SampleSpacialEntity() : mComp( new engine::SpacialComponent() )
        {
            this->setComponent( mComp );
        }

        engine::SpacialComponent * spacial() const { return mComp; }
    };

    class SampleVisualEntity : public SampleSpacialEntity
    {
        engine::VisualComponent * mComp;

    public:

        SampleVisualEntity() : mComp( new engine::VisualComponent() )
        {
            this->setComponent( mComp );
        }

        engine::VisualComponent * visual() const { return mComp; }
    };


    ///
    /// Virtual world used in sample application
    ///
    class SampleWorld
    {
        engine::Entity                * mRoot;
        StringMap<char,engine::Entity*> mEntities; // entities (not including root)

    public:

        //@{

                         SampleWorld();
                        ~SampleWorld();
        void             clear();
        bool             createEntites( const SampleWorldDesc & desc );
        engine::Entity * getRootEntity() const { return mRoot; }
        void             draw( const Matrix44f & proj, const Matrix44f & view ) const; //< Draw all entities in the world.

        //@}
    };

    /// Load models from file into visual component
    bool loadModelsFromFile( engine::VisualComponent & comp, const char * filename );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_SAMPLEWORLD_H__
