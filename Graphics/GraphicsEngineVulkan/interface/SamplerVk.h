#pragma once

/// \file
/// Definition of the Diligent::ISamplerVk interface

#include "../../GraphicsEngine/interface/Sampler.h"

namespace Diligent
{

// {87C21E88-8A9F-4AD2-9A1E-D5EC140415EA}
static constexpr INTERFACE_ID IID_SamplerVk =
    {0x87c21e88, 0x8a9f, 0x4ad2, {0x9a, 0x1e, 0xd5, 0xec, 0x14, 0x4, 0x15, 0xea}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define ISamplerVkInclusiveMethods \
    ISamplerInclusiveMethods;      \
    ISamplerVkMethods SamplerVk

/// Exposes Vulkan-specific functionality of a sampler object.
struct ISamplerVk : public ISampler
{
    /// Returns a Vulkan handle of the internal sampler object.
    virtual VkSampler METHOD(GetVkSampler)() const = 0;
};

} // namespace Diligent
