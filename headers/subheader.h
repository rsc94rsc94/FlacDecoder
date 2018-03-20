#ifndef subheader_h
#define subheader_h

#include <iostream>
#include <vector>
#include "reader.h"
#include "streaminfo.h"
#include "metadataData.h"

using namespace std;


class Subheader{
    public:
        bool ZeroBitPadding;
        uint16_t Type;
        uint64_t WastedBPS;
        void ParseHeader(Reader &r);

};



#endif
