#ifndef metadata_h
#define metadata_h

#include <iostream>
#include "metadataHeader.h"
#include "metadataData.h"
#include "reader.h"

using namespace std;

class Meta{
    public:
        Header header;
        Data * data;
        //obsolete Function
        //void parse(Reader &r);
        Meta(Header h, Data* data);
};

#endif
