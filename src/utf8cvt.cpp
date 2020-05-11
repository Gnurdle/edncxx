
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

#include <edncxx/utf8cvt.h>

#include <locale>
#include <sstream>
#include <stdexcept>

using namespace edncxx;
namespace edncxx{

class MyCodeCvt : public std::codecvt<char32_t, char, std::mbstate_t>{
};

std::string encodeUtf8(const std::u32string& from)
{
    MyCodeCvt cvt;
    std::mbstate_t state = {};

    std::string to(4*from.size(), 0);
    char* to_next;
    const char32_t* from_next;
    auto rc = cvt.out(state, from.data(), from.data() + from.size(), from_next,
                      to.data(), to.data()+to.size(), to_next);
    if(rc != MyCodeCvt::ok){
        std::ostringstream msg;
        msg << "encodeUtf8 returned unexpected state: " << int(rc);
        throw std::runtime_error(msg.str());
    }
    return to.substr(0, to_next - to.data());
}

std::u32string decodeUtf8(const std::string& from)
{
    MyCodeCvt cvt;
    std::mbstate_t state = {};
    std::u32string to(from.size(), 0);
    char32_t* to_next;
    const char* from_next;
    auto rc = cvt.in(state, from.data(), from.data() + from.size(), from_next,
                    to.data(), to.data()+to.size(), to_next);
    if(rc != MyCodeCvt::ok){
        std::ostringstream msg;
        msg << "decodeUtf8 returned unexpected state: " << int(rc);
        throw std::runtime_error(msg.str());
    }
    return to.substr(0, to_next - to.data());
}
} // ns