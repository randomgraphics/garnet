#ifndef __GN_GFX_EFFECTRESOURCE_H__
#define __GN_GFX_EFFECTRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Effect resource implementation class
/// \author  chenli@@REDMOND (2009.8.27)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    ///
    /// Effect resource implementation class
    ///
    class EffectResource::Impl : public SlotBase
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Impl( EffectResource & owner ) : mOwner(owner) {}
        virtual ~Impl() { Clear(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        bool                          Reset( const EffectResourceDesc * desc );

        size_t                        getNumPasses() const { return mPasses.Size(); }

        size_t                        getNumTextures() const { return mTextures.Size(); }
        size_t                        findTexture( const char * name ) const;
        const TextureProperties     & getTextureProperties( size_t i ) const { return mTextures[i]; }

        size_t                        getNumUniforms() const { return mUniforms.Size(); }
        size_t                        findUniform( const char * name ) const;
        const UniformProperties     & getUniformProperties( size_t i ) const { return mUniforms[i]; }

        const EffectResourceDesc::EffectRenderStateDesc &
                                      getRenderStates( size_t pass ) const { GN_ASSERT( pass < mPasses.Size() ); return mPasses[pass].renderstates; }

        void                          applyToContext( size_t pass, GpuContext & gc ) const;

        // ********************************
        // private types
        // ********************************
    private:

        struct GpuProgramItem
        {
            StrA                name;
            AutoRef<GpuProgram> prog;
        };

        struct RenderPass
        {
            /// Index of GPU program used in this pass
            size_t                                    gpuProgramIndex;

            /// render states
            EffectResourceDesc::EffectRenderStateDesc renderstates;
        };

        // ********************************
        // private variables
        // ********************************
    private:

        EffectResource                & mOwner;
        DynaArray<GpuProgramItem>       mPrograms;
        DynaArray<RenderPass>           mPasses;
        DynaArray<TextureProperties>    mTextures;
        DynaArray<UniformProperties>    mUniforms;

        // ********************************
        // private functions
        // ********************************
    private:

        GpuResourceDatabase & database() const { return mOwner.database(); }
        const char *          effectName() const { return mOwner.database().getResourceName( &mOwner ); }

        bool Init( const EffectResourceDesc & desc );
        void Clear();

        bool initGpuPrograms( const EffectResourceDesc & effectDesc );

        bool initTechniques( const EffectResourceDesc & effectDesc );

        bool initTech(
            const EffectResourceDesc                      & effectDesc,
            const StrA                                    & techName,
            const EffectResourceDesc::EffectTechniqueDesc & techDesc );

        bool initTextures( const EffectResourceDesc & effectDesc );

        bool initUniforms( const EffectResourceDesc & effectDesc );

        size_t findGpuProgram( const StrA & shaderName ) const;
    };

    ///
    /// register effect resource factory to database
    ///
    bool registerEffectResourceFactory( GpuResourceDatabase & );

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_EFFECTRESOURCE_H__
