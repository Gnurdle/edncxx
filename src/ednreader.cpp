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


#include <edncxx/ednreader.h>
#include <edncxx/utf8reader.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <optional>

#include <edncxx/ednany.h>

using namespace edncxx;
using namespace std;
namespace edncxx{

static bool iswhitespace(char32_t ch)
{
    return ch == U' ' || ch == U'\t' || ch == U'\r' || ch == U'\n' || ch == U',';
}

static bool isterminator(char32_t ch)
{
    return ch == U'(' || ch == U')' || 
           ch == U'[' || ch == U']' || 
           ch == U'{' || ch == U'}' || 
           ch == char32_t(-1) ||
           iswhitespace(ch);
}

static void eatwhitespace(Utf8Reader& r)
{
    while(true){
        auto ch = r.get();
        if(!iswhitespace(ch)){
            r.unget(ch);
            break;
        }
    }
}

static void eatcomment(Utf8Reader& r)
{
    r.getUntil([](char32_t ch){ return (ch == U'\n'); });
}

static void boom(const std::string_view& what) 
{
    std::ostringstream msg;
    msg << "Sombody forgot to implement: " << what;
    throw std::runtime_error(msg.str());
}

static bool tokenmatch(Utf8Reader& r, const std::string_view& text)
{
    std::u32string buf;
    for(auto want : text){
        auto got = r.get();
        if(got == -1 || got != want || isterminator(got)){
            r.unget(got);
            break;
        }
        buf.push_back(got);
    }
    if(buf.length() != text.length()){
        r.unget(buf);
        return false;
    }
    return true;
}

// forward
std::optional<edncxx::ValueType> readValue(Utf8Reader& r);

static ValueType readString(Utf8Reader& rdr)
{
    bool in_escape = false;
    std::u32string result;
    auto q = rdr.get();
    if(q != U'"'){
        throw std::logic_error("readString input not \"");
    }
    while(true){
        auto ch = rdr.get();
        if(in_escape){
            
            auto ach = 0;
            switch(ch){
                case U't':   ch = U'\t'; break;
                case U'r':   ch = U'\r'; break;
                case U'n':   ch = U'\n'; break;
                case U'\\':  ch = U'\\'; break;
                case U'"':   ch = U'"';  break;
                default:{
                    std::ostringstream msg;
                    msg << "unsupported escape character: \\" << ch;
                    throw std::runtime_error(msg.str());
                }
            }
            in_escape = false;
        }
        else{
            if(ch == U'"') break;
            if(ch == U'\\'){
                in_escape = true;
                continue;
            }
        }
        result.push_back(ch);
    }
    return result;
}

static ValueType readChar(Utf8Reader&)
{
    boom("readChar");
    return {};
}

static ValueType readKeyword(Utf8Reader&)
{
    boom("readKeyword");
    return {};
}

static ValueType readSymbol(Utf8Reader&)
{
    boom("readSymbol");
    return {};
}

static ValueType readList(Utf8Reader&)
{
    boom("readList");
    return {};
}

static ValueType readVector(Utf8Reader&)
{
    boom("readVector");
    return {};
}

static ValueType readMap(Utf8Reader&)
{
    boom("readMap");
    return {};
}

static std::optional<ValueType> readSet(Utf8Reader&)
{
    boom("readSet");
    return {};
}

static std::optional<ValueType> readTagged(Utf8Reader&)
{
    boom("readTagged");
    return {};
}

static std::optional<ValueType> readDiscard(Utf8Reader&)
{
    boom("readDiscard");
    return {};
}

static std::optional<ValueType> readNil(Utf8Reader& r)
{
    if(tokenmatch(r, "nil")){
        return NilType();
    }
    return std::nullopt;
}

static std::optional<ValueType> readBool(Utf8Reader& r)
{
    if(tokenmatch(r, "true")) return BoolType{ true };
    if(tokenmatch(r, "false")) return BoolType{ false };
    return std::nullopt;
}

static std::optional<ValueType> readInteger(Utf8Reader&)
{
    boom("readInteger");
    return {};
}

static std::optional<ValueType> readFloat(Utf8Reader& r)
{
    // auto buf = r.getUntil(r, isterminator);
    boom("readFloat");
    return {};
}

std::optional<ValueType> readValue(Utf8Reader& r)
{
    std::optional<ValueType> result;
    while(!result){
        r.getWhile(iswhitespace);
        auto ch = r.peek();
        if(ch == char32_t(-1))
            return {};

        // comments    
        if(ch == U';'){
            eatcomment(r);
            continue;
        }
        const auto& loc = r.loc();
        
        switch(ch){
            case U'"':     result = readString(r);   break;
            case U'\'':    result = readChar(r);     break;
            case U':':     result = readKeyword(r);  break;
            case U'(':     result = readList(r);     break;
            case U'[':     result = readVector(r);   break;
            case U'{':     result = readMap(r);      break;
            case U'#': {
                if(result = readSet(r)) break;
                if(result = readTagged(r)) break;
                if(result = readDiscard(r)) continue;
            }
            default:{
                if(result = readNil(r)) break;
                if(result = readBool(r)) break;
                if(result = readInteger(r)) break;
                if(result = readFloat(r)) break;
                if(result = readSymbol(r)) break;
            }
        }
        if(!result){
            std::ostringstream msg;
            msg << "Unable to recognize EDN @ line: " << loc.first << " col: " << loc.second;
            throw std::runtime_error(msg.str());
        }
        break;
    }
    return result;
}

} // namespace 
