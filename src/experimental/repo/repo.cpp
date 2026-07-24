// RETIRED: kept as design reference only. Not part of the active build. See repo.h.

#include "pch.h"

#include <map>
#include <mutex>
#include <vector>

namespace GN::rdg2 {

namespace {

GN::Logger * sLogger = GN::getLogger("GN.rdg2.repo");

template<typename RESOURCE>
uint8_t nextRepoId() {
    static std::mutex           m;
    static uint8_t              next = 1;
    std::lock_guard<std::mutex> lock(m);
    if (next == 0) {
        GN_ERROR(sLogger)("Too many repositories created for one RDG2 resource type");
        return 0;
    }
    return next++;
}

template<typename RESOURCE>
class RepoImplBase {
public:
    using RepoT    = typename RESOURCE::Repo;
    using Handle   = typename RESOURCE::Handle;
    using LoadDesc = typename RESOURCE::LoadDesc;
    using Key      = typename LoadDesc::Key;
    using Content  = typename LoadDesc::Content;
    using Ref      = typename RepoT::Ref;

    bool init(const StrA & name, GraphPtr graph) {
        if (!graph) GN_UNLIKELY {
                GN_ERROR(sLogger)("ResourceRepo ({}): graph is null", name);
                return false;
            }

        mRepoId = nextRepoId<RESOURCE>();
        if (!mRepoId) GN_UNLIKELY return false;

        mName  = name;
        mGraph = graph;
        mSlots.resize(1); // index 0 is reserved so default handles stay invalid.
        return true;
    }

    Ref find(const Key & key) const {
        auto found = mByKey.find(key);
        if (found == mByKey.end()) return {};
        return refFromIndex(found->second);
    }

    Ref load(const LoadDesc & desc, std::function<AutoRef<Content>(const Key &)> loader) {
        // check to redundant load. return same resource for same resource key.
        auto ref = find(desc.key);
        if (!ref.handle.empty()) return ref;

        // some sanity checks, unlikely to fail.
        if (!mGraph) GN_UNLIKELY {
                GN_ERROR(sLogger)("ResourceRepo ({}): load failed because graph is null", mName);
                return {};
            }
        if (!loader) GN_UNLIKELY {
                GN_ERROR(sLogger)("ResourceRepo ({}): loader can't be empty.", mName);
                return {};
            }

        // create a new artifact.
        auto artifact = mGraph->createArtifact(desc.key.uri);
        if (!artifact) GN_UNLIKELY return {};

        // Publish fallback before deferred loading so downstream nodes can bind valid content immediately.
        if (desc.fallback) mGraph->publishArtifact(artifact, desc.fallback);

        // Create a new slot
        const uint32_t index = allocateSlot();
        Slot &         slot  = mSlots[index];
        GN_ASSERT(!slot.alive); // the newly allocated slot must not be a live one.
        slot.alive       = true;
        slot.key         = desc.key;
        slot.hotReload   = desc.hotReload;
        slot.artifact    = artifact;
        mByKey[desc.key] = index;

        // Define the loading action.
        auto action = [graph = mGraph, artifact, key = desc.key, loader, repoName = mName]() {
            auto content = loader(key);
            if (content) {
                graph->publishArtifact(artifact, content);
            } else {
                GN_WARN(sLogger)("ResourceRepo ({}): failed to load resource '{}'. Resource content remains unchanged.", repoName, key.uri);
            }
        };

        // load the resource content
        if (desc.deferred) {
            auto nodeName = fmt::format("repo {}: load {}", mName, desc.key.uri);
            mGraph->addNode(NodeDesc(nodeName, action));
        } else {
            action();
        }

        return refFromIndex(index);
    }

    bool reload(Handle) {
        GN_ERROR(sLogger)("resource reloading is not implemented yet.");
        return false;
    }

    void pollHotReload() {
        // File watching and async reload scheduling are intentionally left to resource-specific loaders.
        GN_ERROR(sLogger)("resource reloading is not implemented yet.");
    }

    bool remove(Handle handle) {
        Slot * slot = slotFromHandle(handle);
        if (!slot) return false;

        const uint32_t index = handle.index();
        mByKey.erase(slot->key);
        releaseSlot(index);
        return true;
    }

    size_t size() const { return mByKey.size(); }

    void clear() {
        mByKey.clear();
        mFreeList.clear();
        mSlots.clear();
        mGraph.clear();
        mRepoId = 0;
    }

private:
    struct Slot {
        bool        alive      = false;
        uint32_t    generation = 1;
        Key         key        = {}; // TODO: consider store iterator to mByKey
        bool        hotReload  = false;
        ArtifactPtr artifact   = {};
    };

