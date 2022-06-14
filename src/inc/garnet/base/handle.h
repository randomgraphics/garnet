#ifndef __GN_BASE_HANDLE_H__
#define __GN_BASE_HANDLE_H__
// *****************************************************************************
/// \file
/// \brief   Handle manager
/// \author  chenlee (2005.7.25)
// *****************************************************************************

namespace GN {
///
/// Handle Manager
///
template<typename T, typename HANDLE_TYPE = size_t>
class HandleManager : public NoCopy {
    ///
    /// Handle item used internally by manager
    ///
    struct Item {
        uint8 buf[sizeof(T)];
        bool  occupied;

        T & t() const {
            GN_ASSERT(occupied);
            return *(T *) buf;
        }

        void ctor() {
            GN_ASSERT(!occupied);
            new (buf) T();
            occupied = true;
        }

        void ctor(const T & v) {
            GN_ASSERT(!occupied);
            new (buf) T(v);
            occupied = true;
        }

        void dtor() {
            GN_ASSERT(occupied);
            t().T::~T();
            occupied = false;
        }
    };

    FixSizedRawMemoryPool<sizeof(Item)> mPool;
    DynaArray<Item *>                   mItems;
    DynaArray<size_t>                   mFreeList;

    static inline size_t h2idx(HANDLE_TYPE h) { return (size_t) h - 1; }

    static HANDLE_TYPE idx2h(size_t idx) { return (HANDLE_TYPE) (idx + 1); }

public:
    ///
    /// dtor
    ///
    ~HandleManager() { clear(); }

    ///
    /// clear all handles
    ///
    void clear() {
        for (size_t i = 0; i < mItems.size(); ++i) {
            GN_ASSERT(mItems[i]);
            if (mItems[i]->occupied) mItems[i]->dtor();
            mPool.dealloc(mItems[i]);
        }
        mItems.clear();
        mFreeList.clear();
    }

    ///
    /// Get number of handles
    ///
    size_t size() const { return mItems.size() - mFreeList.size(); }

    ///
    /// Is the manager empty or not.
    ///
    bool empty() const { return mItems.size() == mFreeList.size(); }

    ///
    /// get current capacity
    ///
    size_t capacity() const { return mItems.capacity(); }

    ///
    /// set capacity
    ///
    void reserve(size_t n) {
        mItems.reserve(n);
        mFreeList.reserve(n);
    }

    ///
    /// return first handle
    ///
    HANDLE_TYPE first() const {
        if (empty()) return (HANDLE_TYPE) 0;
        size_t idx = 0;
        while (!mItems[idx]->occupied) {
            GN_ASSERT(idx < mItems.size());
            ++idx;
        }
        return idx2h(idx);
    }

    ///
    /// return next handle
    ///
    HANDLE_TYPE next(HANDLE_TYPE h) const {
        if (!validHandle(h)) return (HANDLE_TYPE) 0;
        size_t idx = h2idx(h) + 1;
        while (idx < mItems.size() && !mItems[idx]->occupied) {
            GN_ASSERT(idx < mItems.size());
            ++idx;
        }
        return idx < mItems.size() ? idx2h(idx) : (HANDLE_TYPE) 0;
    }

    ///
    /// Add new item with user define value
    ///
    HANDLE_TYPE add(const T & val) {
        if (mFreeList.empty()) {
            Item * newItem = (Item *) mPool.alloc();
            if (0 == newItem) {
                GN_ERROR(getLogger("GN.base.HandleManager"))("out of memory");
                return 0;
            }
            newItem->occupied = false;
            newItem->ctor(val);
            mItems.append(newItem);
            return (HANDLE_TYPE) mItems.size();
        } else {
            size_t i = mFreeList.back();
            mFreeList.popBack();
            GN_ASSERT(mItems[i]);
            mItems[i]->ctor(val);
            return idx2h(i);
        }
    }

    ///
    /// Add new item, with undefined value
    ///
    HANDLE_TYPE newHandle() {
        if (mFreeList.empty()) {
            Item * newItem = (Item *) mPool.alloc();
            if (0 == newItem) {
                GN_ERROR(getLogger("GN.base.HandleManager"))("out of memory");
                return 0;
            }
            newItem->occupied = false;
            newItem->ctor();
            mItems.append(newItem);
            return (HANDLE_TYPE) mItems.size();
        } else {
            size_t i = mFreeList.back();
            mFreeList.popBack();
            GN_ASSERT(mItems[i]);
            mItems[i]->ctor();
            return idx2h(i);
        }
    }

    ///
    /// Remove item from manager
    ///
    bool remove(HANDLE_TYPE h) {
        if (!validHandle(h)) {
            GN_ERROR(getLogger("GN.base.HandleManager"))("Invalid handle!");
            return false;
        } else {
            size_t idx = h2idx(h);
            GN_ASSERT(mItems[idx]);
            mItems[idx]->dtor();
            mFreeList.append(idx);
            return true;
        }
    }

    ///
    /// Find specific item (always return first found)
    ///
    HANDLE_TYPE find(const T & val) const {
        for (size_t i = 0; i < mItems.size(); ++i) {
            if (!mItems[i]->occupied) continue;
            if (mItems[i]->t() == val) return idx2h(i); // found!
        }
        return (HANDLE_TYPE) 0; // not found
    }

    ///
    /// Find specific item (always return first found)
    ///
    template<typename FUNC>
    HANDLE_TYPE findIf(const FUNC & fp) const {
        for (size_t i = 0; i < mItems.size(); ++i) {
            if (!mItems[i]->occupied) continue;
            if (fp(mItems[i]->t())) return idx2h(i); // found!
        }
        return (HANDLE_TYPE) 0; // not found
    }

