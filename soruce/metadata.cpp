#include <iostream>
#include "metadata.h"
#include "streaminfo.h"
#include "vorbisComments.h"
#include "seekpoint.h"
#include "picture.h"

using namespace std;

Meta::Meta(Header h, Data* d){
    header = h;
    data = d;
}

//Obsolete function

//void Meta::parse(Reader &r){
//
//    header.parseHeader(r);
//    if(header.type == 0){
//        Streaminfo st;
//        data = &st;
//        data->parseData(r,header);
//        data->printData(header);
//        uint32_t x = data->getBitsPerSample();
//        uint32_t y = data->getSampleRate();
//    }
//    else if(header.type == 4){
//        VorbisComments vc;
//        data = &vc;
//        data->parseData(r,header);
//        data->printData(header);
//    }
//    else if(header.type == 3){
//        SeekpointTable s;
//        data = &s;
//        data->parseData(r,header);
//        data->printData(header);
//    }
//    else if( header.type == 6){
//        Picture p;
//        data = &p;
//        data->parseData(r,header);
//        data->printData(header);
//    }
//    else{
//        r.readBytes(header.length);
//    }

    //if(header)
//}
