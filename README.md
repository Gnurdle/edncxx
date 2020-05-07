edncxx

edn for c++ - an experimental work in progress.

see https://github.com/edn-format/edn

## Warning
Just getting started on this, do not use for anything at all just yet.

## Premise
we want to be able to parse EDN without having
to inject a bunch of blabbery types into the c++
source code.

c++17 introduced std::any, which we leverage.  

std::variant<...> was tried, with ugly results surrounding the type recursion.

The goal is to have std::any be the bearer of 
an EDN value.

Since it can also be the bearer of anything, we
need to be able to narrow it to some known types

So we effectively 'box' scalars:

edncxx::nil, edncxx::bool, edncxx::string,
edncxx::char, edncxx::symbol, edncxx::keyword,
edncxx::integer, edncxx::float, edncxx::tagged

lists   become -> std::list<std::any>
vectors become -> std::vector<std::any>
maps    become -> std::unordered_map<std::any>
sets    become -> std::unordered_set<std::any>

with the presumption that the std::any(s) are
constrained to the above types, and can be 
sorted out at runtime.

