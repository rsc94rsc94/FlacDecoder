#ifndef seekpointtable_h
#define seekpointtable_h

#include <iostream>
#include "metadataData.h"
#include <iomanip>
#include <vector>
#include "seekpoint.h"

using namespace std;


class SeekpointTable : public Data{
public:
    vector<Seekpoint> sp;
    uint32_t numOfSeekPoints;
    void parseData(Reader &r, Header header);
    void printData(Header header);
    vector<Seekpoint> getSeekPoints();
};


#endif
