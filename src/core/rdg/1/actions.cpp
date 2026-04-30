#include "pch.h"

namespace GN::rdg {

namespace {

static void resourceTableReadWriteList(const GpuShaderAction::GpuResourceTable & resources, Arguments::ArtifactReadWriteList & list) {
    for (const auto & set : resources) {
        for (const auto & slot : set) {
            for (const auto & view : slot) {
                if (view.artifact.empty()) GN_UNLIKELY continue;
                auto ptr = view.artifact.get();
                list.readList.insert(ptr);
                if ((view.isImage() && view.imageView.type == GpuResourceView::ImageView::Type::STORAGE) ||
                    (view.isBuffer() && view.bufferView.type == GpuResourceView::BufferView::Type::STORAGE))
                    list.writeList.insert(ptr);
            }
        }
    }
}

} // end of anonymous namespace

GN_API void GpuDraw::A::addToReadWriteList(ArtifactReadWriteList & list) const {
    // geometry
    for (const auto & b : geometry.instances) {
        if (b.buffer) { list.readList.insert(b.buffer.get()); }
    }
    for (const auto & b : geometry.vertices) {
        if (b.buffer) { list.readList.insert(b.buffer.get()); }
    }
    if (geometry.indices.buffer) { list.readList.insert(geometry.indices.buffer.get()); }
    // resources
    resourceTableReadWriteList(resources, list);
    // render target
    if (renderTarget) {
        for (const auto & color : renderTarget->colors) {
            if (color.target.artifact) { list.readList.insert(color.target.artifact.get()); }
        }
        if (renderTarget->depthStencilTarget.artifact) {
            auto ptr = renderTarget->depthStencilTarget.artifact.get();
            if (renderTarget->stencilState.enabled()) {
                list.readList.insert(ptr);
                list.writeList.insert(ptr);
            } else {
                if (renderTarget->depthState.testEnabled()) list.readList.insert(ptr);
                if (renderTarget->depthState.writeEnabled()) list.writeList.insert(ptr);
            }
        }
    }
}

GN_API void GpuCompute::A::addToReadWriteList(ArtifactReadWriteList & list) const { resourceTableReadWriteList(resources, list); }

} // namespace GN::rdg