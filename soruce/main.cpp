#include <iostream>
#include <string>
#include <fstream>
#include "reader.h"
#include "flacStream.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


int main(int argc, char*argv[]){

//
//     char data[3240];
//    char* data2 = (char*)malloc(32767*sizeof(char));
//    memcpy(data2,data,32767);

    cout<< argc << endl;



    //Flac flac;
    string filename = "TestFiles\\0000014804.flac";
    Flac* flac = new Flac(filename);
    flac->makeXML((char*)filename.c_str());

//      Reader r("valid2.flac");
//    cout<< r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) << endl;
//    cout<<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1)<<r.read(1)<<r.read(1) << endl;
//    cout<<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1)<<r.read(1)<<r.read(1) << endl;
//    cout<<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1) <<r.read(1)<<r.read(1)<<r.read(1) << endl;
//      uint8_t * flacMarker = r.readBytes(4);
//
//      cout<< flacMarker[0] <<flacMarker[1]<<flacMarker[2]<<flacMarker[3]  << endl;
//      cout<< r.read(32) << endl;
//      cout << r.read(1);
    return 0;
}
