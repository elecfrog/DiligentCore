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

#include "PlatformMisc.hpp"

#include "gtest/gtest.h"

using namespace Diligent;

namespace
{

template <class PlatformClass>
void TestMSB_LSB()
{
    EXPECT_EQ(PlatformClass::GetMSB(UInt32{0}), UInt32{32});
    for (UInt32 i = 0; i < 32; ++i)
    {
        auto MSB = PlatformClass::GetMSB((UInt32{1} << i) | 1);
        EXPECT_EQ(MSB, i);
    }

    EXPECT_EQ(PlatformClass::GetMSB(UInt64{0}), UInt64{64});
    for (UInt32 i = 0; i < 64; ++i)
    {
        auto MSB = PlatformClass::GetMSB((UInt64{1} << i) | 1);
        EXPECT_EQ(MSB, i);
    }

    EXPECT_EQ(PlatformClass::GetLSB(UInt32{0}), UInt32{32});
    for (UInt32 i = 0; i < 32; ++i)
    {
        auto LSB = PlatformClass::GetLSB((UInt32{1} << i) | (UInt32{1} << 31));
        EXPECT_EQ(LSB, i);
    }

    EXPECT_EQ(PlatformClass::GetLSB(UInt64{0}), UInt64{64});
    for (UInt32 i = 0; i < 64; ++i)
    {
        auto LSB = PlatformClass::GetLSB((UInt64{1} << i) | (UInt64{1} << 63));
        EXPECT_EQ(LSB, i);
    }
}

TEST(Platforms_PlatformMisc, GetMsbLsb)
{
    TestMSB_LSB<PlatformMisc>();
    TestMSB_LSB<BasicPlatformMisc>();
}

template <class PlatformClass>
void TestCountOneBits()
{
    EXPECT_EQ(PlatformClass::CountOneBits(UInt32{0}), UInt32{0});
    EXPECT_EQ(PlatformClass::CountOneBits(UInt64{0}), UInt64{0});
    EXPECT_EQ(PlatformClass::CountOneBits(UInt32{1}), UInt32{1});
    EXPECT_EQ(PlatformClass::CountOneBits(UInt64{1}), UInt64{1});
    EXPECT_EQ(PlatformClass::CountOneBits(UInt32{7}), UInt32{3});
    EXPECT_EQ(PlatformClass::CountOneBits(UInt64{7}), UInt64{3});
    EXPECT_EQ(PlatformClass::CountOneBits((UInt32{1} << 31) | (UInt32{1} << 15)), UInt32{2});
    EXPECT_EQ(PlatformClass::CountOneBits((UInt64{1} << 63) | (UInt32{1} << 31)), UInt64{2});
    EXPECT_EQ(PlatformClass::CountOneBits((UInt32{1} << 31) - 1), UInt32{31});
    EXPECT_EQ(PlatformClass::CountOneBits((UInt64{1} << 63) - 1), UInt64{63});
}

TEST(Platforms_PlatformMisc, CountOneBits)
{
    TestCountOneBits<PlatformMisc>();
    TestCountOneBits<BasicPlatformMisc>();
}

template <typename PlatformClass>
void TestSwapBytes()
{
    EXPECT_EQ(PlatformClass::SwapBytes(UInt64{0x0102030405060708}), UInt64{0x0807060504030201});
    EXPECT_EQ(PlatformClass::SwapBytes(Int64{0x0102030405060708}), Int64{0x0807060504030201});
    EXPECT_EQ(PlatformClass::SwapBytes(UInt32{0x01020304}), UInt32{0x04030201});
    EXPECT_EQ(PlatformClass::SwapBytes(Int32{0x01020304}), Int32{0x04030201});
    EXPECT_EQ(PlatformClass::SwapBytes(UInt16{0x0102}), UInt16{0x0201});
    EXPECT_EQ(PlatformClass::SwapBytes(Int16{0x0102}), Int16{0x0201});
}

TEST(Platforms_PlatformMisc, SwapBytes)
{
    TestSwapBytes<PlatformMisc>();
    TestSwapBytes<BasicPlatformMisc>();

    constexpr float f     = 1234.5678f;
    const auto      fswap = PlatformMisc::SwapBytes(f);
    EXPECT_EQ(PlatformMisc::SwapBytes(fswap), f);
}

} // namespace
