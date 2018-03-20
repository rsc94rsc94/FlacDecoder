#ifndef vorbisComments_h
#define vorbisComments_h

#include <iostream>
#include "metadataData.h"
#include <iomanip>

using namespace std;

class VorbisComments : public Data{
public:
    uint8_t * vorbisComments;
    void parseData(Reader &r, Header header);
    void printData(Header header);
    uint8_t* getVorbisComments();
};

#endif
