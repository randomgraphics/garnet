#ifndef __GN_GFX_MESHRESOURCE_H__
#define __GN_GFX_MESHRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   Mesh resource implementation class
/// \author  chenli@@REDMOND (2009.8.28)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN {
namespace gfx {
///
/// Vertex format analyzer.
///
struct VertexFormatProperties {
    /// minimal strides for each stream
    uint16_t minStrides[GpuContext::MAX_VERTEX_BUFFERS];

    /// true means that stream is referenced by the vertex format.
    bool used[GpuContext::MAX_VERTEX_BUFFERS];

    /// analyze vertex format
    bool analyze(const MeshVertexFormat & vf);
};

///
/// Mesh resource implementation class
///
class MeshResource::Impl {
    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    Impl(MeshResource & owner): mOwner(owner) {}
    virtual ~Impl() { clear(); }
    //@}

    // ********************************
    // public functions
    // ********************************
public:
    bool                         reset(const MeshResourceDesc * desc);
    const MeshResourceDescBase & getDesc() const { return mDesc; }
    void                         applyToContext(GpuContext & context) const;
    void                         calculateBoundingBox(Box<float> &) const;
    void                         calculateBoundingSphere(Sphere<float> &) const;

    // ********************************
    // private variables
    // ********************************
private:
    struct VertexBuffer {
        AutoRef<VtxBuf> gpudata;
    };

    struct IndexBuffer {
        AutoRef<IdxBuf> gpudata;
    };

    MeshResource &       mOwner;
    MeshResourceDescBase mDesc;
    VertexBuffer         mVtxBufs[GpuContext::MAX_VERTEX_BUFFERS];
    IndexBuffer          mIdxBuf;

    // ********************************
    // private functions
    // ********************************
private:
    GpuResourceDatabase & getGdb() const { return mOwner.getGdb(); }
    const char *          getMeshName() const { return mOwner.getGdb().getResourceName(&mOwner)->rawptr(); }

    bool create(const MeshResourceDesc & desc);
    void clear();
};

///
/// register mesh resource factory
///
bool registerMeshResourceFactory(GpuResourceDatabase & db);
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MESHRESOURCE_H__
