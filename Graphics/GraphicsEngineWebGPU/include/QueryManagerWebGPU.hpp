/*
 *  Copyright 2023-2024 Diligent Graphics LLC
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
/// Declaration of Diligent::QueryManagerWebGPU class

#include <vector>
#include <array>

#include "EngineWebGPUImplTraits.hpp"
#include "RenderDeviceWebGPUImpl.hpp"
#include "DeviceContextWebGPUImpl.hpp"

namespace Diligent
{

class QueryManagerWebGPU
{
public:
    QueryManagerWebGPU(RenderDeviceWebGPUImpl* pRenderDeviceWebGPU,
                       const UInt32            QueryHeapSizes[]);

    ~QueryManagerWebGPU();

    // clang-format off
    QueryManagerWebGPU             (const QueryManagerWebGPU&)  = delete;
    QueryManagerWebGPU             (      QueryManagerWebGPU&&) = delete;
    QueryManagerWebGPU& operator = (const QueryManagerWebGPU&)  = delete;
    QueryManagerWebGPU& operator = (      QueryManagerWebGPU&&) = delete;
    // clang-format on

    static constexpr UInt32 InvalidIndex = UINT32_MAX;

    UInt32 AllocateQuery(QUERY_TYPE Type);

    void DiscardQuery(QUERY_TYPE Type, UInt32 Index);

    WGPUQuerySet GetQuerySet(QUERY_TYPE Type) const;

    UInt64 GetQueryResult(QUERY_TYPE Type, UInt32 Index) const;

    void ResolveQuerySet(RenderDeviceWebGPUImpl* pDevice, DeviceContextWebGPUImpl* pDeviceContext);

private:
    class QuerySetObject final : public ObjectBase<IDeviceObject>, public WebGPUResourceBase
    {
    public:
        QuerySetObject(IReferenceCounters* pRefCounters, RenderDeviceWebGPUImpl* pDevice, UInt32 HeapSize, QUERY_TYPE QueryType);

        ~QuerySetObject();

        UInt32 Allocate();

        void Discard(UInt32 Index);

        QUERY_TYPE GetType() const;

        UInt32 GetQueryCount() const;

        UInt64 GetQueryResult(UInt32 Index) const;

        WGPUQuerySet GetWebGPUQuerySet() const;

        UInt32 GetMaxAllocatedQueries() const;

        void ResolveQueries(RenderDeviceWebGPUImpl* pDevice, DeviceContextWebGPUImpl* pDeviceContext);

        const DeviceObjectAttribs& DILIGENT_CALL_TYPE GetDesc() const override final;

        Int32 DILIGENT_CALL_TYPE GetUniqueID() const override final;

        void DILIGENT_CALL_TYPE SetUserData(IObject* pUserData) override final;

        IObject* DILIGENT_CALL_TYPE GetUserData() const override final;

    private:
        DeviceObjectAttribs   m_Desc;
        WebGPUQuerySetWrapper m_wgpuQuerySet;
        WebGPUBufferWrapper   m_wgpuResolveBuffer;
        std::vector<UInt32>   m_AvailableQueries;

        QUERY_TYPE m_Type                = QUERY_TYPE_UNDEFINED;
        UInt32     m_QueryCount          = 0;
        UInt32     m_MaxAllocatedQueries = 0;
    };

    std::array<RefCntAutoPtr<QuerySetObject>, QUERY_TYPE_NUM_TYPES> m_QuerySets;
};

} // namespace Diligent
