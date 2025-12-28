/*
 *  Copyright 2019-2025 Diligent Graphics LLC
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
/// Defines Diligent::IArchiverFactory interface and related structures.

#include "../../../Primitives/interface/Object.h"
#include "Primitives.h"
#include "SerializationDevice.h"

namespace Diligent
{

// {F20B91EB-BDE3-4615-81CC-F720AA32410E}
static constexpr INTERFACE_ID IID_ArchiverFactory =
    {0xf20b91eb, 0xbde3, 0x4615, {0x81, 0xcc, 0xf7, 0x20, 0xaa, 0x32, 0x41, 0xe}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"

#define IArchiverFactoryInclusiveMethods \
    IObjectInclusiveMethods;             \
    IArchiverFactoryMethods ArchiverFactory

/// Serialization device attributes for Direct3D11 backend
struct SerializationDeviceD3D11Info
{
    /// Direct3D11 feature level
    Version FeatureLevel = Version(11, 0);

    /// Tests if two structures are equivalent
    constexpr bool operator==(const SerializationDeviceD3D11Info& RHS) const
    {
        return FeatureLevel == RHS.FeatureLevel;
    }
    constexpr bool operator!=(const SerializationDeviceD3D11Info& RHS) const
    {
        return !(*this == RHS);
    }
};
typedef struct SerializationDeviceD3D11Info SerializationDeviceD3D11Info;


/// Serialization device attributes for Direct3D12 backend
struct SerializationDeviceD3D12Info
{
    /// Shader version supported by the device
    Version ShaderVersion = Version(6, 0);

    /// DX Compiler path
    const Char* DxCompilerPath = nullptr;

    /// Tests if two structures are equivalent
    bool operator==(const SerializationDeviceD3D12Info& RHS) const noexcept
    {
        return ShaderVersion == RHS.ShaderVersion && SafeStrEqual(DxCompilerPath, RHS.DxCompilerPath);
    }
    bool operator!=(const SerializationDeviceD3D12Info& RHS) const noexcept
    {
        return !(*this == RHS);
    }
};
typedef struct SerializationDeviceD3D12Info SerializationDeviceD3D12Info;


/// Serialization device attributes for OpenGL backend
struct SerializationDeviceGLInfo
{
    /// Whether to optimize OpenGL shaders.

    /// \remarks    In OpenGL backend, shaders are stored as source code in the archive.
    ///             The source code can be rather large since all included files are inlined,
    ///             helper shader definitions are added, etc. Compiling such shaders may take
    ///             a significant amount of time, in particular on mobile devices and WebGL.
    ///             When OptimizeShaders is set to true, the archiver will optimize the shader
    ///             source code for run-time loading performance.
    ///
    /// \remarks    Technical details: the archiver will compile the shader source code to SPIRV
    ///             with GLSLang and then translate SPIRV back to GLSL using SPIRV-Cross.
    ///             The resulting GLSL code will be much more compact and will be stored in the
    ///             archive instead of the original source code.
    Bool OptimizeShaders DEFAULT_INITIALIZER(True);

    /// Whether to use zero-to-one clip-space Z range.
    ///
    /// \remarks    In OpenGL, the default clip-space Z range is -1 to 1.
    ///             When this flag is set to True, the archiver will assume
    ///             that the shaders use zero-to-one clip-space Z range.
    Bool ZeroToOneClipZ DEFAULT_INITIALIZER(False);

    bool operator==(const SerializationDeviceGLInfo& RHS) const noexcept
    {
        return OptimizeShaders == RHS.OptimizeShaders &&
            ZeroToOneClipZ == RHS.ZeroToOneClipZ;
    }
    bool operator!=(const SerializationDeviceGLInfo& RHS) const noexcept
    {
        return !(*this == RHS);
    }
};
typedef struct SerializationDeviceGLInfo SerializationDeviceGLInfo;


/// Serialization device attributes for Vulkan backend
struct SerializationDeviceVkInfo
{
    /// Vulkan API version
    Version ApiVersion = Version(1, 0);

    /// Indicates whether the device supports SPIRV 1.4 or above
    Bool SupportsSpirv14 = False;

    /// Path to DX compiler for Vulkan
    const Char* DxCompilerPath = nullptr;

    /// Tests if two structures are equivalent
    bool operator==(const SerializationDeviceVkInfo& RHS) const noexcept
    {
        return ApiVersion == RHS.ApiVersion &&
            SupportsSpirv14 == RHS.SupportsSpirv14 &&
            SafeStrEqual(DxCompilerPath, RHS.DxCompilerPath);
    }
    bool operator!=(const SerializationDeviceVkInfo& RHS) const noexcept
    {
        return !(*this == RHS);
    }
};
typedef struct SerializationDeviceVkInfo SerializationDeviceVkInfo;


/// Serialization device attributes for Metal backend
struct SerializationDeviceMtlInfo
{
    /// Additional compilation options for Metal command-line compiler for MacOS.
    const Char* CompileOptionsMacOS = "-sdk macosx metal";

    /// Additional compilation options for Metal command-line compiler for iOS.
    const Char* CompileOptionsiOS = "-sdk iphoneos metal";

    /// Name of the command-line application that is used to preprocess Metal shader source before compiling to bytecode.
    const Char* MslPreprocessorCmd = nullptr;

    /// Optional directory to dump converted MSL source code and temporary files produced by the Metal toolchain.
    const Char* DumpDirectory = nullptr;

    /// Tests if two structures are equivalent
    bool operator==(const SerializationDeviceMtlInfo& RHS) const noexcept
    {
        return SafeStrEqual(CompileOptionsMacOS, RHS.CompileOptionsMacOS) &&
            SafeStrEqual(CompileOptionsiOS, RHS.CompileOptionsiOS) &&
            SafeStrEqual(MslPreprocessorCmd, RHS.MslPreprocessorCmd);
    }
    bool operator!=(const SerializationDeviceMtlInfo& RHS) const noexcept
    {
        return !(*this == RHS);
    }
};
typedef struct SerializationDeviceMtlInfo SerializationDeviceMtlInfo;


/// Serialization device creation information
struct SerializationDeviceCreateInfo
{
    /// Device info, contains enabled device features.

    /// Can be used to validate shaders, render passes, resource signatures and pipeline states.
    ///
    /// \note For OpenGL that does not support separable programs, disable the SeparablePrograms feature.
    RenderDeviceInfo DeviceInfo;

    /// Adapter info, contains device parameters.

    /// Can be used to validate shaders, render passes, resource signatures and pipeline states.
    GraphicsAdapterInfo AdapterInfo;

    /// Direct3D11 attributes, see Diligent::SerializationDeviceD3D11Info.
    SerializationDeviceD3D11Info D3D11;

    /// Direct3D12 attributes, see Diligent::SerializationDeviceD3D12Info.
    SerializationDeviceD3D12Info D3D12;

    /// OpenGL attributes, see Diligent::SerializationDeviceGLInfo.
    SerializationDeviceGLInfo GL;

    /// Vulkan attributes, see Diligent::SerializationDeviceVkInfo.
    SerializationDeviceVkInfo Vulkan;

    /// Metal attributes, see Diligent::SerializationDeviceMtlInfo.
    SerializationDeviceMtlInfo Metal;

    /// An optional thread pool for asynchronous shader and pipeline state compilation.
    IThreadPool* pAsyncShaderCompilationThreadPool DEFAULT_INITIALIZER(nullptr);

    /// The maximum number of threads that can be used to compile shaders.

    /// If `pAsyncShaderCompilationThreadPool` is `null`, this value is used to define the number of threads in
    /// the default thread pool.
    /// If the value is `0xFFFFFFFF`, the number of threads will be determined automatically.
    /// If the value is `0`, the default thread pool will not be created.
    ///
    /// If `pAsyncShaderCompilationThreadPool` is not `null`, the value is ignored as the user-provided
    /// thread pool is used instead.
    UInt32 NumAsyncShaderCompilationThreads DEFAULT_INITIALIZER(0);

    SerializationDeviceCreateInfo() noexcept
    {
        DeviceInfo.Features  = DeviceFeatures{DEVICE_FEATURE_STATE_ENABLED};
        AdapterInfo.Features = DeviceFeatures{DEVICE_FEATURE_STATE_ENABLED};
        // Disable subpass framebuffer fetch by default to allow backwards compatibility on Metal.
        DeviceInfo.Features.SubpassFramebufferFetch = DEVICE_FEATURE_STATE_DISABLED;
    }
};
typedef struct SerializationDeviceCreateInfo SerializationDeviceCreateInfo;


/// Archiver factory interface
struct IArchiverFactory : public IObject
{
    /// Creates a serialization device.

    /// \param [in]  CreateInfo - Serialization device create information, see Diligent::SerializationDeviceCreateInfo.
    /// \param [out] ppDevice   - Address of the memory location where a pointer to the
    ///                           device interface will be written.
    virtual void DG_CALL_TYPE CreateSerializationDevice(const SerializationDeviceCreateInfo& CreateInfo, ISerializationDevice** ppDevice) = 0;

    /// Creates an archiver.

    /// \param [in]  pDevice    - Pointer to the serialization device.
    /// \param [out] ppArchiver - Address of the memory location where a pointer to the
    ///                           archiver interface will be written.
    virtual void DG_CALL_TYPE CreateArchiver(ISerializationDevice* pDevice, IArchiver** ppArchiver) = 0;

    /// Creates a default shader source input stream factory

    /// \param [in]  SearchDirectories           - Semicolon-separated list of search directories.
    /// \param [out] ppShaderSourceStreamFactory - Memory address where a pointer to the shader source
    ///                                            stream factory will be written.
    virtual void DG_CALL_TYPE CreateDefaultShaderSourceStreamFactory(const Char* SearchDirectories, struct IShaderSourceInputStreamFactory** ppShaderSourceFactory) const = 0;


    /// Removes device-specific data from the archive and writes a new archive to the stream.

    /// \param [in]  pSrcArchive  - Source archive from which device specific-data will be removed.
    /// \param [in]  DeviceFlags  - Combination of device types that will be removed.
    /// \param [out] ppDstArchive - Memory address where a pointer to the new archive will be written.
    /// \return     `true` if the device-specific data was successfully removed, and `false` otherwise.
    virtual Bool METHOD(RemoveDeviceData)(
        const IDataBlob*          pSrcArchive,
        ARCHIVE_DEVICE_DATA_FLAGS DeviceFlags,
        IDataBlob**               ppDstArchive) const = 0;


    /// Copies device-specific data from the source archive to the destination and writes a new archive to the stream.

    /// \param [in]  pSrcArchive    - Source archive to which new device-specific data will be added.
    /// \param [in]  DeviceFlags    - Combination of device types that will be copied.
    /// \param [in]  pDeviceArchive - Archive that contains the same common data and additional device-specific data.
    /// \param [out] ppDstArchive   - Memory address where a pointer to the new archive will be written.
    /// \return     `true` if the device-specific data was successfully added, and `false` otherwise.
    virtual Bool METHOD(AppendDeviceData)(
        const IDataBlob*          pSrcArchive,
        ARCHIVE_DEVICE_DATA_FLAGS DeviceFlags,
        const IDataBlob*          pDeviceArchive,
        IDataBlob**               ppDstArchive) const = 0;


    /// Merges multiple archives into one.

    /// \param [in]  ppSrcArchives   - An array of pointers to the source archives.
    /// \param [in]  NumSrcArchives  - The number of elements in `ppArchives` array.
    /// \param [out] ppDstArchive    - Memory address where a pointer to the merged archive will be written.
    /// \return     `true` if the archives were successfully merged, and `false` otherwise.
    virtual Bool METHOD(MergeArchives)(
        const IDataBlob* ppSrcArchives[],
        UInt32           NumSrcArchives,
        IDataBlob**      ppDstArchive) const = 0;


    /// Prints archive content for debugging and validation.
    virtual Bool METHOD(PrintArchiveContent)(
        const IDataBlob* pArchive) const = 0;

    /// Sets a user-provided debug message callback.

    /// \param [in]     MessageCallback - Debug message callback function to use instead of the default one.
    virtual void METHOD(SetMessageCallback)(spw::DebugMessageCallbackType MessageCallback) const = 0;

    /// Sets whether to break program execution on assertion failure.

    /// \param [in]     BreakOnError - Whether to break on assertion failure.
    virtual void METHOD(SetBreakOnError)(
        bool BreakOnError) const = 0;

    /// Sets the memory allocator to be used by the archiver.

    /// \param [in] pAllocator - Pointer to the memory allocator.
    ///
    /// The allocator is a global setting that applies to the entire execution unit
    /// (executable or shared library that contains the archiver implementation).
    ///
    /// The allocator should be set before any other factory method is called and
    /// should not be changed afterwards.
    /// The allocator object must remain valid until all objects created by the factory
    /// are destroyed.
    virtual void METHOD(SetMemoryAllocator)(
        IMemoryAllocator* pAllocator) const = 0;
};

} // namespace Diligent
