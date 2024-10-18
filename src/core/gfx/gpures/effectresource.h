#ifndef __GN_GFX_EFFECTRESOURCE_H__
#define __GN_GFX_EFFECTRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Effect resource implementation class
/// \author  chenli@@REDMOND (2009.8.27)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN {
namespace gfx {
///
/// Effect resource implementation class
///
class EffectResource::Impl : public SlotBase {
    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    Impl(EffectResource & owner): mOwner(owner) {}
    virtual ~Impl() { clear(); }
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
    bool reset(const EffectResourceDesc * desc);

    uint32_t numPasses() const { return (uint32_t) mPasses.size(); }

    uint32_t                  numTextures() const { return (uint32_t) mTextures.size(); }
    uint32_t                  findTexture(const char * name) const;
    const TextureProperties & textureProperties(uint32_t i) const { return mTextures[i]; }

    uint32_t                  numUniforms() const { return (uint32_t) mUniforms.size(); }
    uint32_t                  findUniform(const char * name) const;
    const UniformProperties & uniformProperties(uint32_t i) const { return mUniforms[i]; }

    uint32_t                    numAttributes() const { return (uint32_t) mAttributes.size(); }
    uint32_t                    findAttribute(const char * name) const;
    const AttributeProperties & attributeProperties(uint32_t i) const { return mAttributes[i]; }

    const EffectResourceDesc::EffectRenderStateDesc & renderStates(uint32_t pass) const {
        GN_ASSERT(pass < mPasses.size());
        return mPasses[pass].renderstates;
    }

    void applyToContext(uint32_t pass, GpuContext & gc) const;

    // ********************************
    // private types
    // ********************************
private:
    struct GpuProgramItem {
        StrA                name;
        AutoRef<GpuProgram> prog;
    };

    struct RenderPass {
        /// Index of GPU program used in this pass
        uint32_t gpuProgramIndex;

        /// render states
        EffectResourceDesc::EffectRenderStateDesc renderstates;
    };

    // ********************************
    // private variables
    // ********************************
private:
    EffectResource &               mOwner;
    DynaArray<GpuProgramItem>      mPrograms;
    DynaArray<RenderPass>          mPasses;
    DynaArray<TextureProperties>   mTextures;
    DynaArray<UniformProperties>   mUniforms;
    DynaArray<AttributeProperties> mAttributes;

    // ********************************
    // private functions
    // ********************************
private:
    GpuResourceDatabase & getGdb() const { return mOwner.getGdb(); }
    const char *          getEffectName() const { return mOwner.getGdb().getResourceName(&mOwner)->rawptr(); }

    bool init(const EffectResourceDesc & desc);
    void clear();

    bool initGpuPrograms(const EffectResourceDesc & effectDesc);

    bool initGpuProgram(const GN::gfx::EffectResourceDesc & effectDesc, const StrA & programName,
                        const GN::gfx::EffectResourceDesc::EffectGpuProgramDesc & programDesc);

    bool initTechniques(const EffectResourceDesc & effectDesc);

    bool initTech(const EffectResourceDesc & effectDesc, const StrA & techName, const EffectResourceDesc::EffectTechniqueDesc & techDesc);

    bool initTextures(const EffectResourceDesc & effectDesc);

    bool initUniforms(const EffectResourceDesc & effectDesc);

    bool initAttributes(const EffectResourceDesc & effectDesc);

    uint32_t findGpuProgram(const EffectResourceDesc & passDesc, const StrA & programName);
};

///
/// register effect resource factory to database
///
bool registerEffectResourceFactory(GpuResourceDatabase &);

} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_EFFECTRESOURCE_H__
