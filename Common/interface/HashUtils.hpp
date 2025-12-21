/*
 *  Copyright 2019-2025 Diligent Graphics LLC
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

#include <functional>
#include <memory>
#include <cstring>
#include <algorithm>

#include "../../Primitives/interface/Errors.hpp"
#include "../../Platforms/Basic/interface/DebugUtilities.hpp"
#include "../../Graphics/GraphicsEngine/interface/Sampler.h"
#include "../../Graphics/GraphicsEngine/interface/RasterizerState.h"
#include "../../Graphics/GraphicsEngine/interface/DepthStencilState.h"
#include "../../Graphics/GraphicsEngine/interface/BlendState.h"
#include "../../Graphics/GraphicsEngine/interface/TextureView.h"
#include "../../Graphics/GraphicsEngine/interface/PipelineResourceSignature.h"
#include "../../Graphics/GraphicsEngine/interface/PipelineState.h"
#include "../../Graphics/GraphicsTools/interface/VertexPool.h"
#include "../../Common/interface/RefCntAutoPtr.hpp"
#include <math/hash.hpp>

#define LOG_HASH_CONFLICTS 1

namespace Diligent
{

template <typename HasherType, typename Type>
struct HashCombiner;

template <typename HasherType>
struct HashCombiner<HasherType, SamplerDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const SamplerDesc& SamDesc) const
    {
        ASSERT_SIZEOF(SamDesc.MinFilter, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SamDesc.MagFilter, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SamDesc.MipFilter, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SamDesc.AddressU, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SamDesc.AddressV, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SamDesc.AddressW, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SamDesc.Flags, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SamDesc.BorderColor, 16, "Hash logic below may be incorrect.");

        // Ignore Name. This is consistent with the operator==
        this->m_Hasher( // SamDesc.Name,
            ((static_cast<uint32_t>(SamDesc.MinFilter) << 0u) |
             (static_cast<uint32_t>(SamDesc.MagFilter) << 8u) |
             (static_cast<uint32_t>(SamDesc.MipFilter) << 24u)),
            ((static_cast<uint32_t>(SamDesc.AddressU) << 0u) |
             (static_cast<uint32_t>(SamDesc.AddressV) << 8u) |
             (static_cast<uint32_t>(SamDesc.AddressW) << 24u)),
            ((static_cast<uint32_t>(SamDesc.Flags) << 0u) |
             ((SamDesc.UnnormalizedCoords ? 1u : 0u) << 8u)),
            SamDesc.MipLODBias,
            SamDesc.MaxAnisotropy,
            static_cast<uint32_t>(SamDesc.ComparisonFunc),
            SamDesc.BorderColor[0],
            SamDesc.BorderColor[1],
            SamDesc.BorderColor[2],
            SamDesc.BorderColor[3],
            SamDesc.MinLOD,
            SamDesc.MaxLOD);
        ASSERT_SIZEOF64(SamDesc, 56, "Did you add new members to SamplerDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, StencilOpDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const StencilOpDesc& StOpDesc) const
    {
        // clang-format off
        ASSERT_SIZEOF(StOpDesc.StencilFailOp,      1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(StOpDesc.StencilDepthFailOp, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(StOpDesc.StencilPassOp,      1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(StOpDesc.StencilFunc,        1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            ((static_cast<uint32_t>(StOpDesc.StencilFailOp)      <<  0u) |
             (static_cast<uint32_t>(StOpDesc.StencilDepthFailOp) <<  8u) |
             (static_cast<uint32_t>(StOpDesc.StencilPassOp)      << 16u) |
             (static_cast<uint32_t>(StOpDesc.StencilFunc)        << 24u)));
        // clang-format on
        ASSERT_SIZEOF(StOpDesc, 4, "Did you add new members to StencilOpDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, DepthStencilStateDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const DepthStencilStateDesc& DSSDesc) const
    {
        ASSERT_SIZEOF(DSSDesc.DepthFunc, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(DSSDesc.StencilReadMask, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(DSSDesc.StencilWriteMask, 1, "Hash logic below may be incorrect.");
        // clang-format off
        this->m_Hasher(
            (((DSSDesc.DepthEnable      ? 1u : 0u) << 0u) |
             ((DSSDesc.DepthWriteEnable ? 1u : 0u) << 1u) |
             ((DSSDesc.StencilEnable    ? 1u : 0u) << 2u) |
             (static_cast<uint32_t>(DSSDesc.DepthFunc)        << 8u)  |
             (static_cast<uint32_t>(DSSDesc.StencilReadMask)  << 16u) |
             (static_cast<uint32_t>(DSSDesc.StencilWriteMask) << 24u)),
            DSSDesc.FrontFace,
            DSSDesc.BackFace);
        // clang-format on
        ASSERT_SIZEOF(DSSDesc, 14, "Did you add new members to DepthStencilStateDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, RasterizerStateDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const RasterizerStateDesc& RSDesc) const
    {
        ASSERT_SIZEOF(RSDesc.FillMode, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(RSDesc.CullMode, 1, "Hash logic below may be incorrect.");

        // clang-format off
        this->m_Hasher(
            ((static_cast<uint32_t>(RSDesc.FillMode) << 0u) |
             (static_cast<uint32_t>(RSDesc.CullMode) << 8u) |
             ((RSDesc.FrontCounterClockwise ? 1u : 0u) << 16u) |
             ((RSDesc.DepthClipEnable       ? 1u : 0u) << 17u) |
             ((RSDesc.ScissorEnable         ? 1u : 0u) << 18u) |
             ((RSDesc.AntialiasedLineEnable ? 1u : 0u) << 19u)),
            RSDesc.DepthBias,
            RSDesc.DepthBiasClamp,
            RSDesc.SlopeScaledDepthBias);
        // clang-format on
        ASSERT_SIZEOF(RSDesc, 20, "Did you add new members to RasterizerStateDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, BlendStateDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const BlendStateDesc& BSDesc) const
    {
        for (size_t i = 0; i < MAX_RENDER_TARGETS; ++i)
        {
            const RenderTargetBlendDesc& rt = BSDesc.RenderTargets[i];

            ASSERT_SIZEOF(rt.SrcBlend, 1, "Hash logic below may be incorrect.");
            ASSERT_SIZEOF(rt.DestBlend, 1, "Hash logic below may be incorrect.");
            ASSERT_SIZEOF(rt.BlendOp, 1, "Hash logic below may be incorrect.");
            ASSERT_SIZEOF(rt.SrcBlendAlpha, 1, "Hash logic below may be incorrect.");
            ASSERT_SIZEOF(rt.DestBlendAlpha, 1, "Hash logic below may be incorrect.");
            ASSERT_SIZEOF(rt.BlendOpAlpha, 1, "Hash logic below may be incorrect.");
            ASSERT_SIZEOF(rt.LogicOp, 1, "Hash logic below may be incorrect.");
            ASSERT_SIZEOF(rt.RenderTargetWriteMask, 1, "Hash logic below may be incorrect.");

            // clang-format off
            this->m_Hasher(
                (((rt.BlendEnable          ? 1u : 0u) <<  0u) |
                 ((rt.LogicOperationEnable ? 1u : 0u) <<  1u) |
                 (static_cast<uint32_t>(rt.SrcBlend)  <<  8u) |
                 (static_cast<uint32_t>(rt.DestBlend) << 16u) |
                 (static_cast<uint32_t>(rt.BlendOp)   << 24u)),
                ((static_cast<uint32_t>(rt.SrcBlendAlpha)  <<  0u) |
                 (static_cast<uint32_t>(rt.DestBlendAlpha) <<  8u) |
                 (static_cast<uint32_t>(rt.BlendOpAlpha)   << 16u) |
                 (static_cast<uint32_t>(rt.LogicOp)        << 24u)),
                rt.RenderTargetWriteMask);
            // clang-format on
        }
        this->m_Hasher(
            (((BSDesc.AlphaToCoverageEnable ? 1u : 0u) << 0u) |
             ((BSDesc.IndependentBlendEnable ? 1u : 0u) << 1u)));

        ASSERT_SIZEOF(BSDesc, 82, "Did you add new members to BlendStateDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, TextureViewDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const TextureViewDesc& TexViewDesc) const
    {
        ASSERT_SIZEOF(TexViewDesc.ViewType, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(TexViewDesc.TextureDim, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(TexViewDesc.Format, 2, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(TexViewDesc.AccessFlags, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(TexViewDesc.Flags, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(TexViewDesc.Swizzle, 4, "Hash logic below may be incorrect.");

        // Ignore Name. This is consistent with the operator==
        this->m_Hasher(
            ((static_cast<uint32_t>(TexViewDesc.ViewType) << 0u) |
             (static_cast<uint32_t>(TexViewDesc.TextureDim) << 8u) |
             (static_cast<uint32_t>(TexViewDesc.Format) << 16u)),
            TexViewDesc.MostDetailedMip,
            TexViewDesc.NumMipLevels,
            TexViewDesc.FirstArraySlice,
            TexViewDesc.NumArraySlices,
            ((static_cast<uint32_t>(TexViewDesc.AccessFlags) << 0u) |
             (static_cast<uint32_t>(TexViewDesc.Flags) << 8u)),
            TexViewDesc.Swizzle.AsUint32());
        ASSERT_SIZEOF64(TexViewDesc, 40, "Did you add new members to TextureViewDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, SampleDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const SampleDesc& SmplDesc) const
    {
        ASSERT_SIZEOF(SmplDesc.Count, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(SmplDesc.Quality, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            ((static_cast<uint32_t>(SmplDesc.Count) << 0u) |
             (static_cast<uint32_t>(SmplDesc.Quality) << 8u)));
        ASSERT_SIZEOF(SmplDesc, 2, "Did you add new members to SampleDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, ShaderResourceVariableDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const ShaderResourceVariableDesc& VarDesc) const
    {
        ASSERT_SIZEOF(VarDesc.Type, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(VarDesc.Flags, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            VarDesc.Name,
            VarDesc.ShaderStages,
            ((static_cast<uint32_t>(VarDesc.Type) << 0u) |
             (static_cast<uint32_t>(VarDesc.Flags) << 8u)));
        ASSERT_SIZEOF64(VarDesc, 16, "Did you add new members to ShaderResourceVariableDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, ImmutableSamplerDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const ImmutableSamplerDesc& SamDesc) const
    {
        this->m_Hasher(SamDesc.ShaderStages, SamDesc.SamplerOrTextureName, SamDesc.Desc);
        ASSERT_SIZEOF64(SamDesc, 16 + sizeof(SamplerDesc), "Did you add new members to ImmutableSamplerDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, PipelineResourceDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const PipelineResourceDesc& ResDesc) const
    {
        ASSERT_SIZEOF(ResDesc.ResourceType, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(ResDesc.VarType, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(ResDesc.Flags, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            ResDesc.Name,
            ResDesc.ShaderStages,
            ResDesc.ArraySize,
            ((static_cast<uint32_t>(ResDesc.ResourceType) << 0u) |
             (static_cast<uint32_t>(ResDesc.VarType) << 8u) |
             (static_cast<uint32_t>(ResDesc.Flags) << 16u)));
        ASSERT_SIZEOF64(ResDesc, 24, "Did you add new members to PipelineResourceDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, PipelineResourceLayoutDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const PipelineResourceLayoutDesc& LayoutDesc) const
    {
        this->m_Hasher(
            LayoutDesc.DefaultVariableType,
            LayoutDesc.DefaultVariableMergeStages,
            LayoutDesc.NumVariables,
            LayoutDesc.NumImmutableSamplers);

        if (LayoutDesc.Variables != nullptr)
        {
            for (size_t i = 0; i < LayoutDesc.NumVariables; ++i)
                this->m_Hasher(LayoutDesc.Variables[i]);
        }
        else
        {
            VERIFY_EXPR(LayoutDesc.NumVariables == 0);
        }

        if (LayoutDesc.ImmutableSamplers != nullptr)
        {
            for (size_t i = 0; i < LayoutDesc.NumImmutableSamplers; ++i)
                this->m_Hasher(LayoutDesc.ImmutableSamplers[i]);
        }
        else
        {
            VERIFY_EXPR(LayoutDesc.NumImmutableSamplers == 0);
        }

        ASSERT_SIZEOF64(LayoutDesc, 40, "Did you add new members to PipelineResourceDesc? Please handle them here.");
    }
};



template <typename HasherType>
struct HashCombiner<HasherType, RenderPassAttachmentDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const RenderPassAttachmentDesc& Desc) const
    {
        ASSERT_SIZEOF(Desc.Format, 2, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.SampleCount, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.LoadOp, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.StoreOp, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.StencilLoadOp, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.StencilStoreOp, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            ((static_cast<uint32_t>(Desc.Format) << 0u) |
             (static_cast<uint32_t>(Desc.SampleCount) << 16u) |
             (static_cast<uint32_t>(Desc.LoadOp) << 24u)),
            ((static_cast<uint32_t>(Desc.StoreOp) << 0u) |
             (static_cast<uint32_t>(Desc.StencilLoadOp) << 8u) |
             (static_cast<uint32_t>(Desc.StencilStoreOp) << 16u)),
            Desc.InitialState,
            Desc.FinalState);
        ASSERT_SIZEOF(Desc, 16, "Did you add new members to RenderPassAttachmentDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, AttachmentReference> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const AttachmentReference& Ref) const
    {
        this->m_Hasher(Ref.AttachmentIndex, Ref.State);
        ASSERT_SIZEOF(Ref, 8, "Did you add new members to AttachmentReference? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, ShadingRateAttachment> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const ShadingRateAttachment& SRA) const
    {
        ASSERT_SIZEOF(SRA.TileSize, 8, "Hash logic below may be incorrect.");
        this->m_Hasher(SRA.Attachment, SRA.TileSize[0], SRA.TileSize[1]);
        ASSERT_SIZEOF(SRA, 16, "Did you add new members to AttachmentReference? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, SubpassDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const SubpassDesc& Subpass) const
    {
        this->m_Hasher(
            Subpass.InputAttachmentCount,
            Subpass.RenderTargetAttachmentCount,
            Subpass.PreserveAttachmentCount);

        if (Subpass.pInputAttachments != nullptr)
        {
            for (size_t i = 0; i < Subpass.InputAttachmentCount; ++i)
                this->m_Hasher(Subpass.pInputAttachments[i]);
        }
        else
        {
            VERIFY_EXPR(Subpass.InputAttachmentCount == 0);
        }

        if (Subpass.pRenderTargetAttachments != nullptr)
        {
            for (size_t i = 0; i < Subpass.RenderTargetAttachmentCount; ++i)
                this->m_Hasher(Subpass.pRenderTargetAttachments[i]);
        }
        else
        {
            VERIFY_EXPR(Subpass.RenderTargetAttachmentCount == 0);
        }

        if (Subpass.pResolveAttachments != nullptr)
        {
            for (size_t i = 0; i < Subpass.RenderTargetAttachmentCount; ++i)
                this->m_Hasher(Subpass.pResolveAttachments[i]);
        }

        if (Subpass.pDepthStencilAttachment)
            this->m_Hasher(*Subpass.pDepthStencilAttachment);

        if (Subpass.pPreserveAttachments != nullptr)
        {
            for (size_t i = 0; i < Subpass.PreserveAttachmentCount; ++i)
                this->m_Hasher(Subpass.pPreserveAttachments[i]);
        }
        else
        {
            VERIFY_EXPR(Subpass.PreserveAttachmentCount == 0);
        }

        if (Subpass.pShadingRateAttachment)
            this->m_Hasher(*Subpass.pShadingRateAttachment);

        ASSERT_SIZEOF64(Subpass, 72, "Did you add new members to SubpassDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, SubpassDependencyDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const SubpassDependencyDesc& Dep) const
    {
        this->m_Hasher(
            Dep.SrcSubpass,
            Dep.DstSubpass,
            Dep.SrcStageMask,
            Dep.DstStageMask,
            Dep.SrcAccessMask,
            Dep.DstAccessMask);
        ASSERT_SIZEOF(Dep, 24, "Did you add new members to SubpassDependencyDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, RenderPassDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const RenderPassDesc& RP) const
    {
        // Ignore Name. This is consistent with the operator==
        this->m_Hasher(
            RP.AttachmentCount,
            RP.SubpassCount,
            RP.DependencyCount);

        if (RP.pAttachments != nullptr)
        {
            for (size_t i = 0; i < RP.AttachmentCount; ++i)
                this->m_Hasher(RP.pAttachments[i]);
        }
        else
        {
            VERIFY_EXPR(RP.AttachmentCount == 0);
        }

        if (RP.pSubpasses != nullptr)
        {
            for (size_t i = 0; i < RP.SubpassCount; ++i)
                this->m_Hasher(RP.pSubpasses[i]);
        }
        else
        {
            VERIFY_EXPR(RP.SubpassCount == 0);
        }

        if (RP.pDependencies != nullptr)
        {
            for (size_t i = 0; i < RP.DependencyCount; ++i)
                this->m_Hasher(RP.pDependencies[i]);
        }
        else
        {
            VERIFY_EXPR(RP.DependencyCount == 0);
        }

        ASSERT_SIZEOF64(RP, 56, "Did you add new members to RenderPassDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, LayoutElement> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const LayoutElement& Elem) const
    {
        ASSERT_SIZEOF(Elem.ValueType, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Elem.IsNormalized, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Elem.Frequency, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            Elem.HLSLSemantic,
            Elem.InputIndex,
            Elem.BufferSlot,
            Elem.NumComponents,
            ((static_cast<uint32_t>(Elem.ValueType) << 0u) |
             ((Elem.IsNormalized ? 1u : 0u) << 8u) |
             (static_cast<uint32_t>(Elem.Frequency) << 16u)),
            Elem.RelativeOffset,
            Elem.Stride,
            Elem.InstanceDataStepRate);
        ASSERT_SIZEOF64(Elem, 40, "Did you add new members to LayoutElement? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, InputLayoutDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const InputLayoutDesc& Layout) const
    {
        this->m_Hasher(Layout.NumElements);
        if (Layout.LayoutElements != nullptr)
        {
            for (size_t i = 0; i < Layout.NumElements; ++i)
                this->m_Hasher(Layout.LayoutElements[i]);
        }
        else
        {
            VERIFY_EXPR(Layout.NumElements == 0);
        }

        ASSERT_SIZEOF64(Layout, 16, "Did you add new members to InputLayoutDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, GraphicsPipelineDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const GraphicsPipelineDesc& Desc) const
    {
        ASSERT_SIZEOF(Desc.NumViewports, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.NumRenderTargets, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.SubpassIndex, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.ShadingRateFlags, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            Desc.BlendDesc,
            Desc.SampleMask,
            Desc.RasterizerDesc,
            Desc.DepthStencilDesc,
            Desc.InputLayout,
            Desc.PrimitiveTopology,
            Desc.NumViewports,
            ((static_cast<uint32_t>(Desc.NumViewports) << 0u) |
             (static_cast<uint32_t>(Desc.NumRenderTargets) << 8u) |
             (static_cast<uint32_t>(Desc.SubpassIndex) << 16u) |
             (static_cast<uint32_t>(Desc.ShadingRateFlags) << 24u)));

        for (size_t i = 0; i < Desc.NumRenderTargets; ++i)
            this->m_Hasher(Desc.RTVFormats[i]);

        this->m_Hasher(Desc.DSVFormat,
                       Desc.SmplDesc,
                       Desc.NodeMask);

        if (Desc.pRenderPass != nullptr)
            this->m_Hasher(Desc.pRenderPass->GetDesc());
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, RayTracingPipelineDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const RayTracingPipelineDesc& Desc) const
    {
        ASSERT_SIZEOF(Desc.ShaderRecordSize, 2, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.MaxRecursionDepth, 1, "Hash logic below may be incorrect.");
        this->m_Hasher(
            ((static_cast<uint32_t>(Desc.ShaderRecordSize) << 0u) |
             (static_cast<uint32_t>(Desc.MaxRecursionDepth) << 16u)));
        ASSERT_SIZEOF(Desc, 4, "Did you add new members to RayTracingPipelineDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, PipelineStateDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const PipelineStateDesc& Desc) const
    {
        // Ignore Name. This is consistent with the operator==
        this->m_Hasher(
            Desc.PipelineType,
            Desc.SRBAllocationGranularity,
            Desc.ImmediateContextMask,
            Desc.ResourceLayout);
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, PipelineResourceSignatureDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const PipelineResourceSignatureDesc& Desc) const
    {
        ASSERT_SIZEOF(Desc.BindingIndex, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.UseCombinedTextureSamplers, 1, "Hash logic below may be incorrect.");
        // Ignore Name. This is consistent with the operator==
        this->m_Hasher(
            Desc.NumResources,
            Desc.NumImmutableSamplers,
            ((static_cast<uint32_t>(Desc.BindingIndex) << 0u) |
             (static_cast<uint32_t>(Desc.UseCombinedTextureSamplers) << 8u)),
            Desc.SRBAllocationGranularity);

        if (Desc.Resources != nullptr)
        {
            for (size_t i = 0; i < Desc.NumResources; ++i)
                this->m_Hasher(Desc.Resources[i]);
        }
        else
        {
            VERIFY_EXPR(Desc.NumResources == 0);
        }

        if (Desc.ImmutableSamplers != nullptr)
        {
            for (size_t i = 0; i < Desc.NumImmutableSamplers; ++i)
                this->m_Hasher(Desc.ImmutableSamplers[i]);
        }
        else
        {
            VERIFY_EXPR(Desc.NumImmutableSamplers == 0);
        }

        if (Desc.UseCombinedTextureSamplers)
            this->m_Hasher(Desc.CombinedSamplerSuffix);

        ASSERT_SIZEOF64(Desc, 56, "Did you add new members to PipelineResourceSignatureDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, ShaderDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const ShaderDesc& Desc) const
    {
        // Ignore Name. This is consistent with the operator==
        this->m_Hasher(
            Desc.ShaderType,
            Desc.UseCombinedTextureSamplers,
            Desc.CombinedSamplerSuffix);

        ASSERT_SIZEOF64(Desc, 24, "Did you add new members to ShaderDesc? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, Version> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const Version& Ver) const
    {
        this->m_Hasher(Ver.Minor, Ver.Major);
        ASSERT_SIZEOF64(Ver, 8, "Did you add new members to Version? Please handle them here.");
    }
};


template <typename HasherType>
struct HashCombiner<HasherType, PipelineStateCreateInfo> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const PipelineStateCreateInfo& CI) const
    {
        this->m_Hasher(
            CI.PSODesc,
            CI.Flags,
            CI.ResourceSignaturesCount);
        if (CI.ppResourceSignatures != nullptr)
        {
            for (size_t i = 0; i < CI.ResourceSignaturesCount; ++i)
            {
                if (const IPipelineResourceSignature* pSign = CI.ppResourceSignatures[i])
                {
                    this->m_Hasher(pSign->GetDesc());
                }
            }
        }
        else
        {
            VERIFY_EXPR(CI.ResourceSignaturesCount == 0);
        }
    }
};

template <typename HasherType>
void HashShaderBytecode(HasherType& Hasher, IShader* pShader)
{
    if (pShader == nullptr)
        return;

    const void* pBytecode = nullptr;
    UInt64      Size      = 0;
    pShader->GetBytecode(&pBytecode, Size);
    VERIFY_EXPR(pBytecode != nullptr && Size != 0);
    Hasher.UpdateRaw(pBytecode, static_cast<size_t>(Size));
}

template <typename HasherType>
struct HashCombiner<HasherType, GraphicsPipelineStateCreateInfo> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const GraphicsPipelineStateCreateInfo& CI) const
    {
        this->m_Hasher(
            static_cast<const PipelineStateCreateInfo&>(CI),
            CI.GraphicsPipeline);
        HashShaderBytecode(this->m_Hasher, CI.pVS);
        HashShaderBytecode(this->m_Hasher, CI.pPS);
        HashShaderBytecode(this->m_Hasher, CI.pDS);
        HashShaderBytecode(this->m_Hasher, CI.pHS);
        HashShaderBytecode(this->m_Hasher, CI.pGS);
        HashShaderBytecode(this->m_Hasher, CI.pAS);
        HashShaderBytecode(this->m_Hasher, CI.pMS);
    }
};

template <typename HasherType>
struct HashCombiner<HasherType, ComputePipelineStateCreateInfo> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const ComputePipelineStateCreateInfo& CI) const
    {
        this->m_Hasher(static_cast<const PipelineStateCreateInfo&>(CI));
        HashShaderBytecode(this->m_Hasher, CI.pCS);
    }
};

template <typename HasherType>
struct HashCombiner<HasherType, RayTracingPipelineStateCreateInfo> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const RayTracingPipelineStateCreateInfo& CI) const
    {
        this->m_Hasher(
            static_cast<const PipelineStateCreateInfo&>(CI),
            CI.RayTracingPipeline,
            CI.GeneralShaderCount,
            CI.TriangleHitShaderCount,
            CI.ProceduralHitShaderCount,
            CI.pShaderRecordName,
            CI.MaxAttributeSize,
            CI.MaxPayloadSize);

        for (size_t i = 0; i < CI.GeneralShaderCount; ++i)
        {
            const RayTracingGeneralShaderGroup& GeneralShader = CI.pGeneralShaders[i];
            this->m_Hasher(GeneralShader.Name);
            HashShaderBytecode(this->m_Hasher, GeneralShader.pShader);
        }

        for (size_t i = 0; i < CI.TriangleHitShaderCount; ++i)
        {
            const RayTracingTriangleHitShaderGroup& TriHitShader = CI.pTriangleHitShaders[i];
            this->m_Hasher(TriHitShader.Name);
            HashShaderBytecode(this->m_Hasher, TriHitShader.pAnyHitShader);
            HashShaderBytecode(this->m_Hasher, TriHitShader.pClosestHitShader);
        }

        for (size_t i = 0; i < CI.ProceduralHitShaderCount; ++i)
        {
            const RayTracingProceduralHitShaderGroup& ProcHitShader = CI.pProceduralHitShaders[i];
            this->m_Hasher(ProcHitShader.Name);
            HashShaderBytecode(this->m_Hasher, ProcHitShader.pAnyHitShader);
            HashShaderBytecode(this->m_Hasher, ProcHitShader.pClosestHitShader);
            HashShaderBytecode(this->m_Hasher, ProcHitShader.pIntersectionShader);
        }
    }
};

template <typename HasherType>
struct HashCombiner<HasherType, TilePipelineDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const TilePipelineDesc& Desc) const
    {
        ASSERT_SIZEOF(Desc.NumRenderTargets, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.SampleCount, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            ((static_cast<uint32_t>(Desc.NumRenderTargets) << 0u) |
             (static_cast<uint32_t>(Desc.SampleCount) << 8u)));

        for (size_t i = 0; i < Desc.NumRenderTargets; ++i)
            this->m_Hasher(Desc.RTVFormats[i]);
    }
};

template <typename HasherType>
struct HashCombiner<HasherType, TilePipelineStateCreateInfo> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const TilePipelineStateCreateInfo& CI) const
    {
        this->m_Hasher(static_cast<const PipelineStateCreateInfo&>(CI), CI.TilePipeline);
        HashShaderBytecode(this->m_Hasher, CI.pTS);
    }
};

template <typename HasherType>
struct HashCombiner<HasherType, VertexPoolElementDesc> : HashCombinerBase<HasherType>
{
    HashCombiner(HasherType& Hasher) :
        HashCombinerBase<HasherType>{Hasher}
    {}

    void operator()(const VertexPoolElementDesc& Desc) const
    {
        ASSERT_SIZEOF(Desc.Usage, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.CPUAccessFlags, 1, "Hash logic below may be incorrect.");
        ASSERT_SIZEOF(Desc.Mode, 1, "Hash logic below may be incorrect.");

        this->m_Hasher(
            Desc.Size,
            Desc.BindFlags,
            ((static_cast<uint32_t>(Desc.Usage) << 0u) |
             (static_cast<uint32_t>(Desc.CPUAccessFlags) << 8u) |
             (static_cast<uint32_t>(Desc.Mode) << 16u)));
    }
};

struct DefaultHasher
{
    template <typename... ArgsType>
    std::size_t operator()(const ArgsType&... Args) noexcept
    {
        HashCombine(m_Seed, Args...);
        return m_Seed;
    }

    void UpdateRaw(const void* pData, uint64_t Size) noexcept
    {
        HashCombine(m_Seed, ComputeHashRaw(pData, static_cast<size_t>(Size)));
    }

    size_t Get() const
    {
        return m_Seed;
    }

private:
    size_t m_Seed = 0;
};

template <typename Type>
struct StdHasher
{
    size_t operator()(const Type& Val) const
    {
        DefaultHasher                     Hasher;
        HashCombiner<DefaultHasher, Type> Combiner{Hasher};
        Combiner(Val);
        return Hasher.Get();
    }
};

} // namespace Diligent


namespace std
{

template <typename T>
struct hash<Diligent::RefCntAutoPtr<T>>
{
    size_t operator()(const Diligent::RefCntAutoPtr<T>& Key) const noexcept
    {
        return std::hash<const T*>{}(static_cast<const T*>(Key));
    }
};


template <>
struct hash<HashMapStringKey>
{
    size_t operator()(const HashMapStringKey& Key) const noexcept
    {
        return Key.GetHash();
    }
};


#define DEFINE_HASH(Type)                        \
    template <>                                  \
    struct hash<Type>                            \
    {                                            \
        size_t operator()(const Type& Val) const \
        {                                        \
            Diligent::StdHasher<Type> Hasher;    \
            return Hasher(Val);                  \
        }                                        \
    }

DEFINE_HASH(Diligent::SamplerDesc);
DEFINE_HASH(Diligent::StencilOpDesc);
DEFINE_HASH(Diligent::DepthStencilStateDesc);
DEFINE_HASH(Diligent::RasterizerStateDesc);
DEFINE_HASH(Diligent::BlendStateDesc);
DEFINE_HASH(Diligent::TextureViewDesc);
DEFINE_HASH(Diligent::SampleDesc);
DEFINE_HASH(Diligent::ShaderResourceVariableDesc);
DEFINE_HASH(Diligent::ImmutableSamplerDesc);
DEFINE_HASH(Diligent::PipelineResourceDesc);
DEFINE_HASH(Diligent::PipelineResourceLayoutDesc);
DEFINE_HASH(Diligent::RenderPassAttachmentDesc);
DEFINE_HASH(Diligent::AttachmentReference);
DEFINE_HASH(Diligent::ShadingRateAttachment);
DEFINE_HASH(Diligent::SubpassDesc);
DEFINE_HASH(Diligent::SubpassDependencyDesc);
DEFINE_HASH(Diligent::RenderPassDesc);
DEFINE_HASH(Diligent::LayoutElement);
DEFINE_HASH(Diligent::InputLayoutDesc);
DEFINE_HASH(Diligent::GraphicsPipelineDesc);
DEFINE_HASH(Diligent::RayTracingPipelineDesc);
DEFINE_HASH(Diligent::PipelineStateDesc);
DEFINE_HASH(Diligent::PipelineResourceSignatureDesc);
DEFINE_HASH(Diligent::ShaderDesc);
DEFINE_HASH(Diligent::Version);
DEFINE_HASH(Diligent::PipelineStateCreateInfo);
DEFINE_HASH(Diligent::GraphicsPipelineStateCreateInfo);
DEFINE_HASH(Diligent::ComputePipelineStateCreateInfo);
DEFINE_HASH(Diligent::RayTracingPipelineStateCreateInfo);
DEFINE_HASH(Diligent::TilePipelineDesc);
DEFINE_HASH(Diligent::TilePipelineStateCreateInfo);
DEFINE_HASH(Diligent::VertexPoolElementDesc);


#undef DEFINE_HASH

} // namespace std
