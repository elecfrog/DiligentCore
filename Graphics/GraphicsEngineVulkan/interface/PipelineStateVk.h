#pragma once

#include "../../GraphicsEngine/interface/PipelineState.h"
#include "RenderPassVk.h"

namespace Diligent
{

// {2FEA0868-0932-412A-9F0A-7CEA7E61B5E0}
static constexpr INTERFACE_ID IID_PipelineStateVk =
    {0x2fea0868, 0x932, 0x412a, {0x9f, 0xa, 0x7c, 0xea, 0x7e, 0x61, 0xb5, 0xe0}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IPipelineStateVkInclusiveMethods \
    IPipelineStateInclusiveMethods;      \
    IPipelineStateVkMethods PipelineStateVk

/// Exposes Vulkan-specific functionality of a pipeline state object.
struct IPipelineStateVk : public IPipelineState
{
    /// Returns a pointer to the internal render pass object.
    virtual IRenderPassVk* METHOD(GetRenderPass)() const = 0;

    /// Returns a Vulkan handle of the internal pipeline state object.
    virtual VkPipeline METHOD(GetVkPipeline)() const = 0;
};

} // namespace Diligent
