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
    GN_API Texture * loadTextureFromFile( Gpu & gpu, const char * filename );

    ///
    /// A collection of models with hierarchy.
    ///
    struct GN_API ModelHierarchyDesc : public NoCopy
    {
        //@{
        struct NodeDesc
        {
            StrA            parent;      //< name of the parent node. Empty if there's no parent.
            Vector3f        position;    //< node position in parent's space
            Quaternionf     orientation; //< node orientation in parent's space
            Vector3f        scaling;     //< node scaling in local space
            Boxf            bbox;        //< bounding box of the node itself (children are not considered)
            DynaArray<StrA> models;      //< List of models in the node.
        };

        StringMap<char,MeshResourceDesc>  meshes;
        DynaArray<AutoRef<Blob> >         meshdata;
        StringMap<char,ModelResourceDesc> models;
        StringMap<char,NodeDesc>          nodes;
        Boxf                              bbox; ///< bounding box of the whole hierarchy.
        //@}

        //@{

        void clear()
        {
            meshes.clear();
            meshdata.clear();
            models.clear();
            nodes.clear();
        }

        /// Load models from file. Existing content will be discarded.
        bool loadFromFile( const char * filename );

        bool saveToFile( const char * filename ) const;

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MISC_H__
