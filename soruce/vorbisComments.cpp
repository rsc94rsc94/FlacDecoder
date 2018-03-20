#include <iostream>
#include <string>
#include <string.h>
#include "metadataData.h"
#include "vorbisComments.h"

using namespace std;

void VorbisComments::parseData(Reader &r, Header header){
    vorbisComments = (uint8_t*) malloc(sizeof(uint8_t)*header.length);
    r.readBytes(vorbisComments,header.length);

}
void VorbisComments::printData(Header header){
    cout<< "------------------------------------Vorbis Comments Metadata block------------------------------------------- "<< endl;
    for(uint32_t i = 0;i<header.length; i++){
        cout<< vorbisComments[i];
    }
    cout<< endl << endl;
}

uint8_t* VorbisComments::getVorbisComments(){
    return vorbisComments;
}

