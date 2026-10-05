#include "pch.h"

namespace GN::fx2::bindless {

// This shell keeps the public contract usable while the two stores are implemented separately.
class SharedShaderConstantsDummy : public SharedShaderConstants {
    AutoRef<gpu2::GpuContext> mGpu;

public:
    GN_REGISTER_RUNTIME_TYPE(SharedShaderConstants);

    explicit SharedShaderConstantsDummy(AutoRef<gpu2::GpuContext> gpu)
        : SharedShaderConstants(TYPE_INFO(), "bindless.SharedShaderConstants"), mGpu(std::move(gpu)) {}

    AutoRef<UniformState> recordUniformUpdate(gpu2::bindless::CnC &, ArrayView<const uint8_t>) override { return {}; }

    StreamingBlock allocateStreaming(uint64_t) override { return {}; }

    void releaseStreaming(StreamingId) override {}
};

GN_API AutoRef<SharedShaderConstants> SharedShaderConstants::create(const CreateParameters & params) {
    if (!params.gpu || !params.uniformCapacity || !params.streamingCapacity) GN_UNLIKELY return {};
    return AutoRef<SharedShaderConstants>(new SharedShaderConstantsDummy(params.gpu));
}

} // namespace GN::fx2::bindless
