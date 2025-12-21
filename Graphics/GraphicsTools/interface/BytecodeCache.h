#pragma once
// clang-format off
/// \file
/// Defines Diligent::IBytecodeCache interface
#include "../../GraphicsEngine/interface/RenderDevice.h"

namespace Diligent {

struct BytecodeCacheCreateInfo
{
    enum RENDER_DEVICE_TYPE DeviceType DEFAULT_INITIALIZER(RENDER_DEVICE_TYPE_UNDEFINED);
};
typedef struct BytecodeCacheCreateInfo BytecodeCacheCreateInfo;

// clang-format on

// {D1F8295F-F9D7-4CD4-9D13-D950FE7572C1}
static constexpr INTERFACE_ID IID_BytecodeCache = {0xD1F8295F, 0xF9D7, 0x4CD4, {0x9D, 0x13, 0xD9, 0x50, 0xFE, 0x75, 0x72, 0xC1}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IBytecodeCacheInclusiveMethods \
    IObjectInclusiveMethods;           \
    IBytecodeCacheMethods BytecodeCache

// clang-format off

/// Byte code cache interface
DILIGENT_BEGIN_INTERFACE(IBytecodeCache, IObject)
{
    /// Loads the cache data from the binary blob

    /// \param [in] pData - A pointer to the cache data.
    /// \return     true if the data was loaded successfully, and false otherwise.
    virtual bool METHOD(Load)(
                              IDataBlob* pData) =0;

    /// Returns the byte code for the requested shader create parameters.

    /// \param [in]  ShaderCI   - Shader create info to find the byte code for.
    /// \param [out] ppByteCode - Address of the memory location where a pointer to the
    ///                           data blob containing the byte code will be written.
    ///                           The function calls AddRef(), so that the new object will have
    ///                           one reference.
    virtual void METHOD(GetBytecode)(
                                     const ShaderCreateInfo  & ShaderCI,
                                     IDataBlob**                ppByteCode) =0;

    /// Adds the byte code to the cache.

    /// \param [in] ShaderCI  - Shader create parameters for the byte code to add.
    /// \param [in] pByteCode - A pointer to the byte code to add to the cache.
    ///
    /// \remarks    If the byte code for the given shader create parameters is already present
    ///             in the cache, it is replaced.
    virtual void METHOD(AddBytecode)(
                                     const ShaderCreateInfo  & ShaderCI,
                                     IDataBlob*                 pByteCode) =0;

    /// Removes the byte code from the cache.

    /// \param [in] ShaderCI - Shader create information for the byte code to remove.
    virtual void METHOD(RemoveBytecode)(
                                        const ShaderCreateInfo  & ShaderCI) =0;

    /// Writes the cache data to the binary data blob.

    /// \param [out] ppDataBlob - Address of the memory location where a pointer to the
    ///                           data blob containing the cache data will be written.
    ///                           The function calls AddRef(), so that the new object will have
    ///                           one reference.
    ///
    /// \remarks    The data produced by this method is intended to be used by the Load method.
    virtual void METHOD(Store)(
                               IDataBlob** ppDataBlob) =0;


    /// Clears the cache and resets it to default state.
    virtual void METHOD(Clear)( ) =0;
};

void CreateBytecodeCache(const BytecodeCacheCreateInfo& CreateInfo, IBytecodeCache** ppCache);

} // namespace Diligent
