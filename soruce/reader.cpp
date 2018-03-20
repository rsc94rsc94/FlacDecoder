#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "reader.h"
#include <math.h>

#define   ZERO    0
#define   ONE     1
#define   TWO     2
#define   THREE   3
#define   FOUR    4
#define   FIVE    5
#define   SIX     6
#define   SEVEN   7
#define   EIGHT   8
#define   NINE    9

using namespace std;

Reader::Reader(const string path){

        ifstream infile;
        cout<< path ;
        infile.open(path.c_str(),ios::binary | ios::in);
        uint64_t fsize;
        infile.seekg(0,infile.end);
        fsize = infile.tellg();
        infile.seekg(0,infile.beg);
        mBuffer = new uint8_t[fsize];
        infile.read((char*)mBuffer,fsize);
        infile.close();
        streamLength = fsize;
        if(streamLength!=0){
            eofbit = false;
        }
        else{
            eofbit = true;
        }
//        ofstream out;
//        int cou = 0;
//        out.open("buffer.flac", ios::binary | ios::out);
//        out.write((char*)mBuffer,fsize);
//        out.close();
//        cout << fsize << endl;
        bitPos = 0;
        octPos = 0;

}

uint64_t Reader::getOctPos(){
    return octPos;
}

uint8_t Reader::getBitPos(){
    return bitPos;
}

string Reader::readBitString(uint64_t length){
    string s="";
    for(int i = 0; i<length; i++){
        uint64_t x = XTRACT1();

        if(x==1)
            s.append("1");
        else if(x==0)
            s.append("0");
        else{
            //cout<< "bitPos: " << (uint16_t)bitPos << "    OctPost: " << octPos << endl;
            return s;
        }
    }
    return s;
}
bool Reader::eof(){
    return eofbit;
}
void Reader::printmBuffer(uint16_t numOfBytes){
        for(uint64_t i = 0; i<numOfBytes&&i<streamLength; i++){
            cout<< mBuffer[i];
        }
        cout<< endl;
}

uint8_t Reader::readByte(){
    if(eofbit ==  true){
        cout<< " end of  stream reached : \n";
        return UINT64_MAX;
    }
    if(octPos<streamLength){
        uint8_t x = mBuffer[octPos++];
        if(octPos==streamLength)
            eofbit = true;
        return x;

    }
    else{
        eofbit = true;
        cout<<"Reached End of Stream: entery any key to exit" << endl << endl;
        return EOF;
    }
}


uint64_t Reader::read(uint8_t numOfBits){
    if(eofbit ==  true){
        cout << " end of stream reached : \n";
        //cout<< "IN Read bitPos: " << (uint16_t)bitPos << "    OctPost: " << octPos << endl;
        return UINT64_MAX;
    }
    if(ceil(numOfBits> ((streamLength-octPos)*8-bitPos))){
      //  cout << "\n Number Of bits To be Read : " << (uint16_t)numOfBits << "   octPos: " << octPos << "    bitPos: "<< (uint16_t)bitPos  << endl;
        numOfBits = ((streamLength-octPos)*8)-bitPos;
        cout << " end of stream reached Reading "<< (uint16_t)numOfBits << " Bits don't read further \n"  << endl;
        eofbit = true;
    }

    if(numOfBits==1){
        return (uint64_t)XTRACT1();
    }
    if(numOfBits>64)
        return -1;
    else if(numOfBits<33){
    return XTRACTVAL(numOfBits);
    }
    else{
        return XTRACTVAL64(numOfBits);
    }

//    else{
//        eofbit = true;
//        cout<<"Reached End of Stream: entery any key to exit" << endl << endl;
//        uint8_t c;
//        cin >> c;
//        exit(1);
//    }
}

void Reader::readBytes(uint8_t* dest,uint64_t numOfBytes){
    if(numOfBytes+octPos>=streamLength){
        numOfBytes = streamLength-octPos;
        eofbit = true;
    }
    memcpy(dest,mBuffer+octPos,numOfBytes);
    octPos += numOfBytes;
}

uint16_t Reader::getByteAlignedBits(){
    if(bitPos>0)
        return 8-bitPos;
    else
        return 0;
}


void Reader::reset(){
    octPos=0;
    bitPos=0;
    eofbit = false;
}


// ReadUnary decodes and returns an unary coded integer, whose value is
// represented by the number of leading zeros before a one.
//
// Examples of unary coded binary on the left and decoded decimal on the right:
//
//    1       => 0
//    01      => 1
//    001     => 2
//    0001    => 3
//    00001   => 4
//    000001  => 5
//    0000001 => 6
uint64_t Reader::readUnary(){
    uint64_t x = 0;
    while(true){
        bool bit = read(1);
        if(bit ==  true)
            break;
        else;
            x++;
    }
    return x;
}





/* Extract 1 bit*/
inline uint8_t Reader::XTRACT1()
{
    uint8_t destVal= 0;
    destVal= ( (mBuffer[octPos]) >> ( ((uint8_t)(8-(bitPos))) - 1) ) & 0x1;
    bitPos += 1;
    octPos += bitPos >> 3;
    bitPos &= 7;
    return destVal;
}


