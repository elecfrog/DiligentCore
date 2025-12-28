#pragma once

/// \file
/// Definition of the Diligent::IPipelineStateCacheVk interface

#include "../../GraphicsEngine/interface/PipelineStateCache.h"

namespace Diligent
{

// {C866CC1D-A607-4F55-8D5F-FD6D4F0E051C}
static constexpr INTERFACE_ID IID_PipelineStateCacheVk =
    {0xc866cc1d, 0xa607, 0x4f55, {0x8d, 0x5f, 0xfd, 0x6d, 0x4f, 0xe, 0x5, 0x1c}};


#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IPipelineStateCacheVkInclusiveMethods \
    IPipelineStateCacheInclusiveMethods;      \
    IPipelineStateCacheVkMethods PipelineStateCacheVk

/// Exposes Vulkan-specific functionality of a pipeline state cache object.
struct IPipelineStateCacheVk : public IPipelineStateCache
{
    /// Returns a Vulkan handle of the internal pipeline cache object.
    virtual VkPipelineCache DG_CALL_TYPE GetVkPipelineCache() const = 0;
};

} // namespace Diligent