    StrA                    mName;
    GraphPtr                mGraph;
    uint8_t                 mRepoId = 0;
    std::vector<Slot>       mSlots;
    std::vector<uint32_t>   mFreeList;
    std::map<Key, uint32_t> mByKey;

    Handle makeHandle(uint32_t index, uint32_t generation) const {
        Handle handle;
        handle.value = RepoHandle64::make(mRepoId, index, generation).value;
        return handle;
    }

    Ref refFromIndex(uint32_t index) const {
        if (index == 0 || index >= mSlots.size()) GN_UNLIKELY return {};
        const Slot & slot = mSlots[index];
        if (!slot.alive) GN_UNLIKELY return {};
        return {makeHandle(index, slot.generation), slot.artifact};
    }

    Slot * slotFromHandle(Handle handle) {
        if (handle.empty() || handle.repo() != mRepoId) GN_UNLIKELY return nullptr;
        const uint32_t index = handle.index();
        if (index == 0 || index >= mSlots.size()) GN_UNLIKELY return nullptr;
        Slot & slot = mSlots[index];
        if (!slot.alive || slot.generation != handle.generation()) GN_UNLIKELY return nullptr;
        return &slot;
    }

    uint32_t allocateSlot() {
        if (!mFreeList.empty()) {
            const uint32_t index = mFreeList.back();
            mFreeList.pop_back();
            return index;
        }

        mSlots.emplace_back();
        return static_cast<uint32_t>(mSlots.size() - 1);
    }

    void releaseSlot(uint32_t index) {
        Slot &   slot           = mSlots[index];
        uint32_t nextGeneration = slot.generation + 1;
        if (nextGeneration == 0) {
            GN_WARN(sLogger)("ResourceRepo ({}): Slot {} is overflowing.", mName, index);
            ++nextGeneration; // handle overflow.
        }
        slot            = Slot {};
        slot.generation = nextGeneration;
        mFreeList.push_back(index);
    }
};

// class TextureRepo final : public TextureResource::Repo {
// public:
//     GN_REGISTER_RUNTIME_TYPE(TextureResource::Repo);

//     explicit TextureRepo(const StrA & name): TextureResource::Repo(TYPE_INFO(), name) {}

//     ~TextureRepo() override { clear(); }

//     bool init(const CreateParameters & cp) {
//         mGpu = cp.gpu;
//         return mImpl.init(name, cp.graph);
//     }

//     Ref find(const Key & key) const override { return mImpl.find(key); }

//     Ref load(const LoadDesc & desc) override {
//         auto gpu = mGpu;
//         return mImpl.load(desc, [gpu](const Key & key) -> AutoRef<TextureResource::Content> {
//             if (!gpu) return {};
//             auto texture = gpu2::Texture::load({.context = gpu, .filename = key.uri});
//             if (!texture) return {};
//             return AutoRef<TextureResource::Content>(new TextureResource::Content(key.uri, std::move(texture)));
//         });
//     }

//     bool reload(Handle handle) override { return mImpl.reload(handle); }

//     void pollHotReload() override { mImpl.pollHotReload(); }

//     bool remove(Handle handle) override { return mImpl.remove(handle); }

//     size_t size() const override { return mImpl.size(); }

// private:
//     void clear() {
//         mImpl.clear();
//         mGpu.clear();
//     }

//     AutoRef<gpu2::GpuContext>     mGpu;
//     RepoImplBase<TextureResource> mImpl;
// };

} // namespace

// /// Create a texture repository.
// GN_API AutoRef<TextureResource::Repo> TextureResource::Repo::create(const StrA & name, const TextureResource::Repo::CreateParameters & cp) {
//     if (!cp.graph) GN_UNLIKELY {
//             GN_ERROR(sLogger)("TextureResource::Repo::create: graph is null");
//             return {};
//         }
//     auto p = AutoRef<TextureRepo>(new TextureRepo(name));
//     if (!p || !p->init(cp)) return {};
//     return p;
// }

// /// Create a geometry repository.
// GN_API AutoRef<GpuGeometryResource::Repo> GpuGeometryResource::Repo::create(const GpuGeometryResource::Repo::CreateParameters & cp) {
//     GN_UNUSED_PARAM(cp);
//     return {};
// }

// /// Create a shader repository.
// GN_API AutoRef<ShaderResource::Repo> ShaderResource::Repo::create(const ShaderResource::Repo::CreateParameters & cp) {
//     GN_UNUSED_PARAM(cp);
//     return {};
// }

} // namespace GN::rdg2
