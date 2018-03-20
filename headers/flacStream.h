#ifndef flacStream_h
#define flacStream_h

#include <iostream>
#include<string>
#include "reader.h"
#include <vector>
#include "metadata.h"
#include "frame.h"
#include "streaminfo.h"
#include "vorbisComments.h"
#include "seektable.h"
#include "picture.h"
#include "utilities.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Flac{
    private:

    Streaminfo si;
    VorbisComments vc;
    SeekpointTable st;
    Picture pictures[1000];

    string flacMarker;
    vector<Meta> meta;
    vector<Frame> frames;
    uint64_t NumberOfFrames;
    string parseFlacMarker(uint8_t* ptr);
    //TODO

    //Frame frames[]
    public:
    Flac(string file);
    Flac();
    void makeXML(char* fileName);

};

#endif
