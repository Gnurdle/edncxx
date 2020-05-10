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

#include <sstream>
#include <stdexcept>
#include <optional>

#include <edncxx/ednany.h>

using namespace edncxx;
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
           iswhitespace(ch);
    }
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

// forward
static std::optional<edncxx::Value> readValue(Utf8Reader& r);

static Value readString(Utf8Reader&)
{
    boom("readString");
    return {};
}

static Value readChar(Utf8Reader&)
{
    boom("readChar");
    return {};
}

static Value readKeyword(Utf8Reader&)
{
    boom("readKeyword");
    return {};
}

static Value readSymbol(Utf8Reader&)
{
    boom("readSymbol");
    return {};
}

static Value readList(Utf8Reader&)
{
    boom("readList");
    return {};
}

static Value readVector(Utf8Reader&)
{
    boom("readVector");
    return {};
}

static Value readMap(Utf8Reader&)
{
    boom("readMap");
    return {};
}

static std::optional<Value> readSet(Utf8Reader&)
{
    boom("readSet");
    return {};
}

static std::optional<Value> readTagged(Utf8Reader&)
{
    boom("readTagged");
    return {};
}

static std::optional<Value> readDiscard(Utf8Reader&)
{
    boom("readDiscard");
    return {};
}

static std::optional<Value> readNil(Utf8Reader&)
{
    boom("readNil");
    return {};
}

static std::optional<Value> readBool(Utf8Reader&)
{
    boom("readBool");
    return {};
}

static std::optional<Value> readInteger(Utf8Reader&)
{
    boom("readInteger");
    return {};
}

static std::optional<Value> readFloat(Utf8Reader&)
{
    boom("readFloat");
    return {};
}

std::optional<Value> readValue(Utf8Reader& r)
{
    std::optional<Value> result;
    while(!result){
        r.getWhile(iswhitespace);
        auto ch = r.get();
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
