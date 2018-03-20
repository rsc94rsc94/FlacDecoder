#ifndef seekpoint_h
#define seekpoint_h

#include <iostream>
#include "metadataData.h"
#include <iomanip>
#include <vector>

using namespace std;

class Seekpoint {
public:
    static uint32_t count;
    uint32_t seekpointNumber;
    uint64_t sampleNumberOfFisrtSampleInTargetFrame;
	uint64_t Offset;
	uint16_t numberOfSamplesInTargetFrame;
	void parseData(Reader &r, uint32_t length);
    void printData(uint32_t length);
};



#endif
