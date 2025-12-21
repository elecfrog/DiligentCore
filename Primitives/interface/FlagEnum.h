#pragma once

#include "CommonDefinitions.h"

#if DILIGENT_C_INTERFACE

#    define DEFINE_FLAG_ENUM_OPERATORS(ENUMTYPE)

#else


template <typename EnumType>
using _UNDERLYING_ENUM_T = typename std::underlying_type<EnumType>::type;

#    define DEFINE_FLAG_ENUM_OPERATORS(ENUMTYPE)                                                                                                                                                                      \
        extern "C++"                                                                                                                                                                                                  \
        {                                                                                                                                                                                                             \
            inline ENUMTYPE&          operator|=(ENUMTYPE& a, ENUMTYPE b) { return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<_UNDERLYING_ENUM_T<ENUMTYPE>&>(a) |= static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(b)); } \
            inline ENUMTYPE&          operator&=(ENUMTYPE& a, ENUMTYPE b) { return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<_UNDERLYING_ENUM_T<ENUMTYPE>&>(a) &= static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(b)); } \
            inline ENUMTYPE&          operator^=(ENUMTYPE& a, ENUMTYPE b) { return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<_UNDERLYING_ENUM_T<ENUMTYPE>&>(a) ^= static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(b)); } \
            inline constexpr ENUMTYPE operator|(ENUMTYPE a, ENUMTYPE b) { return static_cast<ENUMTYPE>(static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(a) | static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(b)); }                \
            inline constexpr ENUMTYPE operator&(ENUMTYPE a, ENUMTYPE b) { return static_cast<ENUMTYPE>(static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(a) & static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(b)); }                \
            inline constexpr ENUMTYPE operator^(ENUMTYPE a, ENUMTYPE b) { return static_cast<ENUMTYPE>(static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(a) ^ static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(b)); }                \
            inline constexpr ENUMTYPE operator~(ENUMTYPE a) { return static_cast<ENUMTYPE>(~static_cast<_UNDERLYING_ENUM_T<ENUMTYPE>>(a)); }                                                                          \
        }

#    define DECLARE_FRIEND_FLAG_ENUM_OPERATORS(ENUMTYPE)               \
        friend ENUMTYPE&          operator|=(ENUMTYPE& a, ENUMTYPE b); \
        friend ENUMTYPE&          operator&=(ENUMTYPE& a, ENUMTYPE b); \
        friend ENUMTYPE&          operator^=(ENUMTYPE& a, ENUMTYPE b); \
        friend constexpr ENUMTYPE operator|(ENUMTYPE a, ENUMTYPE b);   \
        friend constexpr ENUMTYPE operator&(ENUMTYPE a, ENUMTYPE b);   \
        friend constexpr ENUMTYPE operator^(ENUMTYPE a, ENUMTYPE b);   \
        friend constexpr ENUMTYPE operator~(ENUMTYPE a);

#endif