/* To fetch upto 32 bits*/
inline uint32_t Reader::XTRACTVAL(uint8_t numBits)
{
    uint32_t destVal;
    uint8_t aL[4]={0x00, 0x00, 0x00, 0x00};
    copyBits( &mBuffer[octPos], (8-(bitPos)),
              &aL[(3-(uint8_t)((numBits-1)>>3))], ((numBits&7)?(numBits&7):8), numBits);
    destVal = (((uint32_t)aL[0]) << 24) | (((uint32_t)aL[1]) << 16)  |
              (((uint32_t)aL[2]) << 8)  | ((uint32_t)aL[3]);
    bitPos += numBits;
    octPos += bitPos >> 3;
    bitPos &= 7;
    return destVal;
}
/* To fetch upto 64 bits*/
inline uint64_t Reader::XTRACTVAL64(uint8_t numBits)
{
    uint64_t destVal;
	uint8_t aL[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	XTRACTARR(numBits, aL);
	destVal = (((uint64_t)aL[0])<< 56) | (((uint64_t)aL[1])<< 48)| (((uint64_t)aL[2])<< 40)
				| (((uint64_t)aL[3])<< 32)	| (((uint64_t)aL[4])<< 24)	| (((uint64_t)aL[5])<< 16)
				| (((uint64_t)aL[6])<< 8)| ((uint64_t)aL[7]);
    return destVal;
}


/*Extract any number of bits into a 64 bit mBuffer, with the lsb at
   the rightmost bit*/
inline void Reader::XTRACTARR(uint8_t numBits, uint8_t * destPtr)
{
    copyBits(&mBuffer[octPos], (8-(bitPos)), &destPtr[(7-((uint8_t)(numBits-1)>>3))],
               ((numBits&7)?(numBits&7):8), numBits);
    bitPos += numBits;
    octPos += bitPos >> 3;
    bitPos &= 7;

}




/********************************************************************************
 *
 *     Function:   copyBits
 *
 *     Descr:  This routine copies numBits number of bits from srcBitPos
 *        position in  current octet from 'srcBuff buffer to   destBitPos
 *        position in destBuff buffer.
 *
 *
 *********************************************************************************/
inline void Reader::copyBits (uint8_t * srcBuff,   /* Pointer to the source buffer */
      uint32_t srcBitPos,   /* Bit number in source buffer */
      uint8_t * destBuff,   /* Pointer to the destination buffer */
      uint32_t destBitPos,   /* Bit number in destination buffer */
      uint32_t numBits   /* Number of bits to copy */)
{
   uint32_t numBytes = ZERO;

   /*For Stuffing: If we have less than 8 bits to copy, i.e. numbits < 8
     For Xtraction:If we have numBits less than the number of bits required
     to reach the src to nearest octet boundary */
   if ((srcBitPos < EIGHT) && (numBits <= srcBitPos))
   {
      if (numBits <= destBitPos)
      {
         *destBuff |= ((*srcBuff >> (srcBitPos - numBits)) &
               ~(~ZERO << numBits)) << (destBitPos - numBits);
      }
      else
      {
         *destBuff |= ((*srcBuff >> (srcBitPos - destBitPos)) &
               ~(~ZERO << destBitPos));
         destBuff++;
         *destBuff |= ((*srcBuff >> (srcBitPos - numBits)) &
               ~(~ZERO << (numBits - destBitPos))) <<
            (EIGHT - numBits + destBitPos);
      }
      return;
   }

   /*For Stuffing: if numBits are not multiple of 8 then, copy the essential
     bits from the starting so that only multiples of octets left to be copied
     For Xtraction: copy the essential number of bits so Source '
     curbitpos/srcBitPos reaches the octet boundary */
   if (srcBitPos != EIGHT)
   {
      if (srcBitPos <= destBitPos)
      {
         *destBuff |= ((*srcBuff) & ~(~ZERO << srcBitPos)) <<
            (destBitPos - srcBitPos);
         destBitPos -= srcBitPos;
      }
      else
      {
         *destBuff |= ((*srcBuff >> (srcBitPos - destBitPos)) &
               ~(~ZERO << destBitPos));
         destBuff++;
         *destBuff |= ((*srcBuff) & ~(~ZERO << (srcBitPos - destBitPos))) <<
            (EIGHT - srcBitPos + destBitPos);
         destBitPos += (EIGHT - srcBitPos);
      }

      numBits -= srcBitPos;
      srcBuff++;
   }


   /*For Stuffing: copy the rest (which would be left from the previous block.
     (here numbits would be multiple of 8 only)
     For Xtraction: extract the maximum number of octets directly */
   if ((numBytes = (numBits >> THREE)) > ZERO)
   {
      /* Here we copy numBytes from srcBuff buffer which are aligned
         at octet boundaries to destBuff buffer from 'destBitPos' position. */
      numBits -= (numBytes << THREE);
      while (numBytes--)
      {
         *destBuff++ |= (*srcBuff >> (EIGHT - destBitPos));
         if (destBitPos != EIGHT)
         {
            *destBuff = *srcBuff << destBitPos;
         }
         srcBuff++;
      }
   }

   /*For Stuffing: The following block will not be executed( Condition will always be false) */
   /*For Xtraction: copy the rest bits left */
   if (numBits > ZERO)
   {
      if (numBits <= destBitPos)
      {
         *destBuff |=
            ((*srcBuff >> (EIGHT - numBits)) & ~(~ZERO << numBits)) <<
            (destBitPos - numBits);
      }
      else
      {
         *destBuff |= ((*srcBuff >> (EIGHT - destBitPos)) &
               ~(~ZERO << destBitPos));
         destBuff++;
         *destBuff |= ((*srcBuff >> (EIGHT - numBits)) &
               ~(~ZERO << (numBits - destBitPos))) <<
            (EIGHT - numBits + destBitPos);
      }
   }

   return;
}

