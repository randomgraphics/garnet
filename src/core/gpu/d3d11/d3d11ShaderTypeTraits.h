#ifndef __GN_GPU_D3D11_D3D11SHADERTYPETRAITS_H__
#define __GN_GPU_D3D11_D3D11SHADERTYPETRAITS_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 Shader type traits
/// \author  chenli@@REDMOND (2010.9.29)
// *****************************************************************************

namespace GN {
namespace gfx {
template<int SHADER_STAGE>
struct D3D11ShaderTypeTraits {};

template<>
struct D3D11ShaderTypeTraits<ShaderStage::VS> {
    //@{

    typedef ID3D11VertexShader ShaderClass;

    static const char * getLatestProfile(ID3D11Device & dev, GpuProgramLanguage lang) {
        D3D_FEATURE_LEVEL featureLevel = dev.GetFeatureLevel();
        switch (featureLevel) {
        case D3D_FEATURE_LEVEL_9_1:
            return "vs_2_0";
        case D3D_FEATURE_LEVEL_9_2:
            return "vs_2_0";
        case D3D_FEATURE_LEVEL_9_3:
            return "vs_2_a";
        case D3D_FEATURE_LEVEL_10_0:
            return "vs_4_0";
        case D3D_FEATURE_LEVEL_10_1:
            return "vs_4_1";
        case D3D_FEATURE_LEVEL_11_0:
            return "vs_5_0";
        default:
            return "unknown";
        }
    }

    static ID3D11VertexShader * create(ID3D11Device & dev, const char * source, size_t len, uint32 flags, const char * entry, const char * profile,
                                       ID3DBlob ** signature) {
        AutoComPtr<ID3DBlob> bin = AutoComPtr<ID3DBlob>::sAttach(GN::d3d11::compileShader(profile, source, len, flags, entry));
        if (!bin) return NULL;

        ID3D11VertexShader * shader;
        GN_DX_CHECK_RETURN(dev.CreateVertexShader(bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader), 0);

        // success
        if (signature) *signature = bin.detach();
        return shader;
    }

    static void bindShader(ID3D11DeviceContext & cxt, ID3D11DeviceChild * shader) { cxt.VSSetShader((ShaderClass *) shader, NULL, 0); }

    static void bindConstBuffers(ID3D11DeviceContext & cxt, uint32 startSlot, uint32 numBuffers, ID3D11Buffer * const * constBuffers) {
        cxt.VSSetConstantBuffers(startSlot, numBuffers, constBuffers);
    }

    //@}
};

template<>
struct D3D11ShaderTypeTraits<ShaderStage::PS> {
    //@{

    typedef ID3D11PixelShader ShaderClass;

    static const char * getLatestProfile(ID3D11Device & dev, GpuProgramLanguage lang) {
        D3D_FEATURE_LEVEL featureLevel = dev.GetFeatureLevel();
        switch (featureLevel) {
        case D3D_FEATURE_LEVEL_9_1:
            return "ps_2_0";
        case D3D_FEATURE_LEVEL_9_2:
            return "ps_2_0";
        case D3D_FEATURE_LEVEL_9_3:
            return "ps_2_a";
        case D3D_FEATURE_LEVEL_10_0:
            return "ps_4_0";
        case D3D_FEATURE_LEVEL_10_1:
            return "ps_4_1";
        case D3D_FEATURE_LEVEL_11_0:
            return "ps_5_0";
        default:
            return "unknown";
        }
    }

    static ID3D11PixelShader * create(ID3D11Device & dev, const char * source, size_t len, uint32 flags, const char * entry, const char * profile,
                                      ID3DBlob ** signature) {
        AutoComPtr<ID3DBlob> bin = AutoComPtr<ID3DBlob>::sAttach(GN::d3d11::compileShader(profile, source, len, flags, entry));
        if (!bin) return NULL;

        ID3D11PixelShader * shader;
        GN_DX_CHECK_RETURN(dev.CreatePixelShader(bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader), 0);

        // success
        if (signature) *signature = bin.detach();
        return shader;
    }

    static void bindShader(ID3D11DeviceContext & cxt, ID3D11DeviceChild * shader) { cxt.PSSetShader((ShaderClass *) shader, NULL, 0); }

    static void bindConstBuffers(ID3D11DeviceContext & cxt, uint32 startSlot, uint32 numBuffers, ID3D11Buffer * const * constBuffers) {
        cxt.PSSetConstantBuffers(startSlot, numBuffers, constBuffers);
    }

    //@}
};

template<>
struct D3D11ShaderTypeTraits<ShaderStage::GS> {
    //@{
    typedef ID3D11GeometryShader ShaderClass;

    static const char * getLatestProfile(ID3D11Device & dev, GpuProgramLanguage lang) {
        D3D_FEATURE_LEVEL featureLevel = dev.GetFeatureLevel();
        switch (featureLevel) {
        case D3D_FEATURE_LEVEL_10_0:
            return "gs_4_0";
        case D3D_FEATURE_LEVEL_10_1:
            return "gs_4_1";
        case D3D_FEATURE_LEVEL_11_0:
            return "gs_5_0";
        default:
            return "unknown";
        }
    }

