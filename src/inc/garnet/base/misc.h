﻿#ifndef __GN_BASE_MISC_H__
#define __GN_BASE_MISC_H__
// *****************************************************************************
/// \file
/// \brief   misc functions and types
/// \author  chenlee (2005.5.4)
// *****************************************************************************

#include <chrono>
#include <type_traits>

///
/// Get element count of C-style array
///
#define GN_ARRAY_COUNT(x) (sizeof(x) / sizeof(x[0]))

///
/// Get byte offset of class member or struct field
///
#define GN_FIELD_OFFSET(class_, field) ((size_t) (intptr_t) & (((class_ *) (void *) (8))->field) - 8)

///
/// Execute only limited times during the entir life of the
/// application, no matter how many time it is called.
///
#define GN_DO_LIMITED_TIMES(n, X)    \
    if (true) {                      \
        static size_t s_counter = n; \
        if (s_counter > 0) {         \
            --s_counter;             \
            X;                       \
        }                            \
    } else                           \
        void(0)

///
/// Do something only once. 通常用来在内层循环中输出一些调试和错误信息。
///
#define GN_DO_ONCE(X) GN_DO_LIMITED_TIMES(1, X)

#define GN_DO_EVERY_N_SEC(interval, x)              \
    do {                                            \
        static GN::detail::IntervalTimer timer____; \
        if (timer____(interval)) { x; }             \
    } while (0)

///
/// "Use" unused parameter (to avoid compiler warnings)
///
#define GN_UNUSED_PARAM(X) ((void) (X))

///
/// compose RGBA8 color constant
///
#define GN_RGBA8(r, g, b, a) ((((uint32) (r) &0xFF) << 0) | (((uint32) (g) &0xFF) << 8) | (((uint32) (b) &0xFF) << 16) | (((uint32) (a) &0xFF) << 24))

///
/// compose BGRA8 color constant
///
#define GN_BGRA8(r, g, b, a) ((((uint32) (b) &0xFF) << 0) | (((uint32) (g) &0xFF) << 8) | (((uint32) (r) &0xFF) << 16) | (((uint32) (a) &0xFF) << 24))

