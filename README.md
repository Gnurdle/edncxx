edncxx

## edn for c++ - an experimental work in progress.

see https://github.com/edn-format/edn

### Warning
Just getting started on this, do not use for anything at all just yet.

###  Premise
we want to be able to parse EDN without having to inject a bunch of blabbery types into the
c++ source code.

c++17 introduced std::any, which we leverage.  

std::variant<...> was tried, with ugly results surrounding the type recursion.

The goal is to have std::any be the bearer of an EDN value.

Since it can also be the bearer of anything, we need to be able to narrow it to some known types.

edncxx::value = [a constrained] std::any which can be of:

- edncxx::nil
- edncxx::bool
- edncxx::string
- edncxx::char
- edncxx::symbol
- edncxx::keyword
- edncxx::integer
- edncxx::float
- edncxx::tagged
- edncxx::list = std::list\<edncxx::value\>
- edncxx::vector = std::vector\<edncxx::value\>
- edncxx::map = std::unordered_map\<edncxx::value,edncxx::value\>
- edncxx::set = std::unordered_set\<edncxx::value\>

or, at least, that's the idea.....
