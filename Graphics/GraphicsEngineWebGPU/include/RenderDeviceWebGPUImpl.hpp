/*
 *  Copyright 2023-2025 Diligent Graphics LLC
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
/// Declaration of Diligent::RenderDeviceWebGPUImpl class

#include <memory>

#include "EngineWebGPUImplTraits.hpp"
#include "RenderDeviceBase.hpp"
#include "RenderDeviceWebGPU.h"
#include "WebGPUObjectWrappers.hpp"
#include "ShaderWebGPUImpl.hpp"
#include "UploadMemoryManagerWebGPU.hpp"
#include "DynamicMemoryManagerWebGPU.hpp"
#include "GenerateMipsHelperWebGPU.hpp"

namespace Diligent
{

class QueryManagerWebGPU;
class AttachmentCleanerWebGPU;

/// Render device implementation in WebGPU backend.
class RenderDeviceWebGPUImpl final : public RenderDeviceBase<EngineWebGPUImplTraits>
{
public:
    using TRenderDeviceBase = RenderDeviceBase<EngineWebGPUImplTraits>;

    struct CreateInfo
    {
        IMemoryAllocator&             RawMemAllocator;
        IEngineFactory* const         pEngineFactory;
        const EngineWebGPUCreateInfo& EngineCI;
        const GraphicsAdapterInfo&    AdapterInfo;
        const DeviceFeatures&         EnabledFeatures;
        WGPUInstance                  wgpuInstance = {};
        WGPUAdapter                   wgpuAdapter  = {};
        WGPUDevice                    wgpuDevice   = {};
    };
    RenderDeviceWebGPUImpl(IReferenceCounters* pRefCounters, const CreateInfo& CI) noexcept(false);

    ~RenderDeviceWebGPUImpl() override;

    IMPLEMENT_QUERY_INTERFACE_IN_PLACE(IID_RenderDeviceWebGPU, TRenderDeviceBase)

    /// Implementation of IRenderDevice::CreateBuffer() in WebGPU backend.
    void CALLTYPE CreateBuffer(const BufferDesc& BuffDesc,
                                         const BufferData* pBuffData,
                                         IBuffer**         ppBuffer) override final;

    /// Implementation of IRenderDevice::CreateShader() in WebGPU backend.
    void CALLTYPE CreateShader(const ShaderCreateInfo& ShaderCI,
                                         IShader**               ppShader,
                                         IDataBlob**             ppCompilerOutput) override final;

    /// Implementation of IRenderDevice::CreateTexture() in WebGPU backend.
    void CALLTYPE CreateTexture(const TextureDesc& TexDesc,
                                          const TextureData* pData,
                                          ITexture**         ppTexture) override final;

    /// Implementation of IRenderDevice::CreateSampler() in WebGPU backend.
    void CALLTYPE CreateSampler(const SamplerDesc& SamplerDesc,
                                          ISampler**         ppSampler) override final;

    /// Implementation of IRenderDevice::CreateGraphicsPipelineState() in WebGPU backend.
    void CALLTYPE CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& PSOCreateInfo,
                                                        IPipelineState**                       ppPipelineState) override final;

    /// Implementation of IRenderDevice::CreateComputePipelineState() in WebGPU backend.
    void CALLTYPE CreateComputePipelineState(const ComputePipelineStateCreateInfo& PSOCreateInfo,
                                                       IPipelineState**                      ppPipelineState) override final;

    /// Implementation of IRenderDevice::CreateRayTracingPipelineState() in WebGPU backend.
    void CALLTYPE CreateRayTracingPipelineState(const RayTracingPipelineStateCreateInfo& PSOCreateInfo,
                                                          IPipelineState**                         ppPipelineState) override final;

    /// Implementation of IRenderDevice::CreateFence() in WebGPU backend.
    void CALLTYPE CreateFence(const FenceDesc& Desc,
                                        IFence**         ppFence) override final;

    /// Implementation of IRenderDevice::CreateQuery() in WebGPU backend.
    void CALLTYPE CreateQuery(const QueryDesc& Desc,
                                        IQuery**         ppQuery) override final;

    /// Implementation of IRenderDevice::CreateRenderPass() in WebGPU backend.
    void CALLTYPE CreateRenderPass(const RenderPassDesc& Desc,
                                             IRenderPass**         ppRenderPass) override final;

    /// Implementation of IRenderDevice::CreateFramebuffer() in WebGPU backend.
    void CALLTYPE CreateFramebuffer(const FramebufferDesc& Desc,
                                              IFramebuffer**         ppFramebuffer) override final;

    /// Implementation of IRenderDevice::CreateBLAS() in WebGPU backend.
    void CALLTYPE CreateBLAS(const BottomLevelASDesc& Desc,
                                       IBottomLevelAS**         ppBLAS) override final;

    /// Implementation of IRenderDevice::CreateTLAS() in WebGPU backend.
    void CALLTYPE CreateTLAS(const TopLevelASDesc& Desc,
                                       ITopLevelAS**         ppTLAS) override final;

    /// Implementation of IRenderDevice::CreateSBT() in WebGPU backend.
    void CALLTYPE CreateSBT(const ShaderBindingTableDesc& Desc,
                                      IShaderBindingTable**         ppSBT) override final;

    /// Implementation of IRenderDevice::CreatePipelineResourceSignature() in WebGPU backend.
    void CALLTYPE CreatePipelineResourceSignature(const PipelineResourceSignatureDesc& Desc,
                                                            IPipelineResourceSignature**         ppSignature) override final;

    /// Implementation of IRenderDevice::CreateDeviceMemory() in WebGPU backend.
    void CALLTYPE CreateDeviceMemory(const DeviceMemoryCreateInfo& CreateInfo,
                                               IDeviceMemory**               ppMemory) override final;

    /// Implementation of IRenderDevice::CreatePipelineStateCache() in WebGPU backend.
    void CALLTYPE CreatePipelineStateCache(const PipelineStateCacheCreateInfo& CreateInfo,
                                                     IPipelineStateCache**               ppPSOCache) override final;

    /// Implementation of IRenderDevice::CreateDeferredContext() in WebGPU backend.
    virtual void CALLTYPE CreateDeferredContext(IDeviceContext** ppContext) override final;

    /// Implementation of IRenderDevice::ReleaseStaleResources() in WebGPU backend.
    void CALLTYPE ReleaseStaleResources(bool ForceRelease = false) override final {}

    /// Implementation of IRenderDevice::IdleGPU() in WebGPU backend.
    void CALLTYPE IdleGPU() override final;

    /// Implementation of IRenderDevice::GetSparseTextureFormatInfo() in WebGPU backend.
    SparseTextureFormatInfo CALLTYPE GetSparseTextureFormatInfo(TEXTURE_FORMAT     TexFormat,
                                                                          RESOURCE_DIMENSION Dimension,
                                                                          UInt32             SampleCount) const override final;

    /// Implementation of IRenderDeviceWebGPU::GetWebGPUInstance() in WebGPU backend.
    WGPUInstance CALLTYPE GetWebGPUInstance() const override final;

    /// Implementation of IRenderDeviceWebGPU::GetWebGPUAdapter() in WebGPU backend.
    WGPUAdapter CALLTYPE GetWebGPUAdapter() const override final;

    /// Implementation of IRenderDeviceWebGPU::GetWebGPUDevice() in WebGPU backend.
    WGPUDevice CALLTYPE GetWebGPUDevice() const override final;

    /// Implementation of IRenderDeviceWebGPU::CreateTextureFromWebGPUTexture() in WebGPU backend.
    void CALLTYPE CreateTextureFromWebGPUTexture(WGPUTexture        wgpuTexture,
                                                           const TextureDesc& TexDesc,
                                                           RESOURCE_STATE     InitialState,
                                                           ITexture**         ppTexture) override final;

    /// Implementation of IRenderDeviceWebGPU::CreateBufferFromWebGPUBuffer() in WebGPU backend.
    void CALLTYPE CreateBufferFromWebGPUBuffer(WGPUBuffer        wgpuBuffer,
                                                         const BufferDesc& BuffDesc,
                                                         RESOURCE_STATE    InitialState,
                                                         IBuffer**         ppBuffer) override final;

public:
    void CreatePipelineResourceSignature(const PipelineResourceSignatureDesc& Desc,
                                         IPipelineResourceSignature**         ppSignature,
                                         SHADER_TYPE                          ShaderStages,
                                         bool                                 IsDeviceInternal);

    void CreatePipelineResourceSignature(const PipelineResourceSignatureDesc&               Desc,
                                         const PipelineResourceSignatureInternalDataWebGPU& InternalData,
                                         IPipelineResourceSignature**                       ppSignature);

    void CreateBuffer(const BufferDesc& BuffDesc,
                      const BufferData* pBuffData,
                      IBuffer**         ppBuffer,
                      bool              IsDeviceInternal);

    void CreateTexture(const TextureDesc& TexDesc,
                       const TextureData* pData,
                       ITexture**         ppTexture,
                       bool               IsDeviceInternal);

    void CreateSampler(const SamplerDesc& SamplerDesc,
                       ISampler**         ppSampler,
                       bool               IsDeviceInternal);

    const WGPULimits& GetLimits() const { return m_wgpuLimits; }

    QueryManagerWebGPU& GetQueryManager() { return *m_pQueryManager; }

    UInt64 GetCommandQueueCount() const { return 1; }

    UInt64 GetCommandQueueMask() const { return 1; }

    GenerateMipsHelperWebGPU& GetMipsGenerator() const;

    AttachmentCleanerWebGPU& GetAttachmentCleaner() const;

    UploadMemoryManagerWebGPU::Page GetUploadMemoryPage(size_t Size);

    DynamicMemoryManagerWebGPU::Page GetDynamicMemoryPage(size_t Size);

    DynamicMemoryManagerWebGPU& GetDynamicMemoryManager() const
    {
        return *m_pDynamicMemoryManager;
    }

    void DeviceTick();

private:
    void TestTextureFormat(TEXTURE_FORMAT TexFormat) override;

    void FindSupportedTextureFormats();

private:
    WebGPUInstanceWrapper m_wgpuInstance;
    WebGPUAdapterWrapper  m_wgpuAdapter;
    WebGPUDeviceWrapper   m_wgpuDevice;
    WGPULimits            m_wgpuLimits{};

    std::unique_ptr<UploadMemoryManagerWebGPU>  m_pUploadMemoryManager;
    std::unique_ptr<DynamicMemoryManagerWebGPU> m_pDynamicMemoryManager;

    std::unique_ptr<AttachmentCleanerWebGPU>  m_pAttachmentCleaner;
    std::unique_ptr<GenerateMipsHelperWebGPU> m_pMipsGenerator;
    std::unique_ptr<QueryManagerWebGPU>       m_pQueryManager;
};

} // namespace Diligent
