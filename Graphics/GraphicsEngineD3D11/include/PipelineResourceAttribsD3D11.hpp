/*
 *  Copyright 2019-2022 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#pragma once

/// \file
/// Declaration of Diligent::PipelineResourceAttribsD3D11 struct

#include <array>

#include "CommonDefinitions.h"
#include "DebugUtilities.hpp"
#include "HashUtils.hpp"
#include "GraphicsAccessories.hpp"

namespace Diligent
{

enum D3D11_RESOURCE_RANGE : UInt32
{
    D3D11_RESOURCE_RANGE_CBV = 0,
    D3D11_RESOURCE_RANGE_SRV,
    D3D11_RESOURCE_RANGE_SAMPLER,
    D3D11_RESOURCE_RANGE_UAV,
    D3D11_RESOURCE_RANGE_COUNT,
    D3D11_RESOURCE_RANGE_UNKNOWN = ~0u
};


/// Resource binding points in all shader stages.
// sizeof(D3D11ResourceBindPoints) == 8, x64
struct D3D11ResourceBindPoints
{
    /// The number of different shader types (Vertex, Pixel, Geometry, Hull, Domain, Compute)
    static constexpr UInt32 NumShaderTypes = 6;

    D3D11ResourceBindPoints() noexcept
    {
#ifdef DILIGENT_DEBUG
        for (UInt8 BindPoint : Bindings)
            VERIFY_EXPR(BindPoint == InvalidBindPoint);
#endif
    }

    D3D11ResourceBindPoints(const D3D11ResourceBindPoints&) noexcept = default;
    D3D11ResourceBindPoints& operator=(const D3D11ResourceBindPoints&) = default;

    SHADER_TYPE GetActiveStages() const
    {
        return static_cast<SHADER_TYPE>(ActiveStages);
    }

    bool IsEmpty() const
    {
        return GetActiveStages() == SHADER_TYPE_UNKNOWN;
    }

    bool IsStageActive(UInt32 ShaderInd) const
    {
        bool IsActive = (GetActiveStages() & GetShaderTypeFromIndex(ShaderInd)) != 0;
        VERIFY_EXPR((IsActive && Bindings[ShaderInd] != InvalidBindPoint ||
                     !IsActive && Bindings[ShaderInd] == InvalidBindPoint));
        return IsActive;
    }

    UInt8 operator[](UInt32 ShaderInd) const
    {
        VERIFY(IsStageActive(ShaderInd), "Requesting bind point for inactive shader stage.");
        return Bindings[ShaderInd];
    }

    size_t GetHash() const
    {
        size_t Hash = 0;
        for (UInt8 Binding : Bindings)
            HashCombine(Hash, Binding);
        return Hash;
    }

    bool operator==(const D3D11ResourceBindPoints& rhs) const noexcept
    {
        return Bindings == rhs.Bindings;
    }

    D3D11ResourceBindPoints operator+(UInt32 value) const
    {
        D3D11ResourceBindPoints NewBindPoints{*this};
        for (SHADER_TYPE Stages = GetActiveStages(); Stages != SHADER_TYPE_UNKNOWN;)
        {
            Int32 ShaderInd = ExtractFirstShaderStageIndex(Stages);
            VERIFY_EXPR(UInt32{Bindings[ShaderInd]} + value < InvalidBindPoint);
            NewBindPoints.Bindings[ShaderInd] = Bindings[ShaderInd] + static_cast<UInt8>(value);
        }
        return NewBindPoints;
    }

private:
    struct StageAccessor
    {
        StageAccessor(D3D11ResourceBindPoints& _BindPoints,
                      const UInt32             _ShaderInd) :
            BindPoints{_BindPoints},
            ShaderInd{_ShaderInd}
        {}

        // clang-format off
        StageAccessor           (const StageAccessor&)  = delete;
        StageAccessor           (      StageAccessor&&) = delete;
        StageAccessor& operator=(const StageAccessor&)  = delete;
        StageAccessor& operator=(      StageAccessor&&) = delete;
        // clang-format on

        UInt8 operator=(UInt32 BindPoint)
        {
            return BindPoints.Set(ShaderInd, BindPoint);
        }

        operator UInt8() const
        {
            return static_cast<const D3D11ResourceBindPoints&>(BindPoints)[ShaderInd];
        }

    private:
        D3D11ResourceBindPoints& BindPoints;
        const UInt32             ShaderInd;
    };

public:
    StageAccessor operator[](UInt32 ShaderInd)
    {
        return {*this, ShaderInd};
    }

private:
    UInt8 Set(UInt32 ShaderInd, UInt32 BindPoint)
    {
        VERIFY_EXPR(ShaderInd < NumShaderTypes);
        VERIFY(BindPoint < InvalidBindPoint, "Bind point (", BindPoint, ") is out of range.");

        Bindings[ShaderInd] = static_cast<UInt8>(BindPoint);
        ActiveStages |= UInt32{1} << ShaderInd;
        return static_cast<UInt8>(BindPoint);
    }

    static constexpr UInt8 InvalidBindPoint = 0xFF;

    //     0      1      2      3      4      5
    // |  PS  |  VS  |  GS  |  HS  |  DS  |  CS  |
    std::array<UInt8, NumShaderTypes> Bindings{InvalidBindPoint, InvalidBindPoint, InvalidBindPoint, InvalidBindPoint, InvalidBindPoint, InvalidBindPoint};

    UInt16 ActiveStages = 0;
};
SPW_ASSERT_SIZEOF(D3D11ResourceBindPoints, 8, "The struct is used in serialization and must be tightly packed");


/// Shader resource counters for one specific resource range
struct D3D11ResourceRangeCounters
{
    static constexpr UInt32 NumShaderTypes = D3D11ResourceBindPoints::NumShaderTypes;

    UInt8 operator[](UInt32 Stage) const
    {
        VERIFY_EXPR(Stage < NumShaderTypes);
        return (PackedCounters >> (NumBitsPerStage * Stage)) & StageMask;
    }

    D3D11ResourceRangeCounters& operator+=(const D3D11ResourceRangeCounters& rhs)
    {
#ifdef DILIGENT_DEBUG
        for (UInt32 s = 0; s < NumShaderTypes; ++s)
        {
            const UInt32 val0 = static_cast<const D3D11ResourceRangeCounters&>(*this)[s];
            const UInt32 val1 = rhs[s];
            VERIFY(val0 + val1 <= MaxCounter, "The resulting value (", val0 + val1, ") is out of range.");
        }
#endif
        PackedCounters += rhs.PackedCounters;
        return *this;
    }

    bool operator==(const D3D11ResourceRangeCounters& rhs) const noexcept
    {
        return PackedCounters == rhs.PackedCounters;
    }

private:
    struct StageAccessor
    {
        StageAccessor(D3D11ResourceRangeCounters& _Counters,
                      const UInt32                _ShaderInd) :
            Counters{_Counters},
            ShaderInd{_ShaderInd}
        {}

        // clang-format off
        StageAccessor           (const StageAccessor&)  = delete;
        StageAccessor           (      StageAccessor&&) = delete;
        StageAccessor& operator=(const StageAccessor&)  = delete;
        StageAccessor& operator=(      StageAccessor&&) = delete;
        // clang-format on

        UInt8 operator=(UInt32 Counter)
        {
            return Counters.Set(ShaderInd, Counter);
        }

        UInt8 operator+=(UInt32 Val)
        {
            UInt32 CurrValue = static_cast<const D3D11ResourceRangeCounters&>(Counters)[ShaderInd];
            return Counters.Set(ShaderInd, CurrValue + Val);
        }

        operator UInt8() const
        {
            return static_cast<const D3D11ResourceRangeCounters&>(Counters)[ShaderInd];
        }

    private:
        D3D11ResourceRangeCounters& Counters;
        const UInt32                ShaderInd;
    };

public:
    StageAccessor operator[](UInt32 ShaderInd)
    {
        return {*this, ShaderInd};
    }

private:
    UInt8 Set(UInt32 ShaderInd, UInt32 Counter)
    {
        VERIFY_EXPR(Counter <= MaxCounter);
        const UInt64 BitOffset = NumBitsPerStage * ShaderInd;
        PackedCounters &= ~(StageMask << BitOffset);
        PackedCounters |= UInt64{Counter} << BitOffset;
        return static_cast<UInt8>(Counter);
    }

    static constexpr UInt64 NumBitsPerStage = 8;
    static constexpr UInt64 StageMask       = (UInt64{1} << NumBitsPerStage) - 1;
    static constexpr UInt32 MaxCounter      = (UInt32{1} << NumBitsPerStage) - 1;

    // 0      1      2      3      4      5      6      7      8
    // |  VS  |  PS  |  GS  |  HS  |  DS  |  CS  |unused|unused|
    union
    {
        UInt64 PackedCounters = 0;
        struct
        {
            UInt64 VS : NumBitsPerStage;
            UInt64 PS : NumBitsPerStage;
            UInt64 GS : NumBitsPerStage;
            UInt64 HS : NumBitsPerStage;
            UInt64 DS : NumBitsPerStage;
            UInt64 CS : NumBitsPerStage;
        } _Packed;
    };
};

/// Resource counters for all shader stages and all resource types
using D3D11ShaderResourceCounters = std::array<D3D11ResourceRangeCounters, D3D11_RESOURCE_RANGE_COUNT>;


// sizeof(PipelineResourceAttribsD3D11) == 12, x64
struct PipelineResourceAttribsD3D11
{
private:
    static constexpr UInt32 _SamplerIndBits      = 31;
    static constexpr UInt32 _SamplerAssignedBits = 1;

public:
    static constexpr UInt32 InvalidSamplerInd = (1u << _SamplerIndBits) - 1;

    // clang-format off
    const UInt32 SamplerInd           : _SamplerIndBits;       // Index of the assigned sampler in m_Desc.Resources.
    const UInt32 ImtblSamplerAssigned : _SamplerAssignedBits;  // Immutable sampler flag for Texture SRV or Sampler.
    // clang-format on
    const D3D11ResourceBindPoints BindPoints;

    PipelineResourceAttribsD3D11(const D3D11ResourceBindPoints& _BindPoints,
                                 UInt32                         _SamplerInd,
                                 bool                           _ImtblSamplerAssigned) noexcept :
        // clang-format off
        SamplerInd          {_SamplerInd                    },
        ImtblSamplerAssigned{_ImtblSamplerAssigned ? 1u : 0u},
        BindPoints          {_BindPoints                    }
    // clang-format on
    {
        VERIFY(SamplerInd == _SamplerInd, "Sampler index (", _SamplerInd, ") exceeds maximum representable value.");
    }

    // Only for serialization
    PipelineResourceAttribsD3D11() noexcept :
        PipelineResourceAttribsD3D11{{}, 0, false}
    {}

    bool IsSamplerAssigned() const { return SamplerInd != InvalidSamplerInd; }
    bool IsImmutableSamplerAssigned() const { return ImtblSamplerAssigned != 0; }

    bool IsCompatibleWith(const PipelineResourceAttribsD3D11& rhs) const
    {
        // Ignore assigned sampler index.
        // clang-format off
        return IsImmutableSamplerAssigned() == rhs.IsImmutableSamplerAssigned() &&
               BindPoints                   == rhs.BindPoints;
        // clang-format on
    }

    size_t GetHash() const
    {
        return ComputeHash(IsImmutableSamplerAssigned(), BindPoints.GetHash());
    }
};
SPW_ASSERT_SIZEOF(PipelineResourceAttribsD3D11, 12, "The struct is used in serialization and must be tightly packed");

} // namespace Diligent
