#ifndef __GN_GFX_MISC_H__
#define __GN_GFX_MISC_H__
// *****************************************************************************
/// \file
/// \brief   misc. utilities
/// \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// load texture from image file
    ///
    Texture * loadTextureFromFile( Gpu & gpu, const char * filename );

    ///
    /// A collection of models with hierarchy.
    ///
    struct ModelHierarchyDesc : public NoCopy
    {
        //@{
        struct NodeDesc
        {
            StrA            parent;      //< name of the parent node. Empty if there's no parent.
            Vector3f        position;    //< node position in parent's space
            Quaternionf     orientation; //< node orientation in parent's space
            Boxf            bbox;        //< bounding box of the node itself (children are not considered)
            DynaArray<StrA> models;      //< List of models in the node.
        };

        StringMap<char,gfx::MeshResourceDesc>  meshes;
        DynaArray<AutoRef<Blob> >              meshdata;
        StringMap<char,gfx::ModelResourceDesc> models;
        StringMap<char,NodeDesc>               nodes;
        Boxf                                   bbox; ///< bounding box of the whole hierarchy.
        //@}

        //@{

        void clear();

        /// Load models from file. Existing content will be discarded.
        bool loadFromFile( const char * filename );

        bool saveToFile( const char * filename ) const;

        // TODO:
        //  bool LoadFromFile( File & );
        //  bool saveToFile( File & ) const;

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MISC_H__
