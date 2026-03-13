#pragma once

/// \file rtti.h
/// Run-time type information for RDG: unique 64-bit IDs, string names, optional base types.
/// Safe across DLL boundaries: getNextUniqueTypeId() and getStableTypeIdFromName() are
/// defined in one TU and exported (GN_API) so all modules share the same counter and name map.

#include <garnet/GNbase.h>

#include <cstddef>
#include <cstdint>
#include <concepts>

namespace GN::rdg {

/// Base mixin for run-time type information: unique 64-bit ID, string name, optional base types.
/// Layout is header-only; use across DLLs is safe as long as all modules include this same header.
/// Use GN_REGISTER_RUNTIME_TYPE in your type to define TYPE_ID and TYPE_NAME (and optionally BASE_TYPE_IDS).
///
/// Example:
///   struct A { GN_REGISTER_RUNTIME_TYPE(A); };
///   struct B { GN_REGISTER_RUNTIME_TYPE(B); };
///   struct C : B { GN_REGISTER_RUNTIME_TYPE(C, B); };
///   struct D : A, B { GN_REGISTER_RUNTIME_TYPE(D, A, B); };
struct RuntimeType {
    struct TypeInfo {
        const uint64_t           id    = 0;
        const char * const       name  = nullptr;
        const uint32_t           level = 0; ///< length of the inheritance chain
        const TypeInfo * const * bases = nullptr;
        const std::size_t        count = 0; ///< number of base types

        // Recursively check if this type is derived from other type.
        bool isDerivedFrom(const TypeInfo & other) const {
            if (id == other.id) GN_LIKELY return true;

            /// if the level of this type is less than the level of the other type,
            /// then this type MUST NOT be derived from the other type.
            if (level < other.level) return false;

            /// Recursively check if this type is derived from any of the base types.
            for (std::size_t i = 0; i < count; ++i) {
                if (bases[i]->isDerivedFrom(other)) return true;
            }

            return false;
        }
    };

    const TypeInfo & typeInfo() const { return mTypeInfo; }

    /// Type ID for compatibility; same as typeInfo().id.
    uint64_t typeId() const { return mTypeInfo.id; }

    template<typename T>
    T * castTo() {
        if (mTypeInfo.isDerivedFrom(T::TYPE_INFO())) GN_LIKELY return static_cast<T *>(this);
        return nullptr;
    }

    template<typename T>
    const T * castTo() const {
        if (mTypeInfo.isDerivedFrom(T::TYPE_INFO())) GN_LIKELY return static_cast<const T *>(this);
        return nullptr;
    }

    /// Returns a new unique 64-bit type ID each call. Thread-safe. Defined in one TU and exported
    /// (GN_API) so the same counter is used across DLL boundaries.
    /// Use for TYPE_ID in GN_REGISTER_RUNTIME_TYPE; do not persist across process runs.
    GN_API static uint64_t getNextUniqueTypeId();

protected:
    const TypeInfo & mTypeInfo;

    RuntimeType(const TypeInfo & typeInfo): mTypeInfo(typeInfo) {}
};

// --- Template: base type IDs from parameter pack ----------------------------------------------

template<typename... BaseTypes>
struct RttiBaseTypeIds {
    /// return max level of all base types.
    static constexpr uint32_t LEVEL() {
        uint32_t level = 0;
        ((level = std::max(level, BaseTypes::TYPE_INFO().level)), ...);
        return level;
    }

    static const RuntimeType::TypeInfo * const * BASE() {
        static const RuntimeType::TypeInfo * const table[] = {
            &BaseTypes::TYPE_INFO()...,
        };
        return table;
    }
    static constexpr std::size_t SIZE() { return sizeof...(BaseTypes); }
};

template<>
struct RttiBaseTypeIds<> {
    static constexpr uint32_t                    LEVEL() { return 0; }
    static const RuntimeType::TypeInfo * const * BASE() { return nullptr; }
    static constexpr std::size_t                 SIZE() { return 0; }
};

#define GN_RDG_REGISTER_RUNTIME_TYPE(...)                                                                \
    inline static const uint64_t                TYPE_ID = RuntimeType::getNextUniqueTypeId();            \
    static const RuntimeType::TypeInfo & TYPE_INFO() {                                                   \
        static const RuntimeType::TypeInfo ti = {.id    = TYPE_ID,                                       \
                                                     .name  = GN_FUNCTION,                               \
                                                     .level = RttiBaseTypeIds<__VA_ARGS__>::LEVEL() + 1, \
                                                     .bases = RttiBaseTypeIds<__VA_ARGS__>::BASE(),      \
                                                     .count = RttiBaseTypeIds<__VA_ARGS__>::SIZE()};     \
        return ti;                                                                                       \
    }

} // namespace GN::rdg
