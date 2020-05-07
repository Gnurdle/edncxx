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
#include <edncxx/utf8reader.h>
#include <sstream>
#include <string>

TEST(utf8, TestSevenBitUtf){

    // make a buffer of the 7-bit chars
    std::string buf(128, 0);
    for(auto ix=0; ix<128; ix++)
        buf[ix] = static_cast<char>(ix);

    // feed them via stream to utf8 reader
    std::istringstream strm(buf);
    edncxx::Utf8Reader ureader(strm);

    // now read from the reader, should get same count back
    char32_t got[128];
    auto ngot = ureader.read(&got[0], 128);
    EXPECT_EQ(ngot, 128);


    // make sure we got what we sent  
    for(auto ix=0; ix<128; ix++){
        EXPECT_EQ(got[ix], ix);
    }

    // reading now should hit end
    EXPECT_EQ(ureader.read(&got[0], 3), 0);
}

TEST(utf8, WikipediaSamples){
    auto verify = [](char32_t codept, const std::string& input)->bool{
        std::istringstream strm(input);
        edncxx::Utf8Reader ureader(strm);
        char32_t cpfound;
        auto ngot = ureader.read(&cpfound, 1);
        return (ngot == 1) && (cpfound == codept) &&
            (ureader.read(&cpfound, 1) == 0); 
    };
    EXPECT_TRUE( verify(0x0024,  "\x24" ));
    EXPECT_TRUE( verify(0x00a2,  "\xc2\xa2" ));
    EXPECT_TRUE( verify(0x00a2,  "\xc2\xa2" ));
    EXPECT_TRUE( verify(0x0939,  "\xe0\xa4\xb9"));
    EXPECT_TRUE( verify(0x20ac,  "\xe2\x82\xac"));
    EXPECT_TRUE( verify(0xd55c,  "\xed\x95\x9c"));
    EXPECT_TRUE( verify(0x10348, "\xf0\x90\x8d\x88"));
}
