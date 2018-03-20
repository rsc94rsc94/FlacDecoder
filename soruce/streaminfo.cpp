#include <iostream>
#include "metadataData.h"
#include "streaminfo.h"
#include <iomanip>

using namespace std;



void Streaminfo::parseData(Reader &r, Header header){
    minimumBlockSize = r.read(16);
    maximumBlockSize = r.read(16);
    minimumFrameSize = r.read(24);
    maximumFrameSize = r.read(24);
    sampleRate = r.read(20);
    numberOfChannels =  r.read(3)+1;
    bitsPerSample = r.read(5)+1;
    totalSamplesInStream =  r.read(36);
    MD5Signature =(uint8_t*) malloc(sizeof(uint8_t)*16);
    r.readBytes(MD5Signature,16);

}

void Streaminfo::printData(Header header){
	cout<< "------------------------------------Streaminfo Metadata block------------------------------------------- "<< endl;
    cout<< "        Field           " << "Decimal           "     << "     Hex                 " << endl;
    cout<< "minimumBlockSize:       " << left << setw(20) << minimumBlockSize		<< hex << "   0x" << left << setw(20) << minimumBlockSize       <<  dec << endl;
	cout<< "maximumBlockSize:       " << left << setw(20) << maximumBlockSize	    << hex << "   0x" << left << setw(20) << maximumBlockSize       <<  dec << endl;
	cout<< "minimumFrameSize:       " << left << setw(20) << minimumFrameSize		<< hex << "   0x" << left << setw(20) << minimumFrameSize       <<  dec << endl;
	cout<< "maximumFrameSize:       " << left << setw(20) << maximumFrameSize		<< hex << "   0x" << left << setw(20) << maximumFrameSize       <<  dec << endl;
	cout<< "sampleRate:             " << left << setw(20) << sampleRate		    	<< hex << "   0x" << left << setw(20) << sampleRate             <<  dec << endl;
	cout<< "numberOfChannels:       " << left << setw(20) << numberOfChannels		<< hex << "   0x" << left << setw(20) << numberOfChannels       <<  dec << endl;
	cout<< "bitsPerSample:          " << left << setw(20) << bitsPerSample		    << hex << "   0x" << left << setw(20) << bitsPerSample          <<  dec << endl;
	cout<< "totalSamplesInStream:   " << left << setw(20) << totalSamplesInStream  	<< hex << "   0x" << left << setw(20) << totalSamplesInStream   <<  dec << endl;
    cout<< endl << endl;
}

uint32_t Streaminfo::getSampleRate(){
    return sampleRate;
}
uint16_t Streaminfo::getBitsPerSample(){
    return bitsPerSample;
}
uint16_t Streaminfo::getMinimumBlockSize(){
    return minimumBlockSize ;
}
uint16_t Streaminfo::getMaximumBlockSize(){
    return maximumBlockSize;
}
uint32_t Streaminfo::getMinimumFrameSize(){
    return minimumFrameSize;
}
uint32_t Streaminfo::getMaximumFrameSize(){
    return maximumFrameSize;
}
uint16_t Streaminfo::getNumberOfChannels(){
    return numberOfChannels;
}
uint16_t Streaminfo::getTotalSamplesInStream(){
    return totalSamplesInStream;
}
uint8_t* Streaminfo::getMD5Signature(){
    return MD5Signature;
}

