#ifndef __GN_GFXD3D11_D3D11SHADER_H__
#define __GN_GFXD3D11_D3D11SHADER_H__
// *****************************************************************************
/// \file
/// \brief   D3D shader classes
/// \author  chenlee (2005.11.26)
// *****************************************************************************

#include "d3d11Resource.h"
#include "../common/basicShader.h"

namespace GN {
namespace gfx {
// *************************************************************************
// Common program interface
// *************************************************************************

class D3D11GpuProgram : public GpuProgram {
public:
    uint64 getUniqueID() const { return m_ID; }

    ///
    /// Get attribute semantic name and semantic index
    ///
    virtual const char * getAttributeSemantic(uint32 attributeIndex, UINT * semanticIndex) const = 0;

    ///
    /// Get vertex input signature.
    ///
    virtual const void * getInputSignature(size_t * pSignatureSize) const = 0;

    ///
    /// apply shader to D3D device
    ///
    virtual void apply() const = 0;

    ///
    /// Apply uniforms to D3D device
    ///
    virtual void applyUniforms(const Uniform * const * uniforms, uint32 count, bool skipDirtyCheck) const = 0;

    ///
    /// apply textures to D3D device
    ///
    virtual void applyTextures(const TextureBinding * bindings, uint32 count, bool skipDirtyCheck) const = 0;

protected:
    D3D11GpuProgram() {
        static uint64 sShaderID = 0;
        m_ID                    = ++sShaderID;
    }

private:
    uint64 m_ID;
};

// *************************************************************************
// HLSL program
// *************************************************************************

/// shader parameter classes
//@{

struct D3D11UniformParameterDesc : public GpuProgramUniformParameterDesc {
    ///
    /// shader specific properties
    ///
    struct ShaderSpecificProperties {
        AutoInitializer<bool, false> used;   ///< are these properties used
        uint32                       cbidx;  ///< const buffer index
        uint32                       offset; ///< uniform offset in bytes in the const buffer.
    };

    ///
    /// shader specific properites for each shader stage
    ///
    ShaderSpecificProperties ssp[ShaderStage::COUNT];

    /// ctor
    D3D11UniformParameterDesc() { name = NULL; }
};

struct D3D11TextureParameterDesc : public GpuProgramTextureParameterDesc {
    ///
    /// shader specific properties
    ///
    struct ShaderSpecificProperties {
        AutoInitializer<bool, false> used;  ///< are these properties used
        UINT                         stage; ///< texture stage that the parameter is binding to
    };

    ShaderSpecificProperties ssp[ShaderStage::COUNT]; ///< shader specific properites for each shader type

    /// ctor
    D3D11TextureParameterDesc() { name = NULL; }
};

struct D3D11AttributeParameterDesc : public GpuProgramAttributeParameterDesc {
    StrA semanticName;
    UINT semanticIndex;

    /// ctor
    D3D11AttributeParameterDesc() { name = NULL; }
};

class D3D11GpuProgramParameterDesc : public GpuProgramParameterDesc, public NoCopy {
    DynaArray<D3D11UniformParameterDesc>   mUniforms;
    DynaArray<D3D11TextureParameterDesc>   mTextures;
    DynaArray<D3D11AttributeParameterDesc> mAttributes;

public:
    /// ctor
    D3D11GpuProgramParameterDesc();

    /// dtor
    ~D3D11GpuProgramParameterDesc();

    /// clear the descriptor
    void clear();

    /// build parameter arrays
    void buildParameterArrays();

    /// find parameter with specific name
    //@{
    const D3D11UniformParameterDesc *   findUniform(const char * name) const;
    D3D11UniformParameterDesc *         findUniform(const char * name);
    const D3D11TextureParameterDesc *   findTexture(const char * name) const;
    D3D11TextureParameterDesc *         findTexture(const char * name);
    const D3D11AttributeParameterDesc * findAttribute(const char * name) const;
    D3D11AttributeParameterDesc *       findAttribute(const char * name);
    //@}

    /// add new parameters
    //@{
    void addUniform(const D3D11UniformParameterDesc & u) { mUniforms.append(u); }
    void addTexture(const D3D11TextureParameterDesc & t) { mTextures.append(t); }
    void addAttribute(const D3D11AttributeParameterDesc & a) { mAttributes.append(a); }
    //@}
};

//@}

///
/// D3D11 HLSL GPU program
///
class D3D11GpuProgramHLSL : public D3D11GpuProgram, public D3D11Resource, public StdClass {
    GN_DECLARE_STDCLASS(D3D11GpuProgramHLSL, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    D3D11GpuProgramHLSL(D3D11Gpu &);
    virtual ~D3D11GpuProgramHLSL() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(const GpuProgramDesc & desc);
    void quit();

private:
    void clear() {}
    //@}

    // ********************************
    // from Gpu
    // ********************************
public:
    virtual const GpuProgramParameterDesc & getParameterDesc() const { return mParamDesc; }

    // ********************************
    // from D3D11GpuProgram
    // ********************************
public:
    virtual const char * getAttributeSemantic(uint32 attributeIndex, UINT * semanticIndex) const;

    virtual const void * getInputSignature(size_t * pSignatureSize) const;

    virtual void apply() const;

    virtual void applyUniforms(const Uniform * const * uniforms, uint32 count, bool skipDirtyCheck) const;

    virtual void applyTextures(const TextureBinding * bindings, uint32 count, bool skipDirtyCheck) const;

    // ********************************
    // private variables
    // ********************************
private:
    /// array of D3D11 constant buffer
    typedef StackArray<AutoComPtr<ID3D11Buffer>, 16> D3D11ConstBufferArray;

    // We'll cast this auto-ptr array to raw pointer array. So they must be the same size.
    GN_CASSERT(sizeof(AutoComPtr<ID3D11Buffer>) == sizeof(ID3D11Buffer *));

    /// array of constant buffer in system memory
    typedef StackArray<DynaArray<uint8>, 16> SysMemConstBufferArray;

    struct ShaderHLSL {
        AutoComPtr<ID3D11DeviceChild>  shader;    ///< shader pointer
        D3D11ConstBufferArray          constBufs; ///< constant buffers
        mutable SysMemConstBufferArray constData; ///< constant data

        void clear() {
            shader.clear();
            constBufs.clear();
            constData.clear();
        }
    };

    D3D11GpuProgramParameterDesc mParamDesc;
    ShaderHLSL                   mShaders[ShaderStage::COUNT];
    AutoComPtr<ID3DBlob>         mInputSignature;

    // ********************************
    // private functions
    // ********************************
private:
    template<int SHADER_STAGE>
    bool initShader(ShaderHLSL & shader, const ShaderCode & code, GpuProgramLanguage targetLanguage, uint32 compileFlags);

    static bool sInitConstBuffers(ID3D11Device & dev, ID3D11ShaderReflection & reflection, D3D11ConstBufferArray & constBufs,
                                  SysMemConstBufferArray & constData);

    static void sUpdateD3D11ConstData(const D3D11UniformParameterDesc & desc, const SysMemUniform & uniform, SysMemConstBufferArray & cbarray,
                                      ShaderStage::Enum shaderStage, bool * dirtyFlags);
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11SHADER_H__
