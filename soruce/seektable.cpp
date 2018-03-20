#include <iostream>
#include <string>
#include <string.h>
#include "metadataData.h"
#include "seektable.h"

using namespace std;

void SeekpointTable ::parseData(Reader &r, Header header){
    numOfSeekPoints = header.length/18;
    for(uint32_t i = 0;i<numOfSeekPoints; i++){
        Seekpoint s;
        s.parseData(r,header.length);
        //s.printData(header.length);
        sp.push_back(s);
    }
}

void SeekpointTable::printData(Header header){
    for(uint32_t i = 0; i<numOfSeekPoints; i++){
        sp[i].printData(header.length);
    }
}

vector<Seekpoint> SeekpointTable::getSeekPoints(){
    return sp;
}


