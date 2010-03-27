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

        size_t                        GetNumPasses() const { return mPasses.Size(); }

        size_t                        GetNumTextures() const { return mTextures.Size(); }
        size_t                        FindTexture( const char * name ) const;
        const TextureProperties     & GetTextureProperties( size_t i ) const { return mTextures[i]; }

        size_t                        GetNumUniforms() const { return mUniforms.Size(); }
        size_t                        FindUniform( const char * name ) const;
        const UniformProperties     & GetUniformProperties( size_t i ) const { return mUniforms[i]; }

        const EffectResourceDesc::EffectRenderStateDesc &
                                      GetRenderStates( size_t pass ) const { GN_ASSERT( pass < mPasses.Size() ); return mPasses[pass].renderstates; }

        void                          ApplyToContext( size_t pass, GpuContext & gc ) const;

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

        GpuResourceDatabase & GetGdb() const { return mOwner.GetGdb(); }
        const char *          effectName() const { return mOwner.GetGdb().GetResourceName( &mOwner ); }

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
