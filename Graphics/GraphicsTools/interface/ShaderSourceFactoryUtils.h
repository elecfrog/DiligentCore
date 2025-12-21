#pragma once

/// \file
/// Defines graphics engine utilities

#include "../../GraphicsEngine/interface/Shader.h"

namespace Diligent
{

/// Shader source file substitute info.
struct ShaderSourceFileSubstitueInfo
{
    /// Source file name.
    const Char* Name DEFAULT_INITIALIZER(nullptr);

    /// Substitute file name.
    const Char* Substitute DEFAULT_INITIALIZER(nullptr);

#if DILIGENT_CPP_INTERFACE
    constexpr ShaderSourceFileSubstitueInfo() noexcept
    {}

    constexpr ShaderSourceFileSubstitueInfo(const Char* _Name, const Char* _Substitute) noexcept :
        Name{_Name},
        Substitute{_Substitute}
    {}
#endif
};
typedef struct ShaderSourceFileSubstitueInfo ShaderSourceFileSubstitueInfo;


/// Compound shader source factory create info.
struct CompoundShaderSourceFactoryCreateInfo
{
    /// An array of shader source input stream factories.
    IShaderSourceInputStreamFactory** ppFactories DEFAULT_INITIALIZER(nullptr);

    /// The number of factories in ppFactories array.
    UInt32 NumFactories DEFAULT_INITIALIZER(0);

    /// An array of shader source file substitutes.
    const ShaderSourceFileSubstitueInfo* pFileSubstitutes DEFAULT_INITIALIZER(nullptr);

    /// The number of file substitutes in pFileSubstitutes array.
    UInt32 NumFileSubstitutes DEFAULT_INITIALIZER(0);

#if DILIGENT_CPP_INTERFACE
    constexpr CompoundShaderSourceFactoryCreateInfo() noexcept
    {}

    constexpr CompoundShaderSourceFactoryCreateInfo(IShaderSourceInputStreamFactory**    _ppFactories,
                                                    UInt32                               _NumFactories,
                                                    const ShaderSourceFileSubstitueInfo* _pFileSubstitutes   = nullptr,
                                                    UInt32                               _NumFileSubstitutes = 0) noexcept :
        ppFactories{_ppFactories},
        NumFactories{_NumFactories},
        pFileSubstitutes{_pFileSubstitutes},
        NumFileSubstitutes{_NumFileSubstitutes}
    {}
#endif
};
typedef struct CompoundShaderSourceFactoryCreateInfo CompoundShaderSourceFactoryCreateInfo;


/// Creates a compound shader source factory.

/// \param [in]  CreateInfo - Compound shader source factory create info, see Diligent::CompoundShaderSourceFactoryCreateInfo.
/// \param [out] ppFactory  - Address of the memory location where the pointer to the created factory will be written.
///
/// Compound shader source stream factory is a wrapper around multiple shader source stream factories.
/// It is used to combine multiple shader source stream factories into a single one. When a source file
/// is requested, the factory will iterate over all factories in the array and return the first one that
/// returns a non-null stream.
///
/// The factory also allows substituting source file names. This is useful when the same shader source
/// is used for multiple shaders, but some of them require a modified version of the source.
void DILIGENT_GLOBAL_FUNCTION(CreateCompoundShaderSourceFactory)(const CompoundShaderSourceFactoryCreateInfo& CreateInfo,
                                                                 IShaderSourceInputStreamFactory**            ppFactory);


/// Shader source file info.
struct MemoryShaderSourceFileInfo
{
    /// File name.
    const Char* Name DEFAULT_INITIALIZER(nullptr);

    /// Shader source.
    const Char* pData DEFAULT_INITIALIZER(nullptr);

    /// Shader source length. If 0, the length will be calculated automatically
    /// assuming that the source is null-terminated.
    UInt32 Length DEFAULT_INITIALIZER(0);

#if DILIGENT_CPP_INTERFACE
    constexpr MemoryShaderSourceFileInfo() noexcept
    {}

    constexpr MemoryShaderSourceFileInfo(const Char* _Name,
                                         const Char* _pData,
                                         UInt32      _Length = 0) noexcept :
        Name{_Name},
        pData{_pData},
        Length{_Length}
    {}

    MemoryShaderSourceFileInfo(const Char*   _Name,
                               const String& Data) noexcept :
        Name{_Name},
        pData{Data.c_str()},
        Length{static_cast<UInt32>(Data.length())}
    {}

#endif
};
typedef struct MemoryShaderSourceFileInfo MemoryShaderSourceFileInfo;


/// Memory shader source factory create info.
struct MemoryShaderSourceFactoryCreateInfo
{
    /// An array of shader source files.
    const MemoryShaderSourceFileInfo* pSources DEFAULT_INITIALIZER(nullptr);

    /// The number of files in pSources array.
    UInt32 NumSources DEFAULT_INITIALIZER(0);

    /// Whether to copy shader sources. If false, the factory will assume that
    /// the source data will remain valid for the lifetime of the factory.
    Bool CopySources DEFAULT_INITIALIZER(False);

#if DILIGENT_CPP_INTERFACE
    constexpr MemoryShaderSourceFactoryCreateInfo() noexcept
    {}

    constexpr MemoryShaderSourceFactoryCreateInfo(const MemoryShaderSourceFileInfo* _pSources,
                                                  UInt32                            _NumSources,
                                                  bool                              _CopySources = false) noexcept :
        pSources{_pSources},
        NumSources{_NumSources},
        CopySources{_CopySources}
    {}
#endif
};
typedef struct MemoryShaderSourceFactoryCreateInfo MemoryShaderSourceFactoryCreateInfo;

/// Crates a memory shader source factory.
///
/// \param [in]  CreateInfo - Memory shader source factory create info, see Diligent::MemoryShaderSourceFactoryCreateInfo.
/// \param [out] ppFactory  - Address of the memory location where the pointer to the created factory will be written.
void CreateMemoryShaderSourceFactory(const MemoryShaderSourceFactoryCreateInfo& CreateInfo, IShaderSourceInputStreamFactory** ppFactory);


} // namespace Diligent
