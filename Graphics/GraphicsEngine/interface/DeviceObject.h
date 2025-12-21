#pragma once

/// \file
/// Defines Diligent::IDeviceObject interface

#include "../../../Primitives/interface/Object.h"
#include "GraphicsTypes.h"

namespace Diligent
{


// {5B4CCA0B-5075-4230-9759-F48769EE5502}
static constexpr INTERFACE_ID IID_DeviceObject =
    {0x5b4cca0b, 0x5075, 0x4230, {0x97, 0x59, 0xf4, 0x87, 0x69, 0xee, 0x55, 0x2}};

#include "../../../Primitives/interface/DefineInterfaceHelperMacros.h"
#define IDeviceObjectInclusiveMethods \
    IObjectInclusiveMethods;          \
    IDeviceObjectMethods DeviceObject


/// Base interface for all objects created by the render device Diligent::IRenderDevice
struct IDeviceObject : public IObject
{
    /// Returns the object description
    virtual const DeviceObjectAttribs& METHOD(GetDesc)() const = 0;


    /// Returns unique identifier assigned to an object

    /// Unique identifiers can be used to reliably check if two objects are identical.
    /// Note that the engine reuses memory reclaimed after an object has been released.
    /// For example, if a texture object is released and then another texture is created,
    /// the engine may return the same pointer, so pointer-to-pointer comparisons are not
    /// reliable. Unique identifiers, on the other hand, are guaranteed to be, well, unique.
    ///
    /// Unique identifiers are object-specifics, so, for instance, buffer identifiers
    /// are not comparable to texture identifiers.
    ///
    /// Unique identifiers are only meaningful within one session. After an application
    /// restarts, all identifiers become invalid.
    ///
    /// Valid identifiers are always positive values. Zero and negative values can never be
    /// assigned to an object and are always guaranteed to be invalid.
    virtual Int32 METHOD(GetUniqueID)() const = 0;


    /// Stores a pointer to the user-provided data object.

    /// The data may later be retrieved through GetUserData().
    ///
    /// \param [in] pUserData - Pointer to the user data object to store.
    ///
    /// \note   The method is not thread-safe and an application
    ///         must externally synchronize the access.
    ///
    /// The method keeps strong reference to the user data object.
    /// If an application needs to release the object, it
    /// should call SetUserData(nullptr);
    virtual void CALLTYPE SetUserData(IObject* pUserData) = 0;


    /// Returns a pointer to the user data object previously set with SetUserData() method.

    /// \return     The pointer to the user data object.
    ///
    /// \remarks    The method does **NOT** call AddRef()
    ///             for the object being returned.
    virtual IObject* CALLTYPE GetUserData() const = 0;
};




} // namespace Diligent
