/*
 *  Copyright 2019-2022 Diligent Graphics LLC
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

#include "Primitives.h"
#include "BasicPlatformDebug.hpp"

// Use Sparrow's assertion system (Sparrow implementation takes priority)
#include <core/assertions.hpp>

#ifdef SPW_DEBUG

#    include <typeinfo>


// CheckDynamicType - Diligent-specific dynamic type checking utility
template <typename DstType, typename SrcType>
void CheckDynamicType(SrcType* pSrcPtr)
{
    VERIFY(pSrcPtr == nullptr || dynamic_cast<DstType*>(pSrcPtr) != nullptr, "Dynamic type cast failed. Src typeid: \'", typeid(*pSrcPtr).name(), "\' Dst typeid: \'", typeid(DstType).name(), '\'');
}

#    define CHECK_DYNAMIC_TYPE(DstType, pSrcPtr) \
        do                                       \
        {                                        \
            CheckDynamicType<DstType>(pSrcPtr);  \
        } while (false)

#else

// clang-format off
#    define CHECK_DYNAMIC_TYPE(...) do{}while(false)
// clang-format on

#endif

#if defined(SPW_DEBUG)
#    define DEV_CHECK_ERR VERIFY
#elif defined(SPW_PROFILE)
#    define DEV_CHECK_ERR CHECK_ERR
#else
// clang-format off
#    define DEV_CHECK_ERR(...)do{}while(false)
// clang-format on
#endif

#define DEV_ERROR(...) DEV_CHECK_ERR(false, __VA_ARGS__)

#ifdef SPW_PROFILE

#    define DEV_CHECK_WARN CHECK_WARN
#    define DEV_CHECK_INFO CHECK_INFO

#else

// clang-format off
#    define DEV_CHECK_WARN(...)do{}while(false)
#    define DEV_CHECK_INFO(...)do{}while(false)
// clang-format on

#endif
