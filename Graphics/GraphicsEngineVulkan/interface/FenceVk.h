#pragma once

#include "../../GraphicsEngine/interface/Fence.h"

namespace Diligent
{

// {7610B4CD-EDEA-4951-82CF-52F97FAFED2D}
static constexpr INTERFACE_ID IID_FenceVk =
    {0x7610b4cd, 0xedea, 0x4951, {0x82, 0xcf, 0x52, 0xf9, 0x7f, 0xaf, 0xed, 0x2d}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IFenceVkInclusiveMethods \
    IFenceInclusiveMethods;      \
    IFenceVkMethods FenceVk

/// Exposes Vulkan-specific functionality of a fence object.
struct IFenceVk : public IFence
{
    /// If timeline semaphores are supported, returns the semaphore object; otherwise returns VK_NULL_HANDLE.
    virtual VkSemaphore DG_CALL_TYPE GetVkSemaphore() = 0;
};

} // namespace Diligent
