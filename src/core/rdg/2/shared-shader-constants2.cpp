#include <garnet/GNrdg2.h>

static GN::Logger * sLogger = GN::getLogger("GN.rdg2");

namespace GN::rdg2 {

class SharedShaderConstants2Impl : public SharedShaderConstants2 {

    AutoRef<GN::gpu2::GpuContext> mGpu;
    AutoRef<Graph>                mGraph;

public:
    GN_REGISTER_RUNTIME_TYPE(SharedShaderConstants2);

    SharedShaderConstants2Impl(): SharedShaderConstants2(TYPE_INFO(), name) {}

    bool init(const CreateParameters & params) {
        if (!params.gpu || !params.graph) {
            GN_ERROR(sLogger)("Invalid parameters for creating SharedShaderConstants2Impl");
            return false;
        }
        return true;
    }

    TokenPtr takeSnapshot() const override {
        GN_TODO("SharedShaderConstants2Impl::getToken() not implemented");
        return {};
    }

    AutoRef<Content> getContent(TokenPtr) const override {
        GN_TODO("SharedShaderConstants2Impl::getSet0Content() not implemented");
        return {};
    }
};

GN_API AutoRef<SharedShaderConstants2> SharedShaderConstants2::create(const CreateParameters & params) {
    auto ret = AutoRef<SharedShaderConstants2Impl>(new SharedShaderConstants2Impl());
    if (!ret->init(params)) {
        GN_ERROR(sLogger)("Failed to initialize SharedShaderConstants2Impl");
        return {};
    }
    return ret;
}

} // namespace GN::rdg2
