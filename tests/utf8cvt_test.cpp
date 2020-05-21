// The MIT License (MIT)
//
// Copyright (c) 2020 Clay Hopperdietzel (aka Gnurdle)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <gtest/gtest.h>
#include <edncxx/utf8cvt.h>
using namespace edncxx;

TEST(utf8cvt, TransparentSevenBit){

    // make a buffer of the 7-bit chars
    std::string string8(128, 0);
    for(auto ix=0; ix<128; ix++)
        string8[ix] = static_cast<char>(ix);

    std::u32string string32(128, 0);
    for(auto ix=0; ix<128; ix++)
        string32[ix] = static_cast<char32_t>(ix);

    EXPECT_EQ(string32, decodeUtf8(string8));
    EXPECT_EQ(string8, encodeUtf8(string32));
}

TEST(utf8cvt, WikipediaSamples){

    auto verify = [](char32_t codept, const std::string& input)->bool{

        std::u32string cpstr(1, codept);
        auto encoded = encodeUtf8(cpstr);
        return (cpstr == decodeUtf8(input) && (encoded == input));
    };
    EXPECT_TRUE( verify(0x0024,  "\x24" ));
    EXPECT_TRUE( verify(0x00a2,  "\xc2\xa2" ));
    EXPECT_TRUE( verify(0x0939,  "\xe0\xa4\xb9"));
    EXPECT_TRUE( verify(0x20ac,  "\xe2\x82\xac"));
    EXPECT_TRUE( verify(0xd55c,  "\xed\x95\x9c"));
    EXPECT_TRUE( verify(0x10348, "\xf0\x90\x8d\x88"));
}

TEST(utf8cvt, TwoWayString)
{
    std::u32string s32 = {0x0024, 0x00a2, 0x0939, 0x20ac, 0xd55c, 0x10348};
    std::string s8 = "\x24\xc2\xa2\xe0\xa4\xb9\xe2\x82\xac\xed\x95\x9c\xf0\x90\x8d\x88";

    EXPECT_EQ(edncxx::encodeUtf8(s32), s8);
    EXPECT_EQ(edncxx::decodeUtf8(s8), s32);
}

