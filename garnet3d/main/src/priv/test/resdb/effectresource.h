#ifndef __GN_GFX_EFFECTRESOURCE_H__
#define __GN_GFX_EFFECTRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Effect resource implementation class
/// \author  chenli@@REDMOND (2009.8.27)
// *****************************************************************************

#include "gpures.h"

namespace GN { namespace gfx
{

    ///
    /// Effect resource implementation class
    ///
    class EffectResource::Impl : public StdClass, public SlotBase
    {
        GN_DECLARE_STDCLASS( Impl, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Impl( EffectResource & owner ) : mOwner(owner) { clear(); }
        virtual ~Impl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const EffectResourceDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        size_t                    getNumPasses() const { return mPasses.size(); }

        size_t                    getNumTextures() const { return mTextures.size(); }
        size_t                    findTexture( const char * name ) const;
        const TextureProperties & getTextureProperties( size_t i ) const { return mTextures[i]; }

        size_t                    getNumUniforms() const { return mUniforms.size(); }
        size_t                    findUniform( const char * name ) const;
        const UniformProperties & getUniformProperties( size_t i ) const { return mUniforms[i]; }

        void                      applyToContext( size_t pass, GpuContext & gc ) const;

        // ********************************
        // private types
        // ********************************
    private:

        struct GpuProgramProperties
        {
            AutoRef<GpuProgram> gp;
        };

        struct RenderPass
        {
            // TODO: define per-pass render states

            /// Index of GPU program used in this pass
            size_t gpuProgramIndex;
        };

        // ********************************
        // private variables
        // ********************************
    private:

        EffectResource                & mOwner;
        DynaArray<TextureProperties>    mTextures;
        DynaArray<UniformProperties>    mUniforms;
        DynaArray<GpuProgramProperties> mPrograms;
        DynaArray<RenderPass>           mPasses;

        // ********************************
        // private functions
        // ********************************
    private:

        GpuResourceDatabase & database() const { return mOwner.database(); }
        const char *          myname() const { return mOwner.database().getResourceName( mOwner.handle() ); }

    };

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_EFFECTRESOURCE_H__
