#ifndef __GN_GFX_MESHRESOURCE_H__
#define __GN_GFX_MESHRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Mesh resource implementation clss
/// \author  chenli@@REDMOND (2009.8.26)
// *****************************************************************************

#include "gpures.h"

namespace GN { namespace gfx
{
    ///
    /// Model resource implementation class
    ///
    class ModelResource::Impl : public StdClass
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
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        void              setTexture( const StrA & effectParameterName, GpuResourceHandle );
        GpuResourceHandle getTexture( const StrA & effectParameterName ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        class TextureChange
        {
        };

        struct TextureItem
        {
            GpuResourceHandle handle;
            TextureChange   * change;
        };

        struct UniformItem
        {
            GpuResourceHandle handle;
        };

        struct EffectItem
        {
            GpuResourceHandle handle;
        };

        struct MeshItem
        {
            GpuResourceHandle handle;
        };

        struct RenderPass
        {
            GpuContext gc;
        };

        ModelResource                        & mOwner;
        NamedHandleManager<TextureItem,UInt32> mTextures;
        NamedHandleManager<UniformItem,UInt32> mUniforms;
        EffectItem                             mEffect;
        MeshItem                               mMesh;
        DynaArray<RenderPass>                  mPasses;

        // ********************************
        // private functions
        // ********************************
    private:

    };
}};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MESHRESOURCE_H__