    static ID3D11GeometryShader * create(ID3D11Device & dev, const char * source, size_t len, uint32 flags, const char * entry, const char * profile,
                                         ID3DBlob ** signature) {
        AutoComPtr<ID3DBlob> bin = AutoComPtr<ID3DBlob>::sAttach(GN::d3d11::compileShader(profile, source, len, flags, entry));
        if (!bin) return NULL;

        ID3D11GeometryShader * shader;
        GN_DX_CHECK_RETURN(dev.CreateGeometryShader(bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader), 0);

        // success
        if (signature) *signature = bin.detach();
        return shader;
    }

    static void bindShader(ID3D11DeviceContext & cxt, ID3D11DeviceChild * shader) { cxt.GSSetShader((ShaderClass *) shader, NULL, 0); }

    static void bindConstBuffers(ID3D11DeviceContext & cxt, uint32 startSlot, uint32 numBuffers, ID3D11Buffer * const * constBuffers) {
        cxt.GSSetConstantBuffers(startSlot, numBuffers, constBuffers);
    }
    //@}
};

template<>
struct D3D11ShaderTypeTraits<ShaderStage::HS> {
    //@{
    typedef ID3D11HullShader ShaderClass;

    static const char * getLatestProfile(ID3D11Device & dev, GpuProgramLanguage lang) {
        D3D_FEATURE_LEVEL featureLevel = dev.GetFeatureLevel();
        switch (featureLevel) {
        case D3D_FEATURE_LEVEL_11_0:
            return "hs_5_0";
        default:
            return "unknown";
        }
    }

    static ID3D11HullShader * create(ID3D11Device & dev, const char * source, size_t len, uint32 flags, const char * entry, const char * profile,
                                     ID3DBlob ** signature) {
        AutoComPtr<ID3DBlob> bin = AutoComPtr<ID3DBlob>::sAttach(GN::d3d11::compileShader(profile, source, len, flags, entry));
        if (!bin) return NULL;

        ID3D11HullShader * shader;
        GN_DX_CHECK_RETURN(dev.CreateHullShader(bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader), 0);

        // success
        if (signature) *signature = bin.detach();
        return shader;
    }

    static void bindShader(ID3D11DeviceContext & cxt, ID3D11DeviceChild * shader) { cxt.HSSetShader((ShaderClass *) shader, NULL, 0); }

    static void bindConstBuffers(ID3D11DeviceContext & cxt, uint32 startSlot, uint32 numBuffers, ID3D11Buffer * const * constBuffers) {
        cxt.HSSetConstantBuffers(startSlot, numBuffers, constBuffers);
    }
    //@}
};

template<>
struct D3D11ShaderTypeTraits<ShaderStage::DS> {
    //@{
    typedef ID3D11DomainShader ShaderClass;

    static const char * getLatestProfile(ID3D11Device & dev, GpuProgramLanguage lang) {
        D3D_FEATURE_LEVEL featureLevel = dev.GetFeatureLevel();
        switch (featureLevel) {
        case D3D_FEATURE_LEVEL_11_0:
            return "ds_5_0";
        default:
            return "unknown";
        }
    }

    static ID3D11DomainShader * create(ID3D11Device & dev, const char * source, size_t len, uint32 flags, const char * entry, const char * profile,
                                       ID3DBlob ** signature) {
        AutoComPtr<ID3DBlob> bin = AutoComPtr<ID3DBlob>::sAttach(GN::d3d11::compileShader(profile, source, len, flags, entry));
        if (!bin) return NULL;

        ID3D11DomainShader * shader;
        GN_DX_CHECK_RETURN(dev.CreateDomainShader(bin->GetBufferPointer(), bin->GetBufferSize(), NULL, &shader), 0);

        // success
        if (signature) *signature = bin.detach();
        return shader;
    }

    static void bindShader(ID3D11DeviceContext & cxt, ID3D11DeviceChild * shader) { cxt.DSSetShader((ShaderClass *) shader, NULL, 0); }

    static void bindConstBuffers(ID3D11DeviceContext & cxt, uint32 startSlot, uint32 numBuffers, ID3D11Buffer * const * constBuffers) {
        cxt.DSSetConstantBuffers(startSlot, numBuffers, constBuffers);
    }
    //@}
};

static void (*D3D11BindShader[])(ID3D11DeviceContext & cxt, ID3D11DeviceChild * shader) = {
    D3D11ShaderTypeTraits<0>::bindShader, D3D11ShaderTypeTraits<1>::bindShader, D3D11ShaderTypeTraits<2>::bindShader,
    D3D11ShaderTypeTraits<3>::bindShader, D3D11ShaderTypeTraits<4>::bindShader,
};
GN_CASSERT(GN_ARRAY_COUNT(D3D11BindShader) == ShaderStage::COUNT);

static void (*D3D11BindConstBuffers[])(ID3D11DeviceContext & cxt, uint32 startSlot, uint32 numBuffers, ID3D11Buffer * const * constBuffers) = {
    D3D11ShaderTypeTraits<0>::bindConstBuffers, D3D11ShaderTypeTraits<1>::bindConstBuffers, D3D11ShaderTypeTraits<2>::bindConstBuffers,
    D3D11ShaderTypeTraits<3>::bindConstBuffers, D3D11ShaderTypeTraits<4>::bindConstBuffers,
};
GN_CASSERT(GN_ARRAY_COUNT(D3D11BindConstBuffers) == ShaderStage::COUNT);
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPU_D3D11_D3D11SHADERTYPETRAITS_H__
