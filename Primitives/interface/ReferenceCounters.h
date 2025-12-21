#pragma once

/// \file
/// Defines Diligent::IReferenceCounters interface

#include "InterfaceID.h"

namespace Diligent {

typedef Int32 ReferenceCounterValueType;


/// Base interface for a reference counter object that stores the number of strong and
/// weak references and the pointer to the object. It is necessary to separate reference
/// counters from the object to support weak pointers.
class IReferenceCounters
{
public:
    /// Increments the number of strong references by 1.

    /// \return The number of strong references after incrementing the counter.
    /// \remark The method is thread-safe and does not require explicit synchronization.
    /// \note   In a multithreaded environment, the returned number may not be reliable
    ///         as other threads may simultaneously change the actual value of the counter.
    virtual ReferenceCounterValueType AddStrongRef() = 0;


    /// Decrements the number of strong references by 1 and destroys the referenced object
    /// when the counter reaches zero. If there are no more weak references, destroys the
    /// reference counters object itself.

    /// \return The number of strong references after decrementing the counter.
    /// \remark The referenced object is destroyed when the last strong reference is released.\n
    ///         If there are no more weak references, the reference counters object itself is
    ///         also destroyed.\n
    ///         The method is thread-safe and does not require explicit synchronization.
    /// \note   In a multithreaded environment, the returned number may not be reliable
    ///         as other threads may simultaneously change the actual value of the counter.
    ///         The only reliable value is 0 as the object is destroyed when the last
    ///         strong reference is released.
    virtual ReferenceCounterValueType ReleaseStrongRef() = 0;


    /// Increments the number of weak references by 1.

    /// \return The number of weak references after incrementing the counter.
    /// \remark The method is thread-safe and does not require explicit synchronization.
    /// \note   In a multithreaded environment, the returned number may not be reliable
    ///         as other threads may simultaneously change the actual value of the counter.
    virtual ReferenceCounterValueType AddWeakRef() = 0;


    /// Decrements the number of weak references by 1. If there are no more strong and weak
    /// references, destroys the reference counters object itself.

    /// \return The number of weak references after decrementing the counter.
    /// \remark The method is thread-safe and does not require explicit synchronization.
    /// \note   In a multithreaded environment, the returned number may not be reliable
    ///         as other threads may simultaneously change the actual value of the counter.
    virtual ReferenceCounterValueType ReleaseWeakRef() = 0;


    /// Queries a pointer to the IUnknown interface of the referenced object.

    /// \param [out] ppObject - Memory address where the pointer to the object
    ///                         will be stored.
    /// \remark If the object was destroyed, nullptr will be written to *ppObject.
    ///         If the object was not released, the pointer to the object's IUnknown interface
    ///         will be stored. In this case, the number of strong references to the object
    ///         will be incremented by 1.\n
    ///         The method is thread-safe and does not require explicit synchronization.
    virtual void QueryObject(struct IObject** ppObject) = 0;


    /// Returns the number of outstanding strong references.

    /// \return The number of strong references.
    /// \note   In a multithreaded environment, the returned number may not be reliable
    ///         as other threads may simultaneously change the actual value of the counter.
    ///         The only reliable value is 0 as the object is destroyed when the last
    ///         strong reference is released.
    virtual ReferenceCounterValueType GetNumStrongRefs() const = 0;


    /// Returns the number of outstanding weak references.

    /// \return The number of weak references.
    /// \note   In a multithreaded environment, the returned number may not be reliable
    ///         as other threads may simultaneously change the actual value of the counter.
    virtual ReferenceCounterValueType GetNumWeakRefs() const = 0;
};

 } // namespace Diligent
