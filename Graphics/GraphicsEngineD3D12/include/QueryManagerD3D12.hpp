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

#include <mutex>
#include <array>
#include <vector>

#include "Query.h"
#include "IndexWrapper.hpp"

namespace Diligent
{

class CommandContext;

// https://microsoft.github.io/DirectX-Specs/d3d/CountersAndQueries.html#queries

class QueryManagerD3D12
{
public:
    QueryManagerD3D12(class RenderDeviceD3D12Impl* pDeviceD3D12Impl,
                      const UInt32                 QueryHeapSizes[],
                      SoftwareQueueIndex           CommandQueueId,
                      HardwareQueueIndex           HwQueueInd);
    ~QueryManagerD3D12();

    // clang-format off
    QueryManagerD3D12             (const QueryManagerD3D12&)  = delete;
    QueryManagerD3D12             (      QueryManagerD3D12&&) = delete;
    QueryManagerD3D12& operator = (const QueryManagerD3D12&)  = delete;
    QueryManagerD3D12& operator = (      QueryManagerD3D12&&) = delete;
    // clang-format on

    static constexpr UInt32 InvalidIndex = static_cast<UInt32>(-1);

    UInt32 AllocateQuery(QUERY_TYPE Type);
    void   ReleaseQuery(QUERY_TYPE Type, UInt32 Index);

    ID3D12QueryHeap* GetQueryHeap(QUERY_TYPE Type) const
    {
        return m_Heaps[Type].GetD3D12QueryHeap();
    }

    void BeginQuery(CommandContext& Ctx, QUERY_TYPE Type, UInt32 Index) const;
    void EndQuery(CommandContext& Ctx, QUERY_TYPE Type, UInt32 Index) const;
    void ReadQueryData(QUERY_TYPE Type, UInt32 Index, void* pDataPtr, UInt32 DataSize) const;

    SoftwareQueueIndex GetCommandQueueId() const
    {
        return m_CommandQueueId;
    }

private:
    class QueryHeapInfo
    {
    public:
        void Init(ID3D12Device*                pd3d12Device,
                  const D3D12_QUERY_HEAP_DESC& d3d12HeapDesc,
                  QUERY_TYPE                   QueryType,
                  UInt32&                      CurrResolveBufferOffset);

        QueryHeapInfo() noexcept {}
        ~QueryHeapInfo();

        // clang-format off
        QueryHeapInfo             (const QueryHeapInfo&)  = delete;
        QueryHeapInfo             (      QueryHeapInfo&&) = delete;
        QueryHeapInfo& operator = (const QueryHeapInfo&)  = delete;
        QueryHeapInfo& operator = (      QueryHeapInfo&&) = delete;
        // clang-format on

        UInt32 Allocate();
        void   Release(UInt32 Index);

        UInt32 GetQueryCount() const
        {
            return m_QueryCount;
        }
        QUERY_TYPE GetType() const
        {
            return m_Type;
        }
        UInt32 GetMaxAllocatedQueries() const
        {
            return m_MaxAllocatedQueries;
        }
        UInt32 GetResolveBufferOffset(UInt32 QueryIdx) const
        {
            VERIFY_EXPR(QueryIdx < m_QueryCount);
            return m_ResolveBufferBaseOffset + QueryIdx * m_AlignedQueryDataSize;
        }
        ID3D12QueryHeap* GetD3D12QueryHeap() const
        {
            return m_pd3d12QueryHeap;
        }
        bool IsNull() const
        {
            return m_pd3d12QueryHeap == nullptr;
        }

    private:
        CComPtr<ID3D12QueryHeap> m_pd3d12QueryHeap;

        std::mutex          m_AvailableQueriesMtx;
        std::vector<UInt32> m_AvailableQueries;

        QUERY_TYPE m_Type = QUERY_TYPE_UNDEFINED;

        UInt32 m_QueryCount          = 0;
        UInt32 m_MaxAllocatedQueries = 0;

        UInt32 m_ResolveBufferBaseOffset = 0;
        UInt32 m_AlignedQueryDataSize    = 0;
    };

    const SoftwareQueueIndex m_CommandQueueId;

    std::array<QueryHeapInfo, QUERY_TYPE_NUM_TYPES> m_Heaps;

    // Readback buffer that will contain the query data.
    CComPtr<ID3D12Resource> m_pd3d12ResolveBuffer;
};

} // namespace Diligent
