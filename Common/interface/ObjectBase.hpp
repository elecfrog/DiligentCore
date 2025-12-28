#pragma once

/// \file
/// Implementation of the Diligent::ObjectBase template class

#include "../../Primitives/interface/Object.h"
#include "RefCountedObjectImpl.hpp"

namespace Diligent
{


#define IMPLEMENT_QUERY_INTERFACE_BODY(InterfaceID, ParentClassName) \
    {                                                                \
        if (ppInterface == nullptr)                                  \
            return;                                                  \
        if (IID == InterfaceID)                                      \
        {                                                            \
            *ppInterface = this;                                     \
            (*ppInterface)->AddRef();                                \
        }                                                            \
        else                                                         \
        {                                                            \
            ParentClassName::QueryInterface(IID, ppInterface);       \
        }                                                            \
    }

#define IMPLEMENT_QUERY_INTERFACE(ClassName, InterfaceID, ParentClassName)         \
    void ClassName::QueryInterface(const INTERFACE_ID& IID, IObject** ppInterface) \
        IMPLEMENT_QUERY_INTERFACE_BODY(InterfaceID, ParentClassName)

// clang-format off
#define IMPLEMENT_QUERY_INTERFACE_IN_PLACE(InterfaceID, ParentClassName)                                    \
    virtual void DG_CALL_TYPE QueryInterface(const INTERFACE_ID& IID, IObject** ppInterface) override \
        IMPLEMENT_QUERY_INTERFACE_BODY(InterfaceID, ParentClassName)                                        \
        using IObject::QueryInterface;
// clang-format on

#define IMPLEMENT_QUERY_INTERFACE2_BODY(InterfaceID1, InterfaceID2, ParentClassName) \
    {                                                                                \
        if (ppInterface == nullptr)                                                  \
            return;                                                                  \
        if (IID == InterfaceID1 || IID == InterfaceID2)                              \
        {                                                                            \
            *ppInterface = this;                                                     \
            (*ppInterface)->AddRef();                                                \
        }                                                                            \
        else                                                                         \
        {                                                                            \
            ParentClassName::QueryInterface(IID, ppInterface);                       \
        }                                                                            \
    }

#define IMPLEMENT_QUERY_INTERFACE2(ClassName, InterfaceID1, InterfaceID2, ParentClassName) \
    void ClassName::QueryInterface(const INTERFACE_ID& IID, IObject** ppInterface)         \
        IMPLEMENT_QUERY_INTERFACE2_BODY(InterfaceID1, InterfaceID2, ParentClassName)

// clang-format off
#define IMPLEMENT_QUERY_INTERFACE2_IN_PLACE(InterfaceID1, InterfaceID2, ParentClassName)                    \
    virtual void DG_CALL_TYPE QueryInterface(const INTERFACE_ID& IID, IObject** ppInterface) override \
        IMPLEMENT_QUERY_INTERFACE2_BODY(InterfaceID1, InterfaceID2, ParentClassName)                        \
        using IObject::QueryInterface;
// clang-format on

/// Template class implementing base functionality for an object
template <typename BaseInterface>
class ObjectBase : public RefCountedObject<BaseInterface>
{
public:
    explicit ObjectBase(IReferenceCounters* pRefCounters) noexcept :
        RefCountedObject<BaseInterface>{pRefCounters}
    {}

    virtual void DG_CALL_TYPE QueryInterface(const INTERFACE_ID& IID, IObject** ppInterface)
    {
        if (ppInterface == nullptr)
            return;

        *ppInterface = nullptr;
        if (IID == IID_Unknown)
        {
            *ppInterface = this;
            (*ppInterface)->AddRef();
        }
    }
};

} // namespace Diligent
