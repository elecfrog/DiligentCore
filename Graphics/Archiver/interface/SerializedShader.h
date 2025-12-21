#pragma once

/// \file
/// Defines Diligent::ISerializedShader interface

#include "../../GraphicsEngine/interface/GraphicsTypes.h"
#include "../../GraphicsEngine/interface/Shader.h"

namespace Diligent
{

// {53A9A017-6A34-4AE9-AA23-C8E587023F9E}
static constexpr INTERFACE_ID IID_SerializedShader =
    {0x53a9a017, 0x6a34, 0x4ae9, {0xaa, 0x23, 0xc8, 0xe5, 0x87, 0x2, 0x3f, 0x9e}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define ISerializedShaderInclusiveMethods \
    IShaderInclusiveMethods;              \
    ISerializedShaderMethods SerializedShader

// clang-format off

/// Serialized shader interface
struct ISerializedShader : public IShader
{
    /// Returns a device-specific shader for the given render device type.

    /// \note   In order for the returned shader object to be fully initialized
    ///         and suitable for use in rendering commands, a corresponding render
    ///         device must have been initialized in the serialization device through
    ///         ISerializationDevice::AddRenderDevice().
    virtual IShader* METHOD(GetDeviceShader)(

        enum RENDER_DEVICE_TYPE DeviceType) const =0;
};

 } // namespace Diligent
