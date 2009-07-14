#ifndef __GN_SCENE_RESMGR_H__
#define __GN_SCENE_RESMGR_H__
// *****************************************************************************
/// \file
/// \brief   Resource manager used in scene model
/// \author  chenli@@REDMOND (2009.7.13)
// *****************************************************************************

namespace GN { namespace scene
{
    struct ModelDesc
    {
        struct TextureDesc
        {
            StrA             brief;
            bool             loadFromExternalFile;
            StrA             filename;
            gfx::TextureDesc desc;
        };

        struct UniformDesc
        {
            StrA             brief;
            size_t           size;
            DynaArray<UInt8> value;
        };

        struct EffectDesc
        {
            StrA                  filename;        // effect file name
            std::map<StrA,size_t> textureBindings; // bind texture (value) to effect parameter (key)
            std::map<StrA,size_t> uniformBindings; // bind uniform (value) to effect parameter (key)
        };

        struct MeshDesc
        {
            StrA          brief;
            gfx::MeshDesc desc;
        };

        struct SubsetDesc
        {
            StrA                  brief;

            size_t                effect;
            size_t                mesh;

            // vertices/indices range (0,0,0,0) means the whole mesh
            size_t                startvtx;
            size_t                numvtx;
            size_t                startidx;
            size_t                numidx;

            /// ctor
            SubsetDesc()
                : startvtx(0)
                , numvtx(0)
                , startidx(0)
                , numidx(0)
            {
            }
        };

        DynaArray<TextureDesc> textures;
        DynaArray<UniformDesc> uniforms;
        DynaArray<MeshDesc>    meshes;
        DynaArray<EffectDesc>  effects;
        DynaArray<SubsetDesc>  subsets;
    };

    class EffectResource
    {
    };

    class ModelResource
    {
    };

    typedef UInt32 ResourceHandle;

    template<typename RES, typename DESC, typename HANDLE>
    struct ResourceMapperTempl
    {
        HANDLE       createFromFile( const StrA & filename );
        HANDLE       createFromDesc( const StrA & name, const DESC & desc );
        void         deleteResource( HANDLE );
        void         clear(); // delete all resources
        const StrA & getResourceName( HANDLE );
        HANDLE       getResourceHandle( const StrA & name );
        RES        & getResource( HANDLE );
    };

    struct ResourceMapper
    {
        ResourceMapperTempl<gfx::Texture*,gfx::TextureDesc,ResourceHandle> textures;
        ResourceMapperTempl<gfx::Uniform*,UniformDesc,ResourceHandle>      uniforms;
        ResourceMapperTempl<gfx::Mesh,gfx::MeshDesc,ResourceHandle>        meshes;
        ResourceMapperTempl<EffectResource,gfx::EffectDesc,ResourceHandle> effects;
        ResourceMapperTempl<ModelResource,ModelDesc,ResourceHandle>        models;
    };

    struct ResourceClass
    {
        void * creator(...);
        void   deletor(...);
    };

    struct ResourceDataBase
    {
        ResourceHandle addNewResource( const StrA & name, ResourceClass class );
        void           activateResource( ResourceHandle
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_RESMGR_H__
