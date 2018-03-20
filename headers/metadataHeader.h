#ifndef metadataHeader_h
#define metadataHeader_h

#include <iostream>
#include "reader.h"

using namespace std;

class Header{
    public:
    bool isLast;
    uint8_t type;
    uint32_t length;
    public:
    void parseHeader(Reader &r);
};

#endif
