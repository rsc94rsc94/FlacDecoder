#ifndef streaminfo_h
#define streaminfo_h

#include <iostream>
#include "metadataData.h"
#include <iomanip>

using namespace std;

class Streaminfo : public Data{
public:
    uint16_t minimumBlockSize;											//16
	uint16_t maximumBlockSize;											//16
	uint32_t minimumFrameSize;												//24
	uint32_t maximumFrameSize;												//24
	uint32_t sampleRate;													//20
	uint16_t numberOfChannels;											//3
	uint16_t bitsPerSample;												//5
	uint64_t totalSamplesInStream;									//36
	uint8_t *MD5Signature;	;

    void parseData(Reader &r, Header header);
    void printData(Header header);
    uint32_t getSampleRate();
    uint16_t getBitsPerSample();
    uint16_t getMinimumBlockSize();
    uint16_t getMaximumBlockSize();
    uint32_t getMinimumFrameSize();
    uint32_t getMaximumFrameSize();
    uint16_t getNumberOfChannels();
    uint16_t getTotalSamplesInStream();
    uint8_t* getMD5Signature();
};

#endif
