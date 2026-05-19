#pragma once

namespace GN::gpu2 {

/// Internal payload base for backend payloads and grouped payloads.
class GpuPayloadImpl : public GpuPayload {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuPayload);

    GpuPayloadImpl * nextPayloadInGroup() const { return mNextPayload.get(); }
    GpuPayloadImpl * prevPayloadInGroup() const { return mPrevPayload; }
    bool             isPayloadInGroup() const { return mPrevPayload || mNextPayload; }

protected:
    using GpuPayload::GpuPayload;

private:
    friend class GpuPayloadGroup;

    AutoRef<GpuPayloadImpl> mNextPayload;
    GpuPayloadImpl *        mPrevPayload = nullptr;
};

/// API-neutral representative payload for ordered child payloads.
class GpuPayloadGroup final : public GpuPayloadImpl {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadImpl);

    explicit GpuPayloadGroup(const StrA & name): GpuPayloadImpl(TYPE_INFO(), name) {}
    ~GpuPayloadGroup() override { clearChildren(); }

    GpuPayloadImpl * firstChild() const { return mFirstChild.get(); }
    GpuPayloadImpl * lastChild() const { return mLastChild; }

    void appendChild(AutoRef<GpuPayloadImpl> child) {
        GN_ASSERT(child);
        GN_ASSERT(!child->isPayloadInGroup());

        if (!mFirstChild) {
            mFirstChild = child;
            mLastChild  = child.get();
            return;
        }

        mLastChild->mNextPayload = child;
        child->mPrevPayload      = mLastChild;
        mLastChild               = child.get();
    }

private:
    void clearChildren() {
        AutoRef<GpuPayloadImpl> child = mFirstChild;
        while (child) {
            AutoRef<GpuPayloadImpl> next = child->mNextPayload;
            child->mNextPayload          = {};
            child->mPrevPayload          = nullptr;
            child                        = next;
        }
        mFirstChild = {};
        mLastChild  = nullptr;
    }

    AutoRef<GpuPayloadImpl> mFirstChild;
    GpuPayloadImpl *        mLastChild = nullptr;
};

AutoRef<GpuPayload> combineGpuPayloads(const StrA & name, const ArrayProxy<AutoRef<GpuPayload>> & payloads);

} // namespace GN::gpu2
