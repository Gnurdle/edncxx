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
#include <iostream>
#include <edncxx/utf8reader.h>
#include <edncxx/ednreader.h>
#include <edncxx/ednany.h>
using namespace edncxx;
using namespace std;

TEST(ednreader, TestEmpty)
{
    std::istringstream strm("");
    edncxx::Utf8Reader ureader(strm);
    EXPECT_FALSE(edncxx::readValue(ureader));
}

TEST(ednreader, nil)
{
    std::string str(" nil ");
    std::istringstream strm(str);
    Utf8Reader rdr(strm);
    if(auto optval = readValue(rdr)){
        EXPECT_TRUE( is<NilType>(*optval));
        EXPECT_FALSE(readValue(rdr));
    } else FAIL();

}

#ifdef NOPE

TEST(ednreader, bool)
{
    std::string str(" true         false");
    std::istringstream strm(str);
    Utf8Reader rdr(strm);
    auto got = readValue(rdr);
    EXPECT_TRUE( got && is<BoolType>(*got) && std::any_cast<BoolType>(got));
    got = readValue(rdr);
    EXPECT_TRUE( got && is<BoolType>(*got) && !std::any_cast<BoolType>(got));
    EXPECT_FALSE(readValue(rdr));
}
#endif 
