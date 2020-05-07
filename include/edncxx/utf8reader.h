#pragma once
#include <istream>

// Utf8Reader wraps an std::istream& and yields buffers of char32_t 

namespace edncxx{
    class Utf8Reader{
    public:
        explicit Utf8Reader(std::istream& source); 
        virtual ~Utf8Reader();
        std::streamsize read(char32_t* buf, std::streamsize);
    public:
        std::istream& _source;
    };
}
