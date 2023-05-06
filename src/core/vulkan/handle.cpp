#include "pch.h"

namespace GN::vulkan {

struct Chunk {
    Chunk * next;
};

class PoolAllocator {
private:
    std::mutex          _mutex;
    size_t              _chunksPerBlock;
    Chunk *             _startPtr = nullptr;
    std::vector<void *> _allocatedBlocks;
    Chunk *             allocateBlock(size_t chunkSize);

public:
    // TODO: Determine optimal # of block allocations for any given application
    static PoolAllocator & getInstance(size_t chunksPerBlock = 176);

    PoolAllocator(size_t chunksPerBlock);
    ~PoolAllocator();

    void * allocate(size_t size);
    void   deallocate(void * ptr, size_t);

private:
    PoolAllocator(PoolAllocator & poolAllocator);
};

PoolAllocator::PoolAllocator(size_t chunksPerBlock): _chunksPerBlock(chunksPerBlock) {}

PoolAllocator::~PoolAllocator() {
    for (auto & block : _allocatedBlocks) { free(block); }
    _allocatedBlocks.clear();
}

Chunk * PoolAllocator::allocateBlock(size_t chunkSize) {
    void * block = malloc(_chunksPerBlock * chunkSize);
    _allocatedBlocks.emplace_back(block);
    Chunk * blockBegin = reinterpret_cast<Chunk *>(block);
    Chunk * chunk      = blockBegin;
    for (int i = 0; i < static_cast<int>(_chunksPerBlock) - 1; ++i) {
        chunk->next = reinterpret_cast<Chunk *>(reinterpret_cast<char *>(chunk) + chunkSize);
        chunk       = chunk->next;
    }
    chunk->next = nullptr;
    return blockBegin;
}

PoolAllocator & PoolAllocator::getInstance(size_t chunksPerBlock) {
    static PoolAllocator instance(chunksPerBlock);
    return instance;
}

void * PoolAllocator::allocate(size_t size) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_startPtr == nullptr) { _startPtr = allocateBlock(size); }
    Chunk * freeChunk = _startPtr;
    _startPtr         = _startPtr->next;
    return freeChunk;
}

void PoolAllocator::deallocate(void * ptr, size_t) {
    std::lock_guard<std::mutex> lock(_mutex);
    reinterpret_cast<Chunk *>(ptr)->next = _startPtr;
    _startPtr                            = reinterpret_cast<Chunk *>(ptr);
}

GN_API void * allocateAutoHandle(size_t size) { return PoolAllocator::getInstance().allocate(size); }

GN_API void deallocateAutoHandle(void * ptr, size_t size) { return PoolAllocator::getInstance().deallocate(ptr, size); }

} // namespace GN::vulkan