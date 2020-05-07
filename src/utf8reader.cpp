#include <edncxx/utf8reader.h>

using namespace edncxx;

Utf8Reader::Utf8Reader(std::istream& source)
    : _source(source)
{}

Utf8Reader::~Utf8Reader()
{}

std::streamsize Utf8Reader::read(char32_t* buf, std::streamsize limit)
{
    return 0;
}
