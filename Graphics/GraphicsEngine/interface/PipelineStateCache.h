#pragma once

// clang-format off

/// \file
/// Definition of the Diligent::IPipelineStateCache interface and related data structures

#include "../../../Primitives/interface/Object.h"
#include "../../../Primitives/interface/DataBlob.h"
#include "GraphicsTypes.h"
#include "DeviceObject.h"

namespace Diligent {

/// Pipeline state cache mode.
DILIGENT_TYPED_ENUM(PSO_CACHE_MODE, UInt8)
{
    /// PSO cache will be used to load PSOs from it.
    PSO_CACHE_MODE_LOAD  = 1u << 0u,

    /// PSO cache will be used to store PSOs.
    PSO_CACHE_MODE_STORE = 1u << 1u,

    /// PSO cache will be used to load and store PSOs.
    PSO_CACHE_MODE_LOAD_STORE = PSO_CACHE_MODE_LOAD | PSO_CACHE_MODE_STORE
};
DEFINE_FLAG_ENUM_OPERATORS(PSO_CACHE_MODE);

/// Pipeline state cache flags.
DILIGENT_TYPED_ENUM(PSO_CACHE_FLAGS, UInt8)
{
    /// No flags.
    PSO_CACHE_FLAG_NONE = 0u,

    /// Print diagnostic messages e.g. when PSO is not found in the cache.
    PSO_CACHE_FLAG_VERBOSE = 1u << 0u
};
DEFINE_FLAG_ENUM_OPERATORS(PSO_CACHE_FLAGS);

/// Pipeline state cache description
struct PipelineStateCacheDesc DILIGENT_DERIVE(DeviceObjectAttribs)

    /// Cache mode, see Diligent::PSO_CACHE_MODE.

    /// Metal backend allows generating the cache on one device
    /// and loading PSOs from it on another.
    ///
    /// Vulkan PSO cache depends on the GPU device, driver version and other parameters,
    /// so the cache must be generated and used on the same device.
    PSO_CACHE_MODE Mode DEFAULT_INITIALIZER(PSO_CACHE_MODE_LOAD_STORE);

    /// PSO cache flags, see Diligent::PSO_CACHE_FLAGS.
    PSO_CACHE_FLAGS Flags DEFAULT_INITIALIZER(PSO_CACHE_FLAG_NONE);

    // ImmediateContextMask ?
};
typedef struct PipelineStateCacheDesc PipelineStateCacheDesc;

/// Pipeline state pbject cache create info
struct PipelineStateCacheCreateInfo
{
    /// Pipeline state cache description
    PipelineStateCacheDesc Desc;

    /// All fields can be null to create an empty cache
    const void* pCacheData    DEFAULT_INITIALIZER(nullptr);

    /// The size of data pointed to by pCacheData
    UInt32      CacheDataSize DEFAULT_INITIALIZER(0);
};
typedef struct PipelineStateCacheCreateInfo PipelineStateCacheCreateInfo;

// clang-format on

// {6AC86F22-FFF4-493C-8C1F-C539D934F4BC}
static constexpr INTERFACE_ID IID_PipelineStateCache =
    {0x6ac86f22, 0xfff4, 0x493c, {0x8c, 0x1f, 0xc5, 0x39, 0xd9, 0x34, 0xf4, 0xbc}};


#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IPipelineStateCacheInclusiveMethods \
    IDeviceObjectInclusiveMethods;          \
    IPipelineStateCacheMethods PipelineStateCache

// clang-format off

/// Pipeline state cache interface
struct IPipelineStateCache : public IDeviceObject
{
    /// Creates a blob with pipeline state cache data
    virtual void CALLTYPE GetData( IDataBlob** ppBlob) =0;
};

}
