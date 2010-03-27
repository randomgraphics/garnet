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
        virtual ~Impl() { Clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        bool                        Reset( const ModelResourceDesc * desc );

        AutoRef<ModelResource>      MakeClone( const char * nameOfTheClone ) const;

        bool                        SetTextureResource( const char * effectParameterName, GpuResource * );
        AutoRef<TextureResource>    GetTextureResource( const char * effectParameterName ) const;

        bool                        SetUniformResource( const char * effectParameterName, GpuResource * );
        AutoRef<UniformResource>    GetUniformResource( const char * effectParameterName ) const;

        bool                        SetMeshResource( GpuResource * mesh, const MeshResourceSubset * subset );
        AutoRef<MeshResource>       GetMeshResource( MeshResourceSubset * subset ) const;

        bool                        SetEffectResource( GpuResource * resource );
        AutoRef<EffectResource>     GetEffectResource() const { return mEffect.resource; }

        void                        Draw() const;

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

            void onTextureChange( TextureResource & r );

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

            void onUniformChange( UniformResource & r );

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
            MeshResourceSubset    subset;
        };

        ///
        /// comments...
        ///
        struct RenderPass
        {
            mutable GpuContext                        gc;
            EffectResourceDesc::EffectRenderStateDesc renderstates;
        };

        // ********************************
        // private variables
        // ********************************
    private:

        ModelResource             & mOwner;
        EffectItem                  mEffect;
        MeshItem                    mMesh;
        DynaArray<RenderPass>       mPasses;
        DynaArray<TextureItem>      mTextures;
        DynaArray<UniformItem>      mUniforms;

        mutable StringMap<char,AutoRef<UniformResource> > mDummyUniforms;

        // ********************************
        // private functions
        // ********************************
    private:

        GpuResourceDatabase & GetGdb() const { return mOwner.GetGdb(); }
        const char *          modelName() const { return mOwner.GetGdb().GetResourceName( &mOwner ); }

        bool fromDesc( const ModelResourceDesc & desc );
        void Clear();

        void copyFrom( const Impl & other );

        void onEffectChanged( EffectResource & );
        void onMeshChanged( MeshResource & );
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
