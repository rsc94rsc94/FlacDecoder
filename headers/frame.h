#ifndef frame_h
#define frame_h

#include <iostream>
#include <vector>
#include <fstream>
#include "reader.h"

#include "streaminfo.h"
#include "metadataData.h"
#include "subframe.h"

using namespace std;
class FrameHeader{
    public:
    static uint32_t count;
    uint32_t frameNum;
    uint16_t syncCode;
	uint16_t reserved;
	uint16_t blockingStrategy;
	uint8_t blockSizeBits;
	uint16_t blockSize;
	uint8_t sampleRateBits;
	uint32_t sampleRate;
	uint16_t channelAssignment;
	uint8_t sampleSizeBits;
	uint16_t sampleSize;					//in bits
	uint16_t reserved2;
	uint64_t frameOrSampleNumber;
	uint16_t CRC8;
	uint16_t Channels;

	void parseFrameHeader(Reader &r, Data* si);
	void printFrameHeader();

	uint32_t getSyncCode();
	uint16_t getReserved();
	uint16_t getBlockingStrategy();
	uint8_t getBlockSizeBits();
	uint16_t getBlockSize();
	uint8_t getSampleRateBits();
	uint32_t getSampleRate();
	uint16_t getChannelAssignment();
	uint8_t getSampleSizeBits();
	uint16_t getSampleSize();
	uint16_t getReserved2();
	uint64_t getFrameOrSampleNumber();
	uint16_t getCRC8();



};

class Frame{
    public:
    FrameHeader fHeader;
    //TODO
    //this implementation  needs changed later when decoding subframes
    uint64_t LengthOfData;
    string fData;

    //vector<Subframe> subframes;
    vector<Subframe> subframes;

    uint64_t getLengthOfData();
    string  getData();
    void parseFrame(Reader &r, Data* si);
};

#endif // frame_h
