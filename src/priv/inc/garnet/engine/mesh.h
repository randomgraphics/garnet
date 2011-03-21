#ifndef __GN_ENGINE_MESH_H__
#define __GN_ENGINE_MESH_H__
// *****************************************************************************
/// \file
/// \brief   Mesh entities
/// \author  chen@@chenli-homepc (2011.2.28)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// Static mesh entity
    ///
    class StaticMesh : public Entity
    {
    public:

        /// constructor
        StaticMesh();

        /// destructor
        ~StaticMesh();

        /// get the spacial component
        const SpacialComponent & spacial() const { return mSpacial; }

        /// get the spacial component
        SpacialComponent & spacial() { return mSpacial; }

        /// get the visual component
        const VisualComponent & visual() const { return mVisual; }

        /// get the visual component
        VisualComponent & visual() { return mVisual; }

        /// Load the whole model hierarchy as a single static mesh.
        /// Existing content in the mesh will be discarded.
        bool loadFromModelHierarchy( const gfx::ModelHierarchyDesc & );

        /// Load from file
        bool loadFromFile( const char * filename )
        {
            gfx::ModelHierarchyDesc mhd;
            if( !mhd.loadFromFile( filename ) ) return false;
            return loadFromModelHierarchy( mhd );
        }

    private:

        StackRefCounter<SpacialComponent> mSpacial;
        StackRefCounter<VisualComponent>  mVisual;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_MESH_H__
