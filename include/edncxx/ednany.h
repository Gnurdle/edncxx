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
#include <any>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace edncxx{
    using ValueType = std::any;

    struct NilType{};
    using BoolType = bool;
    using CharType = char32_t;
    using StringType = std::u32string;
    struct KeywordType { std::u32string ns; std::u32string keyword; };
    struct SymbolType  { std::u32string ns; std::u32string symbol; };
    using IntegerType = int64_t;
    using FloatType = double;
    using ListType = std::list<ValueType>;
    using VectorType = std::vector<ValueType>;
    using MapType = std::unordered_map<ValueType, ValueType>;
    using SetType = std::unordered_set<ValueType>;
    struct TaggedType  { std::u32string ns; std::u32string tag; ValueType rep; };
    struct DiscardType { ValueType discarded; };

    enum EdnType{ T_Invalid=0, T_Nil, T_Bool, T_Char, T_String, T_Keyword, T_Symbol, T_Integer,
           T_Float, T_List, T_Vector, T_Map, T_Set, T_Tagged, T_Discard };

    EdnType edntype(const ValueType&);
    std::string typenameof(const ValueType&);

    bool isValue(const ValueType&);   // contains some type of constrained value
    bool isNil(const ValueType&);     // contains NilType
    bool isBool(const ValueType&);    // contans bool
    bool isChar(const ValueType&);    // contains char32_t
    bool isString(const ValueType&);  // contains ustring32
    bool isKeyword(const ValueType&); // contains KeywordType
    bool isSymbol(const ValueType&);  // contains SymbolType
    bool isInteger(const ValueType&); // contains int64_t
    bool isFloat(const ValueType&);   // contains double
    bool isList(const ValueType&);    // contains std::list<ValueType>
    bool isVector(const ValueType&);  // contains std::vector<ValueType>
    bool isMap(const ValueType&);     // contains std::unordered_map<ValueType, ValueType>
    bool isSet(const ValueType&);     // contains std::unordered_set<ValueType>
    bool isTagged(const ValueType&);  // contains TaggedType
    bool isDiscard(const ValueType&); // contains DiscardType

    // these force the issue, and throw exceptions if mismatch
    NilType requireNil(const ValueType&);
    BoolType requireBool(const ValueType&);
    CharType requireChar(const ValueType&);
    const StringType& requireString(const ValueType&);
    const KeywordType& requireKeyword(const ValueType&);
    const SymbolType& requireSymbol(const ValueType&);
    IntegerType requireInteger(const ValueType&);
    FloatType requireFloat(const ValueType&);
    const ListType& requireList(const ValueType&);
    const VectorType& requireVector(const ValueType&);
    const MapType& requireMap(const ValueType&);
    const SetType& requireSet(const ValueType&);
    const TaggedType& requireTagged(const ValueType&);
    const DiscardType& requireDiscard(const ValueType&);

    // these are softer, you don't get the value if not the right type
    std::optional<NilType> maybeNil(const ValueType&);
    std::optional<BoolType> maybeBool(const ValueType&);
    std::optional<CharType> maybeChar(const ValueType&);
    std::optional<const StringType&> maybeString(const ValueType&);
    std::optional<const KeywordType&> maybeKeyword(const ValueType&);
    std::optional<const SymbolType&> maybeSymbol(const ValueType&);
    std::optional<IntegerType> maybeInteger(const ValueType&);
    std::optional<FloatType> maybeFloat(const ValueType&);
    std::optional<const ListType&> maybeList(const ValueType&);
    std::optional<const VectorType&> maybeVector(const ValueType&);
    std::optional<const MapType&> maybeMap(const ValueType&);
    std::optional<const SetType&> maybeSet(const ValueType&);
    std::optional<const TaggedType&> maybeTagged(const ValueType&);
    std::optional<const DiscardType&> maybeDiscard(const ValueType&);

}
