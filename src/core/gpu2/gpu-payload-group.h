#pragma once

namespace GN::gpu2 {

/// API-neutral representative payload for ordered child payloads.
class GpuPayloadGroup final : public GpuPayload {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuPayload);

    GpuPayloadGroup(const StrA & name, ArrayContainer<AutoRef<GpuPayload>> children): GpuPayload(TYPE_INFO(), name), mChildren(std::move(children)) {
        for (auto & child : mChildren) {
            if (child) child->mPayloadGroupOwner = this;
        }
    }

    ~GpuPayloadGroup() override {
        for (auto & child : mChildren) {
            if (child && child->mPayloadGroupOwner == this) child->mPayloadGroupOwner = nullptr;
        }
    }

    const ArrayContainer<AutoRef<GpuPayload>> & children() const { return mChildren; }

private:
    ArrayContainer<AutoRef<GpuPayload>> mChildren;
};

} // namespace GN::gpu2
