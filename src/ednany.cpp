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

#include <iostream>
#include <edncxx/ednany.h>
#include <typeindex>
#include <unordered_map>

using namespace edncxx;

static std::unordered_map<std::size_t, EdnType> typeindex2typeenum = {
    {typeid(NilType).hash_code(), EdnType::T_Nil},
    {typeid(BoolType).hash_code(), EdnType::T_Bool},
    {typeid(CharType).hash_code(), EdnType::T_Char},
    {typeid(StringType).hash_code(), EdnType::T_String},
    {typeid(KeywordType).hash_code(), EdnType::T_Keyword},
    {typeid(SymbolType).hash_code(), EdnType::T_Symbol},
    {typeid(IntegerType).hash_code(), EdnType::T_Integer},
    {typeid(FloatType).hash_code(), EdnType::T_Float},
    {typeid(ListType).hash_code(), EdnType::T_List},
    {typeid(VectorType).hash_code(), EdnType::T_Vector},
    {typeid(MapType).hash_code(), EdnType::T_Map},
    {typeid(SetType).hash_code(), EdnType::T_Set},
    {typeid(TaggedType).hash_code(), EdnType::T_Tagged},
    {typeid(DiscardType).hash_code(), EdnType::T_Discard}
};

static std::vector<std::string> typeNames{
    "?Invalid?", "Nil", "Bool", "Char", "String", "Keyword", "Symbol", "Integer",
    "Float", "List", "Vector", "Map", "Set", "Tagged", "Discard"
};

using namespace edncxx;
namespace edncxx{

EdnType edntype(const ValueType& v)
{
    auto it = typeindex2typeenum.find(v.type().hash_code());
    return (it == typeindex2typeenum.end()) ? EdnType::T_Invalid : it->second;
}

std::string typenameof(const ValueType& v)
{
    auto idx = static_cast<size_t>(edntype(v));
    if(idx >= typeNames.size()) idx = 0;
    return typeNames[idx];
}

bool isNil(const ValueType& v){
    return std::any_cast<NilType>(&v);
}

bool isBool(const ValueType& v ){
    return std::any_cast<BoolType>(&v);
}

bool isChar(const ValueType& v){
    return std::any_cast<CharType>(&v);
}


} // ns