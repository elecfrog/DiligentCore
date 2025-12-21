#pragma once

/// \file
/// Defines Diligent::IFileStream interface

#include "Object.h"
#include "DataBlob.h"

namespace Diligent
{

/// IFileStream interface unique identifier
// {E67F386C-6A5A-4A24-A0CE-C66435465D41}
static constexpr struct INTERFACE_ID IID_FileStream =
    {0xe67f386c, 0x6a5a, 0x4a24, {0xa0, 0xce, 0xc6, 0x64, 0x35, 0x46, 0x5d, 0x41}};


#include "DefineInterfaceHelperMacros.h"

#define IFileStreamInclusiveMethods \
    IObjectInclusiveMethods;        \
    IFileStreamMethods FileStream

/// Base interface for a file stream
struct IFileStream : public IObject
{
    /// Reads data from the stream
    virtual bool CALLTYPE Read(void* Data, size_t BufferSize) = 0;

    virtual void METHOD(ReadBlob)(IDataBlob* pData) = 0;

    /// Writes data to the stream
    virtual bool METHOD(Write)(const void* Data, size_t Size) = 0;

    virtual size_t METHOD(GetSize)() = 0;

    virtual size_t METHOD(GetPos)() = 0;

    virtual bool METHOD(SetPos)(size_t Offset, int Origin) = 0;

    virtual bool METHOD(IsValid)() = 0;
};

} // namespace Diligent
