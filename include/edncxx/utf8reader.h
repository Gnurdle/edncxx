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

#pragma once
#include <istream>
#include <vector>
#include <utility>
#include <string>
#include <functional>

namespace edncxx{

    // Utf8Reader wraps an std::istream& and yields buffers of char32_t 
    class Utf8Reader{
    public:
        explicit Utf8Reader(std::istream& source); 
        virtual ~Utf8Reader();
        char32_t get();
        void unget(char32_t);
        void unget(const std::u32string_view&);
        std::u32string getWhile(std::function<bool(char32_t)> pred);
        std::u32string getUntil(std::function<bool(char32_t)> pred);
        const std::pair<unsigned, unsigned> loc() const { return _loc; }

    public:
        std::istream& _source;
        std::vector<char32_t> _pushback;
        std::pair<unsigned, unsigned> _loc;
    };
}