    ///
    /// Is valid handle or not?
    ///
    bool validHandle(HANDLE_TYPE h) const {
        size_t idx = h2idx(h);
        return idx < mItems.size() && mItems[idx]->occupied;
    }

    ///
    /// Get item from manager. Handle must be valid.
    ///
    T & get(HANDLE_TYPE h) const {
        GN_ASSERT(validHandle(h));
        return mItems[h2idx(h)]->t();
    }

    ///
    /// Get item from manager. Handle must be valid.
    ///
    T & operator[](HANDLE_TYPE h) const { return get(h); }
};

///
/// expension to HandleManager class, that object can be referenced by both handle and name.
///
template<typename T, typename H, bool CASE_INSENSITIVE = false>
class NamedHandleManager {
    typedef StringMap<char, H> NameMap;

    struct NamedItem {
        NamedHandleManager & mgr;
        const H              handle;
        const StrA           name;
        T                    data;

        NamedItem(NamedHandleManager & m, H h, const StrA & n, const T & d): mgr(m), handle(h), name(n), data(d) {}

        NamedItem(NamedHandleManager & m, H h, const StrA & n): mgr(m), handle(h), name(n) {}
    };

    NameMap                       mNames; // name -> handle
    HandleManager<NamedItem *, H> mItems; // handle -> name/data
    ObjectPool<NamedItem>         mPool;  // named item pool

public:
    //@{

    typedef H ItemHandle;

    ///
    /// dtor
    ///
    ~NamedHandleManager() { clear(); }

    ///
    /// clear all handles
    ///
    void clear() {
        for (H i = mItems.first(); i != 0; i = mItems.next(i)) { mPool.deconstructAndFree(mItems[i]); }
        mItems.clear();
        mNames.clear();
    }

    ///
    /// Get number of handles
    ///
    size_t size() const {
        GN_ASSERT(mItems.size() == mNames.size());
        return mItems.size();
    }

    ///
    /// Is the manager empty or not.
    ///
    bool empty() const {
        GN_ASSERT(mItems.size() == mNames.size());
        return mItems.empty();
    }

    ///
    /// return first handle
    ///
    H first() const { return mItems.first(); }

    ///
    /// return next handle
    ///
    H next(H h) const { return mItems.next(h); }

    ///
    /// name must be unique.
    ///
    H add(const StrA & name) {
        if (CASE_INSENSITIVE) { GN_UNIMPL(); }

        if (NULL != mNames.find(name)) {
            GN_ERROR(getLogger("GN.base.NamedHandleManager"))("name '%s' is not unique.", name.rawptr());
            return 0;
        }

        // create new item
        NamedItem * p = mPool.allocUnconstructed();
        if (0 == p) return 0;

        H handle = mItems.newHandle();
        if (0 == handle) return 0;

        new (p) NamedItem(*this, handle, name);

        mItems[handle] = p;

        mNames.insert(name, handle);

        return handle;
    }

    ///
    /// name must be unique.
    ///
    H add(const StrA & name, const T & data) {
        if (CASE_INSENSITIVE) { GN_UNIMPL(); }

        if (NULL != mNames.find(name)) {
            GN_ERROR(getLogger("GN.base.NamedHandleManager"))("name '%s' is not unique.", name.rawptr());
            return 0;
        }

        NamedItem * p = mPool.allocUnconstructed();
        if (0 == p) return 0;

        H handle = mItems.newHandle();
        if (0 == handle) return 0;

        new (p) NamedItem(*this, handle, name, data);

        mItems[handle] = p;

        mNames.insert(name, handle);

        return handle;
    }

    void remove(H h) {
        if (!validHandle(h)) {
            GN_ERROR(getLogger("GN.base.NamedHandleManager"))("invalid handle : %d.", h);
            return;
        }

        NamedItem * item = mItems[h];
        GN_ASSERT(item && item->handle == h);

        mNames.remove(item->name);

        mItems.remove(item->handle);

        mPool.deconstructAndFree(item);
    }

    void remove(const StrA & name) {
        if (CASE_INSENSITIVE) { GN_UNIMPL(); }

        if (!validName(name)) {
            GN_ERROR(getLogger("GN.base.NamedHandleManager"))("invalid name: %s.", name.rawptr());
            return;
        }

        size_t handle = mNames[name];

        NamedItem * item = mItems[handle];

        mNames.remove(name);

        mItems.remove(handle);

        mPool.deconstructAndFree(item);
    }

    bool validHandle(H h) const { return mItems.validHandle(h); }

    bool validName(const StrA & name) const {
        if (CASE_INSENSITIVE) { GN_UNIMPL(); }

        return NULL != mNames.find(name);
    }

    H name2handle(const StrA & name) const {
        if (CASE_INSENSITIVE) { GN_UNIMPL(); }

        const H * h = mNames.find(name);

        return h ? *h : (H) 0;
    }

    const StrA * handle2name(H h) const {
        if (!mItems.validHandle(h))
            return NULL;
        else
            return &mItems[h]->name;
    }

    T & get(H h) const { return mItems[h]->data; }

    T & get(const StrA & name) const {
        GN_ASSERT(validName(name));
        return mItems[mNames[name]]->data;
    }

    T & operator[](H h) const { return get(h); }

    T & operator[](const StrA & name) const { return get(name); }

    //@}
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_HANDLE_H__
