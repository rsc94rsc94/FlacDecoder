#include <iostream>
#include <string>
#include <string.h>
#include "metadataData.h"
#include "seektable.h"

using namespace std;
uint32_t Seekpoint::count = 0;


void Seekpoint::parseData(Reader &r, uint32_t length){
    count++;
    seekpointNumber = count;
    sampleNumberOfFisrtSampleInTargetFrame = r.read(64);
	Offset = r.read(64);
	numberOfSamplesInTargetFrame = r.read(16);
}
void Seekpoint::printData(uint32_t length){
    cout<< "------------------------------------Seek Point Metadata block : " << seekpointNumber << " ------------------------------------------- "<< endl;
    cout<< "        Field                           " << "Decimal           "     << "     Hex                 " << endl;
    cout<< "sampleNumberOfFisrtSampleInTargetFrame: " << left << setw(20) << sampleNumberOfFisrtSampleInTargetFrame	<< hex << "   0x" << left << setw(20) << sampleNumberOfFisrtSampleInTargetFrame <<  dec << endl;
	cout<< "Offset:                                 " << left << setw(20) << Offset	                                << hex << "   0x" << left << setw(20) << Offset                                 <<  dec << endl;
	cout<< "numberOfSamplesInTargetFrame:           " << left << setw(20) << numberOfSamplesInTargetFrame	        << hex << "   0x" << left << setw(20) << numberOfSamplesInTargetFrame           <<  dec << endl;
    cout<< endl << endl;
}


