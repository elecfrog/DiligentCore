/*
 *  Copyright 2019-2025 Diligent Graphics LLC
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

#include <array>

#include "Align.hpp"

#include "gtest/gtest.h"

using namespace Diligent;

namespace
{

TEST(Common_Align, IsPowerOfTwo)
{
    for (UInt32 i = 0; i < 256; ++i)
    {
        bool IsPw2 = (i == 1 || i == 2 || i == 4 || i == 8 || i == 16 || i == 32 || i == 64 || i == 128);
        EXPECT_EQ(IsPowerOfTwo(static_cast<UInt8>(i)), IsPw2);
        EXPECT_EQ(IsPowerOfTwo(static_cast<UInt16>(i)), IsPw2);
        EXPECT_EQ(IsPowerOfTwo(i), IsPw2);
    }

    for (UInt32 bit = 0; bit < 32; ++bit)
    {
        auto Pw2 = UInt32{1} << bit;
        EXPECT_TRUE(IsPowerOfTwo(Pw2));
        EXPECT_EQ(IsPowerOfTwo(Pw2 + 1), Pw2 == 1);
        EXPECT_EQ(IsPowerOfTwo(Pw2 - 1), Pw2 == 2);
    }

    for (UInt64 bit = 0; bit < 64; ++bit)
    {
        auto Pw2 = UInt64{1} << bit;
        EXPECT_TRUE(IsPowerOfTwo(Pw2));
        EXPECT_EQ(IsPowerOfTwo(Pw2 + 1), Pw2 == 1);
        EXPECT_EQ(IsPowerOfTwo(Pw2 - 1), Pw2 == 2);
    }
}

TEST(Common_Align, AlignUp)
{
    EXPECT_EQ(AlignUp(UInt8{0}, UInt8{16}), UInt8{0});
    EXPECT_EQ(AlignUp(UInt8{1}, UInt8{16}), UInt8{16});
    EXPECT_EQ(AlignUp(UInt8{15}, UInt8{16}), UInt8{16});
    EXPECT_EQ(AlignUp(UInt8{16}, UInt8{16}), UInt8{16});
    EXPECT_EQ(AlignUp(UInt8{17}, UInt8{16}), UInt8{32});

    EXPECT_EQ(AlignUp(UInt8{17}, UInt32{1024}), UInt32{1024});
    EXPECT_EQ(AlignUp(UInt16{400}, UInt8{128}), UInt16{512});

    for (UInt32 i = 0; i < 1024; ++i)
    {
        constexpr UInt32 Alignment = 16;

        auto Aligned = (i / Alignment) * Alignment;
        if (Aligned < i) Aligned += Alignment;

        EXPECT_EQ(AlignUp(i, Alignment), Aligned);
    }

    EXPECT_EQ(AlignUp((UInt64{1} << 63) + 1, UInt64{1024}), (UInt64{1} << 63) + 1024);
}

TEST(Common_Align, AlignDown)
{
    EXPECT_EQ(AlignDown(UInt8{0}, UInt8{16}), UInt8{0});
    EXPECT_EQ(AlignDown(UInt8{1}, UInt8{16}), UInt8{0});
    EXPECT_EQ(AlignDown(UInt8{15}, UInt8{16}), UInt8{0});
    EXPECT_EQ(AlignDown(UInt8{16}, UInt8{16}), UInt8{16});
    EXPECT_EQ(AlignDown(UInt8{17}, UInt8{16}), UInt8{16});

    EXPECT_EQ(AlignDown(UInt16{519}, UInt8{128}), UInt16{512});
    EXPECT_EQ(AlignDown(UInt8{127}, UInt32{1024}), UInt32{0});

    for (UInt32 i = 0; i < 1024; ++i)
    {
        constexpr UInt32 Alignment = 16;

        auto Aligned = (i / Alignment) * Alignment;

        EXPECT_EQ(AlignDown(i, Alignment), Aligned);
    }

    EXPECT_EQ(AlignDown((UInt64{1} << 63) + 1023, UInt64{1024}), (UInt64{1} << 63));
}

TEST(Common_Align, AlignPtr)
{
    EXPECT_EQ(AlignUp((void*)0x1000, size_t{16}), (void*)0x1000);
    EXPECT_EQ(AlignUp((void*)0x1001, size_t{16}), (void*)0x1010);
    EXPECT_EQ(AlignUp((void*)0x100F, size_t{16}), (void*)0x1010);
    EXPECT_EQ(AlignUp((void*)0x1010, size_t{16}), (void*)0x1010);
    EXPECT_EQ(AlignUp((void*)0x1011, size_t{16}), (void*)0x1020);

    for (uintptr_t i = 0; i < 1024; ++i)
    {
        constexpr uintptr_t Alignment = 16;

        auto Aligned = (i / Alignment) * Alignment;
        if (Aligned < i) Aligned += Alignment;

        EXPECT_EQ(AlignUp((void*)(0x1000 + i), (size_t)Alignment), (void*)(0x1000 + Aligned));
    }
}

TEST(Common_Align, AlignDownPtr)
{
    EXPECT_EQ(AlignDown((void*)0x1000, size_t{16}), (void*)0x1000);
    EXPECT_EQ(AlignDown((void*)0x1001, size_t{16}), (void*)0x1000);
    EXPECT_EQ(AlignDown((void*)0x100F, size_t{16}), (void*)0x1000);
    EXPECT_EQ(AlignDown((void*)0x1010, size_t{16}), (void*)0x1010);
    EXPECT_EQ(AlignDown((void*)0x1011, size_t{16}), (void*)0x1010);

    for (uintptr_t i = 0; i < 1024; ++i)
    {
        constexpr uintptr_t Alignment = 16;

        auto Aligned = (i / Alignment) * Alignment;

        EXPECT_EQ(AlignDown((void*)(0x1000 + i), (size_t)Alignment), (void*)(0x1000 + Aligned));
    }
}

TEST(Common_Align, AlignDownNonPw2)
{
    EXPECT_EQ(AlignDownNonPw2(UInt8{0}, UInt8{17}), UInt8{0});
    EXPECT_EQ(AlignDownNonPw2(UInt16{1}, UInt8{15}), UInt16{0});
    EXPECT_EQ(AlignDownNonPw2(UInt32{14}, UInt8{15}), UInt32{0});
    EXPECT_EQ(AlignDownNonPw2(Int8{15}, Int16{15}), Int16{15});
    EXPECT_EQ(AlignDownNonPw2(Int32{16}, Int16{15}), Int32{15});

    EXPECT_EQ(AlignDownNonPw2(Int8{127}, Int16{531}), Int16{0});
    EXPECT_EQ(AlignDownNonPw2(Int32{1023}, Int8{119}), Int32{952});

    for (UInt32 i = 0; i < 1024; ++i)
    {
        constexpr UInt32 Alignment = 17;

        auto Aligned = (i / Alignment) * Alignment;

        EXPECT_EQ(AlignDownNonPw2(i, Alignment), Aligned);
    }

    EXPECT_EQ(AlignDownNonPw2((UInt64{1} << 63) + 1023, UInt64{1024}), (UInt64{1} << 63));
}

TEST(Common_Align, AlignUpNonPw2)
{
    EXPECT_EQ(AlignUpNonPw2(UInt8{0}, UInt8{17}), UInt8{0});
    EXPECT_EQ(AlignUpNonPw2(UInt16{1}, UInt8{15}), UInt16{15});
    EXPECT_EQ(AlignUpNonPw2(UInt32{14}, UInt16{15}), UInt32{15});
    EXPECT_EQ(AlignUpNonPw2(Int8{15}, Int32{15}), Int32{15});
    EXPECT_EQ(AlignUpNonPw2(Int16{16}, Int8{15}), Int16{30});

    EXPECT_EQ(AlignUpNonPw2(Int8{15}, Int32{1125}), Int32{1125});
    EXPECT_EQ(AlignUpNonPw2(Int32{325}, Int8{113}), Int32{339});

    for (UInt32 i = 0; i < 1024; ++i)
    {
        constexpr UInt32 Alignment = 15;

        auto Aligned = (i / Alignment) * Alignment;
        if (Aligned < i) Aligned += Alignment;

        EXPECT_EQ(AlignUpNonPw2(i, Alignment), Aligned);
    }

    EXPECT_EQ(AlignUpNonPw2((UInt64{1} << 63) + 1, UInt64{1024}), (UInt64{1} << 63) + 1024);
}

template <typename T>
void TestAlignUpToPowerOfTwo()
{
    EXPECT_EQ(AlignUpToPowerOfTwo(T{0}), T{0});
    EXPECT_EQ(AlignUpToPowerOfTwo(T{1}), T{1});
    EXPECT_EQ(AlignUpToPowerOfTwo(T{2}), T{2});

    for (T i = 2; i < sizeof(T) * 8 - 1; ++i)
    {
        T Pw2   = T{1} << i;
        T Test1 = Pw2 - T{1};
        T Test2 = (Pw2 >> T{1}) + T{1};
        EXPECT_EQ(AlignUpToPowerOfTwo(Pw2), Pw2);
        EXPECT_EQ(AlignUpToPowerOfTwo(Test1), Pw2);
        EXPECT_EQ(AlignUpToPowerOfTwo(Test2), Pw2);
    }
}

TEST(Common_Align, AlignUpToPowerOfTwo)
{
    TestAlignUpToPowerOfTwo<UInt8>();
    TestAlignUpToPowerOfTwo<UInt16>();
    TestAlignUpToPowerOfTwo<UInt32>();
    TestAlignUpToPowerOfTwo<UInt64>();
}

template <typename T>
void TestAlignDownToPowerOfTwo()
{
    EXPECT_EQ(AlignDownToPowerOfTwo(T{0}), T{0});
    EXPECT_EQ(AlignDownToPowerOfTwo(T{1}), T{1});
    EXPECT_EQ(AlignDownToPowerOfTwo(T{2}), T{2});

    for (T i = 2; i < sizeof(T) * 8 - 1; ++i)
    {
        T Pw2   = T{1} << i;
        T Pw2_1 = Pw2 >> T{1};
        T Test1 = Pw2 - T{1};
        T Test2 = Pw2_1 + T{1};
        EXPECT_EQ(AlignDownToPowerOfTwo(Pw2), Pw2);
        EXPECT_EQ(AlignDownToPowerOfTwo(Test1), Pw2_1);
        EXPECT_EQ(AlignDownToPowerOfTwo(Test2), Pw2_1);
    }
}

TEST(Common_Align, AlignDownToPowerOfTwo)
{
    TestAlignDownToPowerOfTwo<UInt8>();
    TestAlignDownToPowerOfTwo<UInt16>();
    TestAlignDownToPowerOfTwo<UInt32>();
    TestAlignDownToPowerOfTwo<UInt64>();
}

} // namespace
