#ifndef __GN_GFX_MODELRESOURCE_H__
#define __GN_GFX_MODELRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Model resource implementation clss
/// \author  chenli@@REDMOND (2009.8.26)
// *****************************************************************************

#include "gpures.h"

namespace GN { namespace gfx
{
    ///
    /// Model resource implementation class
    ///
    class ModelResource::Impl : public StdClass, public SlotBase
    {
        GN_DECLARE_STDCLASS( ModelResource::Impl, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Impl( ModelResource & owner ) : mOwner(owner) { clear(); }
        virtual ~Impl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const ModelResourceDesc & desc );
        void quit();
    private:
        void clear();
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        void              setTexture( const char * effectParameterName, GpuResourceHandle );
        GpuResourceHandle getTexture( const char * effectParameterName ) const;

        void              setUniform( const char * effectParameterName, GpuResourceHandle );
        GpuResourceHandle getUniform( const char * effectParameterName ) const;

        void              draw() const;

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

            void setHandle( Impl & owner, size_t effectParameterIndex, GpuResourceHandle texture );

            GpuResourceHandle getHandle() const { return mHandle; }

        private:

            Impl            * mOwner;
            size_t            mEffectParameterIndex;
            GpuResourceHandle mHandle;

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

            void setHandle( Impl & owner, size_t effectParameterIndex, GpuResourceHandle texture );

            GpuResourceHandle getHandle() const { return mHandle; }

        private:

            Impl            * mOwner;
            size_t            mEffectParameterIndex;
            GpuResourceHandle mHandle;

        private:

            void onUniformChange( GpuResource & r );

            void updateContext( Uniform * );
        };

        ///
        /// comments...
        ///
        struct EffectItem
        {
            GpuResourceHandle handle;
        };

        ///
        /// comments...
        ///
        struct MeshItem
        {
            GpuResourceHandle handle;
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
        const char *          modelName() const { return mOwner.database().getResourceName( mOwner.handle() ); }

        void onEffectChanged( GpuResource & );
        void onMeshChanged( GpuResource & );
    };
}};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MODELRESOURCE_H__
