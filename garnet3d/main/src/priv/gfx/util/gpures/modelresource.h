#ifndef __GN_GFX_MODELRESOURCE_H__
#define __GN_GFX_MODELRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Model resource implementation clss
/// \author  chenli@@REDMOND (2009.8.26)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    ///
    /// Model resource implementation class
    ///
    class ModelResource::Impl : public SlotBase
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Impl( ModelResource & owner ) : mOwner(owner) {}
        virtual ~Impl() { clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        bool                        reset( const ModelResourceDesc * desc );

        void                        setTexture( const char * effectParameterName, GpuResource * );
        AutoRef<TextureResource>    getTexture( const char * effectParameterName ) const;

        void                        setUniform( const char * effectParameterName, GpuResource * );
        AutoRef<UniformResource>    getUniform( const char * effectParameterName ) const;

        void                        setMesh( GpuResource * mesh, const MeshResourceSubset * subset );

        void                        draw() const;

        // ********************************
        // private classes
        // ********************************
    private:

        ///
        /// comments...
        ///
        class TextureItem : public SlotBase
        {
        public:

            TextureItem();
            ~TextureItem();

            void setResource( Impl & owner, size_t effectParameterIndex, TextureResource * );

            const AutoRef<TextureResource> & getResource() const { return mResource; }

        private:

            Impl                   * mOwner;
            size_t                   mEffectParameterIndex;
            AutoRef<TextureResource> mResource;

        private:

            void onTextureChange( GpuResource & r );

            void updateContext( Texture * );
        };

        ///
        /// comments...
        ///
        class UniformItem : public SlotBase
        {
        public:

            UniformItem();
            ~UniformItem();

            void setResource( Impl & owner, size_t effectParameterIndex, UniformResource * );

            const AutoRef<UniformResource> & getResource() const { return mResource; }

        private:

            Impl                   * mOwner;
            size_t                   mEffectParameterIndex;
            AutoRef<UniformResource> mResource;

        private:

            void onUniformChange( GpuResource & r );

            void updateContext( Uniform * );
        };

        ///
        /// comments...
        ///
        struct EffectItem
        {
            AutoRef<EffectResource> resource;
        };

        ///
        /// comments...
        ///
        struct MeshItem
        {
            AutoRef<MeshResource> resource;
        };

        ///
        /// comments...
        ///
        struct RenderPass
        {
            mutable GpuContext                        gc;
            EffectResourceDesc::EffectRenderStateDesc rsdesc;
        };

        // ********************************
        // private variables
        // ********************************
    private:

        ModelResource             & mOwner;
        ModelResourceDesc           mDesc;
        EffectItem                  mEffect;
        MeshItem                    mMesh;
        DynaArray<RenderPass>       mPasses;
        DynaArray<TextureItem>      mTextures;
        DynaArray<UniformItem>      mUniforms;

        // ********************************
        // private functions
        // ********************************
    private:

        GpuResourceDatabase & database() const { return mOwner.database(); }
        const char *          modelName() const { return mOwner.database().getResourceName( &mOwner ); }

        bool init( const ModelResourceDesc & desc );
        void clear();

        void onEffectChanged( GpuResource & );
        void onMeshChanged( GpuResource & );
    };

    ///
    /// register model resource factory to database
    ///
    bool registerModelResourceFactory( GpuResourceDatabase & );

}};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MODELRESOURCE_H__
