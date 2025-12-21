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

/// \file
/// Declaration of Diligent::BufferVkImpl class

#include "EngineVkImplTraits.hpp"
#include "BufferBase.hpp"
#include "BufferViewVkImpl.hpp" // Required by BufferBase

#include "VulkanDynamicHeap.hpp"
#include "VulkanUtilities/ObjectWrappers.hpp"
#include "VulkanUtilities/MemoryManager.hpp"
#include "STDAllocator.hpp"

namespace Diligent
{

/// Buffer object implementation in Vulkan backend.
class BufferVkImpl final : public BufferBase<EngineVkImplTraits>
{
public:
    using TBufferBase = BufferBase<EngineVkImplTraits>;

    BufferVkImpl(IReferenceCounters*        pRefCounters,
                 FixedBlockMemoryAllocator& BuffViewObjMemAllocator,
                 RenderDeviceVkImpl*        pDeviceVk,
                 const BufferDesc&          BuffDesc,
                 const BufferData*          pBuffData = nullptr);

    BufferVkImpl(IReferenceCounters*        pRefCounters,
                 FixedBlockMemoryAllocator& BuffViewObjMemAllocator,
                 class RenderDeviceVkImpl*  pDeviceVk,
                 const BufferDesc&          BuffDesc,
                 RESOURCE_STATE             InitialState,
                 VkBuffer                   vkBuffer);
    ~BufferVkImpl();

    IMPLEMENT_QUERY_INTERFACE_IN_PLACE(IID_BufferVk, TBufferBase)

    /// Implementation of IBufferVk::GetVkBuffer().
    virtual VkBuffer CALLTYPE GetVkBuffer() const override final;

    /// Implementation of IBuffer::GetNativeHandle() in Vulkan backend.
    virtual UInt64 CALLTYPE GetNativeHandle() override final { return BitCast<UInt64>(GetVkBuffer()); }

    /// Implementation of IBufferVk::SetAccessFlags().
    virtual void CALLTYPE SetAccessFlags(VkAccessFlags AccessFlags) override final;

    /// Implementation of IBufferVk::GetAccessFlags().
    virtual VkAccessFlags CALLTYPE GetAccessFlags() const override final;

    /// Implementation of IBufferVk::GetVkDeviceAddress().
    virtual VkDeviceAddress CALLTYPE GetVkDeviceAddress() const override final;

    /// Implementation of IBuffer::FlushMappedRange().
    virtual void CALLTYPE FlushMappedRange(UInt64 StartOffset,
                                                     UInt64 Size) override final;

    /// Implementation of IBuffer::InvalidateMappedRange().
    virtual void CALLTYPE InvalidateMappedRange(UInt64 StartOffset,
                                                          UInt64 Size) override final;

    /// Implementation of IBuffer::GetSparseProperties().
    virtual SparseBufferProperties CALLTYPE GetSparseProperties() const override final;

    bool CheckAccessFlags(VkAccessFlags AccessFlags) const
    {
        return (GetAccessFlags() & AccessFlags) == AccessFlags;
    }

    void* GetCPUAddress()
    {
        VERIFY_EXPR(m_Desc.Usage == USAGE_STAGING || m_Desc.Usage == USAGE_UNIFIED);
        return reinterpret_cast<UInt8*>(m_MemoryAllocation.Page->GetCPUMemory()) + m_BufferMemoryAlignedOffset;
    }

private:
    friend class DeviceContextVkImpl;

    virtual void CreateViewInternal(const struct BufferViewDesc& ViewDesc, IBufferView** ppView, bool bIsDefaultView) override;

    VulkanUtilities::BufferViewWrapper CreateView(struct BufferViewDesc& ViewDesc);

    UInt32       m_DynamicOffsetAlignment    = 0;
    VkDeviceSize m_BufferMemoryAlignedOffset = 0;

    VulkanUtilities::BufferWrapper    m_VulkanBuffer;
    VulkanUtilities::MemoryAllocation m_MemoryAllocation;
};

} // namespace Diligent
