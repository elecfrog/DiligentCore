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
/// Declaration of functions that initialize Vulkan-based engine implementation

#include "../../GraphicsEngine/interface/EngineFactory.h"
#include "../../GraphicsEngine/interface/RenderDevice.h"
#include "../../GraphicsEngine/interface/DeviceContext.h"
#include "../../GraphicsEngine/interface/SwapChain.h"

#if PLATFORM_ANDROID || PLATFORM_LINUX || PLATFORM_MACOS || PLATFORM_IOS || PLATFORM_TVOS || (PLATFORM_WIN32 && !defined(_MSC_VER))
// https://gcc.gnu.org/wiki/Visibility
#    define API_QUALIFIER __attribute__((visibility("default")))
#elif PLATFORM_WIN32
#    define API_QUALIFIER
#else
#    error Unsupported platform
#endif

#if DILIGENT_VK_SHARED && PLATFORM_WIN32 && defined(_MSC_VER)
#    include "../../GraphicsEngine/interface/LoadEngineDll.h"
#    define DILIGENT_VK_EXPLICIT_LOAD 1
#endif

namespace Diligent
{

// {F554EEE4-57C2-4637-A508-85BE80DC657C}
static constexpr INTERFACE_ID IID_EngineFactoryVk =
    {0xf554eee4, 0x57c2, 0x4637, {0xa5, 0x8, 0x85, 0xbe, 0x80, 0xdc, 0x65, 0x7c}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IEngineFactoryVkInclusiveMethods \
    IEngineFactoryInclusiveMethods;      \
    IEngineFactoryVkMethods EngineFactoryVk


struct IEngineFactoryVk : public IEngineFactory
{
    /// Creates a render device and device contexts for Vulkan backend

    /// \param [in] EngineCI    - Engine creation attributes.
    /// \param [out] ppDevice   - Address of the memory location where pointer to
    ///                           the created device will be written
    /// \param [out] ppContexts - Address of the memory location where pointers to
    ///                           the contexts will be written. Immediate context goes at
    ///                           position 0. If EngineCI.NumDeferredContexts > 0,
    ///                           pointers to the deferred contexts are written afterwards.
    virtual void CALLTYPE CreateDeviceAndContextsVk(const EngineVkCreateInfo& EngineCI,
                                                    IRenderDevice**           ppDevice,
                                                    IDeviceContext**          ppContexts) = 0;


    /// Creates a swap chain for Vulkan-based engine implementation

    /// \param [in] pDevice           - Pointer to the render device
    /// \param [in] pImmediateContext - Pointer to the immediate device context.
    ///                                 Swap chain creation will fail if the context can not present to the window.
    /// \param [in] SCDesc            - Swap chain description
    /// \param [in] Window            - Platform-specific native window description that
    ///                                 the swap chain will be associated with.
    ///
    /// \param [out] ppSwapChain    - Address of the memory location where pointer to the new
    ///                               swap chain will be written
    virtual void CALLTYPE CreateSwapChainVk(
        IRenderDevice*       pDevice,
        IDeviceContext*      pImmediateContext,
        const SwapChainDesc& SwapChainDesc,
        const NativeWindow&  Window,
        ISwapChain**         ppSwapChain) = 0;

    /// Enables device simulation layer (if available).

    /// Vulkan instance will be created with the device simulation layer.
    /// Use VK_DEVSIM_FILENAME environment variable to define the path to the .json file.
    ///
    /// \remarks Use this function before calling EnumerateAdapters() and CreateDeviceAndContextsVk().
    virtual void CALLTYPE EnableDeviceSimulation() = 0;


    /// Returns the supported Vulkan version. If Vulkan is not supported, returns 0.

    /// This function can be used to check whether Vulkan is supported on the platform.
    virtual Version CALLTYPE GetVulkanVersion() const = 0;
};

typedef struct IEngineFactoryVk* (*GetEngineFactoryVkType)();

#if DILIGENT_VK_EXPLICIT_LOAD

inline GetEngineFactoryVkType DILIGENT_GLOBAL_FUNCTION(LoadGraphicsEngineVk)()
{
    static GetEngineFactoryVkType GetFactoryFunc = NULL;
    if (GetFactoryFunc == NULL)
    {
        GetFactoryFunc = (GetEngineFactoryVkType)LoadEngineDll("GraphicsEngineVk", "GetEngineFactoryVk");
    }
    return GetFactoryFunc;
}

#else

API_QUALIFIER
struct IEngineFactoryVk* DILIGENT_GLOBAL_FUNCTION(GetEngineFactoryVk)();

#endif

/// Loads the graphics engine Vulkan implementation DLL if necessary and returns the engine factory.
inline struct IEngineFactoryVk* DILIGENT_GLOBAL_FUNCTION(LoadAndGetEngineFactoryVk)()
{
    GetEngineFactoryVkType GetFactoryFunc = NULL;
#if DILIGENT_VK_EXPLICIT_LOAD
    GetFactoryFunc = DILIGENT_GLOBAL_FUNCTION(LoadGraphicsEngineVk)();
    if (GetFactoryFunc == NULL)
    {
        return NULL;
    }
#else
    GetFactoryFunc = DILIGENT_GLOBAL_FUNCTION(GetEngineFactoryVk);
#endif
    return GetFactoryFunc();
}

} // namespace Diligent