namespace GN {

/// @brief A utility function to combine hash values.
/// @tparam T Type of the value.
/// @param seed Reference to the current hash value to combine to. This will also be the new hash value.
/// @param val The value that we'd like to add to the hash.
template<typename T>
inline void combineHash(std::size_t & seed, const T & val) {
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

/// @brief Hash anything as a plain memory block.
/// @details This is useful for hashing POD types. Be careful when your type contains padding bytes.
///          Those padding bytes could interfere with the hash calculation and generate unexpected result.
/// @tparam T type of the value.
template<typename T>
struct BlobHash {
    enum {
        N_SIZE_T = sizeof(T) / sizeof(size_t),
        TAIL     = sizeof(T) % sizeof(size_t),
    };

    size_t operator()(const T & t) const {
        const size_t * pz = (const size_t *) &t;

        size_t h = 0;
        for (size_t i = 0; i < N_SIZE_T; ++i, ++pz) { combineHash(h, *pz); }

        const uint8_t * p8 = (const uint8_t *) pz;
        for (size_t i = 0; i < TAIL; ++i, ++p8) { combineHash(h, *p8); }

        return h;
    }
};

/// @brief Equal function for comparing the type as a plain memory block.
/// @details Be careful when your type contains padding bytes. It could interfere with the comparison and yield unexpected result.
/// @tparam T
template<typename T>
struct MemoryEqual {
    bool operator()(const T & a, const T & b) const { return 0 == ::memcmp(&a, &b, sizeof(T)); }
};

///
/// type cast function
///
/// perform dynamic cast in debug build, and reinterpret cast in release build.
// ------------------------------------------------------------------------
template<class TO, class FROM>
GN_FORCE_INLINE TO & safeCastRef(FROM & from) {
#if GN_BUILD_DEBUG_ENABLED && (!GN_MSVC || defined(_CPPRTTI))
    return dynamic_cast<TO &>(from);
#else
    return reinterpret_cast<TO &>(from);
#endif
}

///
/// type cast function
///
/// perform dynamic cast in debug build, and reinterpret cast in release build.
// ------------------------------------------------------------------------
template<class TO, class FROM>
GN_FORCE_INLINE TO * safeCastPtr(FROM * from) {
#if GN_BUILD_DEBUG_ENABLED && (!GN_MSVC || defined(_CPPRTTI))
    TO * to = dynamic_cast<TO *>(from);
#else
    TO * to = reinterpret_cast<TO *>(from);
#endif

    GN_ASSERT(0 == from || 0 != to);

    return to;
}

///
/// Set environment variable. Set value to empty or NULL to delete
/// that environment variable.
///
GN_API void putEnv(const char * name, const char * value);

///
/// Get environment variable.
///
GN_API void getEnv(StrA & result, const char * name);

///
/// Get environment variable.
///
inline StrA getEnv(const char * name) {
    StrA result;
    getEnv(result, name);
    return result;
}

///
/// Get environment variable as boolean variable. Return true only when the variable
/// exists and the value is "1" or "yes" (case-insensitive).
///
inline bool getEnvBoolean(const char * name) {
    StrA result;
    getEnv(result, name);
    return "1" == result || 0 == str::compareI("yes", result.rawptr()) || 0 == str::compareI("true", result.rawptr());
}

///
/// GUID class
///
struct GN_API Guid {
    ///
    /// Hasing
    ///
    struct Hash {
        uint64_t operator()(const Guid & guid) const {
            const uint64_t * u64 = (const uint64_t *) &guid;
            return u64[0] + u64[1];
        }
    };

    /// \name data members
    //@{
    uint32  data1;
    uint16  data2;
    uint16  data3;
    uint8_t data4[8];
    //@}

    /// \name public methods
    //@{

    ///
    /// Convert to string
    ///
    /// \note: this is not thread safe
    ///
    const char * toStr() const;

    //@}

    /// \name operators
    //@{

    bool operator==(const Guid & rhs) const {
        const uint64_t * a = (const uint64_t *) this;
        const uint64_t * b = (const uint64_t *) &rhs;
        return a[0] == b[0] && a[1] == b[1];
    }

    bool operator!=(const Guid & rhs) const {
        const uint64_t * a = (const uint64_t *) this;
        const uint64_t * b = (const uint64_t *) &rhs;
        return a[0] != b[0] || a[1] != b[1];
    }

    bool operator<(const Guid & rhs) const {
        // Note: may produce different result on machine with different endian.
        const uint64_t * a = (const uint64_t *) this;
        const uint64_t * b = (const uint64_t *) &rhs;
        if (a[0] != b[0])
            return a[0] < b[0];
        else
            return a[1] < b[1];
    }

    //@}
};
GN_CASSERT(sizeof(Guid) == 128 / 8);

///
/// Hard to explain in English, please see code by your self :)
///
struct ScopeBool {
    bool & mValue; ///< Reference to the boolean variable.

    ///
    /// Ctor
    ///
    ScopeBool(bool & v): mValue(v) { mValue = true; }

    ///
    /// Dtor
    ///
    ~ScopeBool() { mValue = false; }
};

///
/// Increase counter by one when entering the life scope, decrease the counter by one
/// when going out of the life scope.
///
template<typename T>
struct ScopeCounter {
    T & mValue; ///< Reference to the integer variable.

    ///
    /// Ctor
    ///
    ScopeCounter(T & v): mValue(v) { ++mValue; }

    ///
    /// Dtor
    ///
    ~ScopeCounter() { --mValue; }
};

///
/// Data type with automatic-initialization.
///
template<typename T, T DEFAULT_VALUE>
class AutoInitializer {
    T mValue;

public:
    ///
    /// Default constructor
    ///
    AutoInitializer(): mValue(DEFAULT_VALUE) {}

    ///
    /// Construct from T
    ///
    AutoInitializer(const T & value): mValue(value) {}

    ///
    /// get internal value
    ///
    T & value() { return mValue; }

    ///
    /// get internal value
    ///
    const T & value() const { return mValue; }

    ///
    /// Convert to T
    ///
    operator T &() { return mValue; }

    ///
    /// Convert to constant T
    ///
    operator const T &() const { return mValue; }
};

///
/// Automatically execute a function at the end of life scope
///
template<class FUNCTOR>
class AutoFinalizer {
    FUNCTOR mFunc;
    bool    mDismissed;

public:
    ///
    /// Ctor
    ///
    AutoFinalizer(const FUNCTOR & func): mFunc(func), mDismissed(false) {}

    ///
    /// Dtor
    ///
    ~AutoFinalizer() {
        if (!mDismissed) mFunc();
    }

    ///
    /// Tell finalizer to _NOT_ run the function by the end of life scope.
    ///
    void dismiss() { mDismissed = true; }
};

namespace detail {
class IntervalTimer {
    std::chrono::high_resolution_clock::time_point _lastLogTime;

public:
    IntervalTimer() {}
    bool operator()(int interval) {
        using namespace std::chrono;
        if (interval <= 0) return true;
        auto now = high_resolution_clock::now();
        auto dur = duration_cast<seconds>(now - _lastLogTime).count();
        if (dur < interval) return false;
        _lastLogTime = now;
        return true;
    }
};
} // namespace detail
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MISC_H__
