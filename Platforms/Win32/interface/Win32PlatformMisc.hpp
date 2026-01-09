#pragma once

#include "../../Basic/interface/BasicPlatformMisc.hpp"
#include "../../../Platforms/Basic/interface/DebugUtilities.hpp"

#include <intrin.h>

namespace Diligent
{

struct WindowsMisc : public BasicPlatformMisc
{
    inline static UInt32 GetMSB(UInt32 Val)
    {
        if (Val == 0) return 32;

        unsigned long MSB = 32;
        _BitScanReverse(&MSB, Val);
        VERIFY_EXPR(MSB == BasicPlatformMisc::GetMSB(Val));

        return MSB;
    }

    inline static UInt32 GetMSB(UInt64 Val)
    {
        if (Val == 0) return 64;

        unsigned long MSB = 64;
#if _WIN64
        _BitScanReverse64(&MSB, Val);
#else
        UInt32 high = static_cast<UInt32>((Val >> 32) & 0xFFFFFFFF);
        if (high != 0)
        {
            MSB = 32 + GetMSB(high);
        }
        else
        {
            UInt32 low = static_cast<UInt32>(Val & 0xFFFFFFFF);
            VERIFY_EXPR(low != 0);
            MSB = GetMSB(low);
        }
#endif
        VERIFY_EXPR(MSB == BasicPlatformMisc::GetMSB(Val));

        return MSB;
    }

    inline static UInt32 GetLSB(UInt32 Val)
    {
        if (Val == 0) return 32;

        unsigned long LSB = 32;
        _BitScanForward(&LSB, Val);
        VERIFY_EXPR(LSB == BasicPlatformMisc::GetLSB(Val));

        return LSB;
    }

    inline static UInt32 GetLSB(UInt64 Val)
    {
        if (Val == 0) return 64;

        unsigned long LSB = 64;
#if _WIN64
        _BitScanForward64(&LSB, Val);
#else
        UInt32 low = static_cast<UInt32>(Val & 0xFFFFFFFF);
        if (low != 0)
        {
            LSB = GetLSB(low);
        }
        else
        {
            UInt32 high = static_cast<UInt32>((Val >> 32) & 0xFFFFFFFF);
            VERIFY_EXPR(high != 0);
            LSB = 32 + GetLSB(high);
        }
#endif

        VERIFY_EXPR(LSB == BasicPlatformMisc::GetLSB(Val));
        return LSB;
    }

    inline static UInt32 CountOneBits(UInt32 Val)
    {
#if defined _M_ARM || defined _M_ARM64
        // MSVC _CountOneBits intrinsics undefined for ARM64
        // Cast bits to 8x8 datatype and use VCNT on result
        const uint8x8_t Vsum = vcnt_u8(vcreate_u8(static_cast<uint64_t>(Val)));
        // Pairwise sums: 8x8 -> 16x4 -> 32x2
        auto Bits = static_cast<UInt32>(vget_lane_u32(vpaddl_u16(vpaddl_u8(Vsum)), 0));
#else
        auto Bits = __popcnt(Val);
#endif
        VERIFY_EXPR(Bits == BasicPlatformMisc::CountOneBits(Val));
        return Bits;
    }

    inline static UInt32 CountOneBits(UInt64 Val)
    {
#if defined _M_ARM || defined _M_ARM64
        // Cast bits to 8x8 datatype and use VCNT on result
        const uint8x8_t Vsum = vcnt_u8(vcreate_u8(Val));
        // Pairwise sums: 8x8 -> 16x4 -> 32x2 -> 64x1
        auto Bits = static_cast<UInt32>(vget_lane_u64(vpaddl_u32(vpaddl_u16(vpaddl_u8(Vsum))), 0));
#elif _WIN64
        auto Bits = __popcnt64(Val);
#else
        auto Bits =
            CountOneBits(static_cast<UInt32>((Val >> 0) & 0xFFFFFFFF)) +
            CountOneBits(static_cast<UInt32>((Val >> 32) & 0xFFFFFFFF));
#endif
        VERIFY_EXPR(Bits == BasicPlatformMisc::CountOneBits(Val));
        return static_cast<UInt32>(Bits);
    }

    template <typename Type>
    static typename std::enable_if<sizeof(Type) == 2, Type>::type SwapBytes(Type Val)
    {
        auto SwappedBytes = _byteswap_ushort(reinterpret_cast<unsigned short&>(Val));
        return reinterpret_cast<const Type&>(SwappedBytes);
    }

    template <typename Type>
    static typename std::enable_if<sizeof(Type) == 4, Type>::type SwapBytes(Type Val)
    {
        auto SwappedBytes = _byteswap_ulong(reinterpret_cast<unsigned long&>(Val));
        return reinterpret_cast<const Type&>(SwappedBytes);
    }

    template <typename Type>
    static typename std::enable_if<sizeof(Type) == 8, Type>::type SwapBytes(Type Val)
    {
        auto SwappedBytes = _byteswap_uint64(reinterpret_cast<unsigned long long&>(Val));
        return reinterpret_cast<const Type&>(SwappedBytes);
    }

#if PLATFORM_WIN32
    /// Sets the current thread affinity mask and on success returns the previous mask.
    /// On failure, returns 0.
    static UInt64 SetCurrentThreadAffinity(UInt64 Mask);

    static ThreadPriority GetCurrentThreadPriority();

    /// Sets the current thread priority and on success returns the previous priority.
    /// On failure, returns ThreadPriority::Unknown.
    static ThreadPriority SetCurrentThreadPriority(ThreadPriority Priority);

    /// Sets the name of the current thread.
    static void SetCurrentThreadName(const char* Name);
#endif
};

} // namespace Diligent
