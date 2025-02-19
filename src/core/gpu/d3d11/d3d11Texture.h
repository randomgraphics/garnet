#ifndef __GN_GFXD3D11_D3D11TEXTURE_H__
#define __GN_GFXD3D11_D3D11TEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   D3D 10 texture class
/// \author  chenlee (2006.09.20)
// *****************************************************************************

#include "../common/basicSurface.h"
#include "d3d11Resource.h"

namespace GN {
namespace gfx {
class D3D11Gpu;

///
/// D3D texture
///
class D3D11Texture : public BasicTexture, public D3D11Resource, public StdClass {
    GN_DECLARE_STDCLASS(D3D11Texture, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    D3D11Texture(D3D11Gpu & r): D3D11Resource(r) { clear(); }
    virtual ~D3D11Texture() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init(const TextureDesc & desc);
    void quit();

private:
    void clear() { mTexture = 0; }
    //@}

    // ********************************
    // From Texture
    // ********************************
public:
    virtual void     updateMipmap(uint32_t face, uint32_t level, const Box<uint32_t> * area, uint32_t rowPitch, uint32_t slicePitch, const void * data,
                                  SurfaceUpdateFlag flag);
    virtual void     readMipmap(uint32_t face, uint32_t level, MipmapData & data);
    virtual void     blobWrite(const void *, uint32_t) { GN_UNIMPL(); }
    virtual uint32_t blobRead(void *) {
        GN_UNIMPL();
        return 0;
    }
    virtual void   generateMipmapPyramid() { GN_UNIMPL(); }
    virtual void * getAPIDependentData() const { return mTexture; }

    // ********************************
    // public functions
    // ********************************
public:
    ///
    /// Get the texture resource
    ///
    ID3D11Resource * getD3D11Resource() const { return mTexture; }

    ///
    /// get shader resource view
    ///
    ID3D11ShaderResourceView * getSRView() {
        const TextureDesc & texdesc = getDesc();

        return getSRView(mReadingFormat, 0, texdesc.faces, 0, texdesc.levels, 0, texdesc.depth);
    }

    ///
    /// get shader resource view
    ///
    ID3D11ShaderResourceView * getSRView(DXGI_FORMAT format, uint32_t firstFace, uint32_t numFaces, uint32_t firstMipLevel, uint32_t numLevels,
                                         uint32_t firstSlice, uint32_t numSlices);

    ///
    /// get render target view of specific subresource
    ///
    ID3D11RenderTargetView * getRTView(uint32_t face, uint32_t level, uint32_t slice);

    ///
    /// get render target view of specific subresource
    ///
    ID3D11DepthStencilView * getDSView(uint32_t face, uint32_t level, uint32_t slice);

    // ********************************
    // private variables
    // ********************************
private:
    // mapping subresource index to render target view
    typedef std::unordered_map<D3D11_SHADER_RESOURCE_VIEW_DESC, AutoComPtr<ID3D11ShaderResourceView>, BlobHash<D3D11_SHADER_RESOURCE_VIEW_DESC>,
                               MemoryEqual<D3D11_SHADER_RESOURCE_VIEW_DESC>>
        SRViewMap;
    typedef std::unordered_map<D3D11_RENDER_TARGET_VIEW_DESC, AutoComPtr<ID3D11RenderTargetView>, BlobHash<D3D11_RENDER_TARGET_VIEW_DESC>,
                               MemoryEqual<D3D11_RENDER_TARGET_VIEW_DESC>>
        RTViewMap;
    typedef std::unordered_map<D3D11_DEPTH_STENCIL_VIEW_DESC, AutoComPtr<ID3D11DepthStencilView>, BlobHash<D3D11_DEPTH_STENCIL_VIEW_DESC>,
                               MemoryEqual<D3D11_DEPTH_STENCIL_VIEW_DESC>>
        DSViewMap;

    D3D11_SRV_DIMENSION mDimension;     ///< texture dimension
    DXGI_FORMAT         mTextureFormat; ///< D3D11 texture format
    DXGI_FORMAT         mReadingFormat; ///< format for reading from the texture
    DXGI_FORMAT         mWritingFormat; ///< format for writing to the texture
    ID3D11Resource *    mTexture;       ///< resource instance
    SRViewMap           mSRViews;       ///< shader resource views
    RTViewMap           mRTViews;       ///< render target views
    DSViewMap           mDSViews;       ///< depth stencil views

    // ********************************
    // private functions
    // ********************************
private:
    bool createTexture();
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11TEXTURE_H__
