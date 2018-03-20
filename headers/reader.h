#ifndef reader_h
#define reader_h

#include <iostream>
#include <string>
#include <stdlib.h>
#define UINT64_MAX 0xffffffffffffffff

using namespace std;



class Reader{
    private:
        uint8_t* mBuffer;
        uint64_t octPos;
        uint8_t bitPos;
        bool eofbit;
        uint64_t streamLength;
        inline uint8_t XTRACT1();
        inline uint32_t XTRACTVAL(uint8_t numBits);
        inline uint64_t XTRACTVAL64(uint8_t numBits);
        inline void XTRACTARR(uint8_t numBits, uint8_t * destPtr);
        inline void copyBits (uint8_t * srcBuff, uint32_t srcBitPos, uint8_t * destBuff, uint32_t destBitPos, uint32_t numBits);

    public:
        Reader(string path);
        bool eof();
        uint8_t readByte();
        uint64_t read(uint8_t numOfBits); // read upto 64 bits in an 64 bit integer;
        void readBytes(uint8_t* dest,uint64_t numOfBytes);
        void printmBuffer(uint16_t numOfBytes);
        uint64_t readUnary();
        void reset();
        uint64_t getOctPos();
        uint16_t getByteAlignedBits();
        uint8_t getBitPos();
        string readBitString(uint64_t length);

};

#endif
