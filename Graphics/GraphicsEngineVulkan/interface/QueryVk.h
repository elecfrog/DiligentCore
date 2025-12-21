#pragma once

#include "../../GraphicsEngine/interface/Query.h"

namespace Diligent {

// {161C015B-1FE2-4452-8BFF-E35F27B3103C}
static constexpr INTERFACE_ID IID_QueryVk =
    {0x161c015b, 0x1fe2, 0x4452, {0x8b, 0xff, 0xe3, 0x5f, 0x27, 0xb3, 0x10, 0x3c}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IQueryVkInclusiveMethods \
    IQueryInclusiveMethods
//IQueryVkMethods QueryVk

#if DILIGENT_CPP_INTERFACE

/// Exposes Vulkan-specific functionality of a Query object.
struct IQueryVk : public IQuery{};


#endif



} // namespace Diligent
