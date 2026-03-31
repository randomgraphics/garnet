#pragma once

/// \file rtti.h
/// Run-time type information for RDG: unique 64-bit IDs, string names, optional base types.
/// Safe across DLL boundaries: getNextUniqueTypeId() and getStableTypeIdFromName() are
/// defined in one TU and exported (GN_API) so all modules share the same counter and name map.

#include <garnet/GNbase.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <tuple>

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
        std::string_view         name  = {};
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

    /// Return the actual type info for current instance.
    const TypeInfo & typeInfo() const { return mTypeInfo; }

    /// Check if the current instance is derived from the given type.
    template<typename T>
    bool isKindOf() const {
        return typeInfo().isDerivedFrom(T::TYPE_INFO());
    }

    template<typename TO, typename FROM>
    static TO * cast(FROM * from) {
        if (!from) return nullptr;
        if (!from->typeInfo().isDerivedFrom(TO::TYPE_INFO())) return nullptr;
        return static_cast<TO *>(from);
    }

    template<typename TO, typename FROM>
    static TO * cast(FROM & from) {
        if (!from.typeInfo().isDerivedFrom(TO::TYPE_INFO())) return nullptr;
        return static_cast<TO *>(&from);
    }

    template<typename TO, typename FROM>
    static const TO * cast(const FROM * from) {
        if (!from) return nullptr;
        if (!from->typeInfo().isDerivedFrom(TO::TYPE_INFO())) return nullptr;
        return static_cast<const TO *>(from);
    }

    template<typename TO, typename FROM>
    static const TO * cast(const FROM & from) {
        if (!from.typeInfo().isDerivedFrom(TO::TYPE_INFO())) return nullptr;
        return static_cast<const TO *>(&from);
    }

    template<typename TO, typename FROM>
    static AutoRef<TO> cast(const AutoRef<FROM> & from) {
        if (!from) return {};
        if (!from->typeInfo().isDerivedFrom(TO::TYPE_INFO())) return {};
        return from.template staticCastTo<TO>();
    }

    /// Returns a new unique 64-bit type ID each call. Thread-safe. Defined in one TU and exported
    /// (GN_API) so the same counter is used across DLL boundaries.
    /// Use for TYPE_ID in GN_REGISTER_RUNTIME_TYPE; do not persist across process runs.
    GN_API static uint64_t getNextUniqueTypeId();

protected:
    const TypeInfo & mTypeInfo;

    RuntimeType(const TypeInfo & typeInfo): mTypeInfo(typeInfo) {}
};

template<typename... BaseTypes>
struct RttiBaseTypeIds {
    /// return max level of all base types.
    static constexpr uint32_t LEVEL() {
        uint32_t level = 0;
        ((level = std::max(level, BaseTypes::TYPE_INFO().level)), ...);
        return level;
    }

    /// Return type of the first base type.
    using FirstBaseType = typename std::tuple_element<0, std::tuple<BaseTypes...>>::type;

    static std::array<const RuntimeType::TypeInfo *, sizeof...(BaseTypes)> BASE() {
        return std::array<const RuntimeType::TypeInfo *, sizeof...(BaseTypes)> {&BaseTypes::TYPE_INFO()...};
    }
};

template<>
struct RttiBaseTypeIds<> {
    static constexpr uint32_t                                     LEVEL() { return 0; }
    static constexpr std::array<const RuntimeType::TypeInfo *, 0> BASE() { return std::array<const RuntimeType::TypeInfo *, 0> {}; }

    using FirstBaseType = RuntimeType;
};

// The main entry point for registering a runtime type.
#define GN_RDG_REGISTER_RUNTIME_TYPE(...)                                                                                                \
    inline static const uint64_t                  TYPE_ID = GN::rdg::RuntimeType::getNextUniqueTypeId();                                 \
    static const GN::rdg::RuntimeType::TypeInfo & TYPE_INFO() {                                                                          \
        static const auto bases = RttiBaseTypeIds<__VA_ARGS__>::BASE();                                                                  \
        static const auto ti    = GN::rdg::RuntimeType::TypeInfo {.id    = TYPE_ID,                                                      \
                                                                  .name  = GN_FUNCTION,                                                  \
                                                                  .level = RttiBaseTypeIds<__VA_ARGS__>::LEVEL() + 1,                    \
                                                                  .bases = bases.data(),                                                 \
                                                                  .count = bases.size()};                                                \
        return ti;                                                                                                                       \
    }                                                                                                                                    \
    const GN::rdg::RuntimeType::TypeInfo & typeInfo() const { return GN::rdg::RttiBaseTypeIds<__VA_ARGS__>::FirstBaseType::typeInfo(); } \
    template<typename DERIVED_TYPE>                                                                                                      \
    bool isKindOf() const {                                                                                                              \
        return typeInfo().isDerivedFrom(DERIVED_TYPE::TYPE_INFO());                                                                      \
    }

} // namespace GN::rdg
