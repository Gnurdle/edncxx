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

#include <edncxx/utf8reader.h>

using namespace edncxx;

Utf8Reader::Utf8Reader(std::istream& source)
    : _source(source)
{}

Utf8Reader::~Utf8Reader()
{}

// adapted from:

// Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

static const char32_t UTF8_ACCEPT = 0;
static const char32_t UTF8_REJECT = 1;

static char32_t decode(char32_t& state, char32_t& codep, char32_t abyte)
 {
    static const uint8_t utf8d[] = {
        // The first part of the table maps bytes to character classes that
        // to reduce the size of the transition table and create bitmasks.
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

        // The second part is a transition table that maps a combination
        // of a state of the automaton and a character class to a state.
        0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
        12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
        12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
        12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
        12,36,12,12,12,12,12,12,12,12,12,12, 
        };

    char32_t type = utf8d[abyte];
    codep = (state != UTF8_ACCEPT) ?
        (abyte & 0x3fu) | (codep << 6) :
        (0xff >> type) & (abyte);

    state = utf8d[256 + state + type];
    return state;
}

char32_t Utf8Reader::get()
{
    if(!_pushback.empty()){
        auto ch = _pushback.back();
        _pushback.pop_back();
        return ch;
    }

    char32_t state = UTF8_ACCEPT;
    char32_t codep = 0;
    char32_t abyte = 0;

    do{
        abyte = _source.get();
        if(_source.eof() || _source.fail())
            return char32_t(-1);

    } while (decode(state, codep, abyte));
    return codep;
}

char32_t Utf8Reader::peek()
{
    auto ch = get();
    unget(ch);
    return ch;
}

void Utf8Reader::unget(char32_t ch)
{
    if(ch != char32_t(-1))
        _pushback.push_back(ch);
}

void Utf8Reader::unget(const std::u32string_view& str)
{
    for(auto it=str.rbegin(); it != str.rend(); ++it)
        unget(*it);
}

std::u32string Utf8Reader::getWhile(std::function<bool(char32_t)> pred)
{
    std::u32string result;
    while(true){
        auto ch = get();
        if(pred(ch))
            result.push_back(ch);
        else{
            unget(ch);
            break;
        }
    }
    return result;
}

std::u32string Utf8Reader::getUntil(std::function<bool(char32_t)> pred)
{
    std::u32string result;
    while(true){
        auto ch = get();
        if(pred(ch)){
            unget(ch);
            return result;
        }
        result.push_back(ch);
    }
}


