#ifndef metadataData_h
#define metadataData_h
#include<vector>
#include <iostream>
#include "reader.h"
#include "metadataHeader.h"
#include "seekpoint.h"
using namespace std;

class Data{
    public:
    virtual void parseData(Reader &r, Header header)=0;
    virtual void printData(Header header) = 0;
    virtual uint32_t getSampleRate(){
        return 0;
    }
    virtual uint16_t getBitsPerSample(){
        return 0;
    }
    virtual uint16_t getMinimumBlockSize(){
        return 0 ;
    }
    virtual uint16_t getMaximumBlockSize(){
        return 0;
    }
    virtual uint32_t getMinimumFrameSize(){
        return 0;
    }
    virtual uint32_t getMaximumFrameSize(){
        return 0;
    }
    virtual uint16_t getNumberOfChannels(){
        return 0;
    }
    virtual uint16_t getTotalSamplesInStream(){
        return 0;
    }
    virtual uint8_t* getMD5Signature(){
        return 0;
    }


    virtual uint8_t* getVorbisComments(){
        return 0;
    }


    virtual vector<Seekpoint> getSeekPoints(){
        vector<Seekpoint> i;
        return i;
    }




    virtual uint32_t getPictureType(){
        return 0;
    }
    virtual uint32_t getLengthOfMIMEType(){
        return 0;
    }
    virtual uint8_t* getMIMEType(){
        return 0;
    }
    virtual uint32_t getLengthOfDescription(){
        return 0;
    }
    virtual uint8_t* getDescription(){
        return 0;
    }
    virtual uint32_t getWidthOfThePictureInPixels(){
        return 0;
    }
    virtual uint32_t getHeightOfThePictureInPixels(){
        return 0;
    }
    virtual uint32_t getBitsPerPixel(){
        return 0;
    }
    virtual uint32_t getNumberOfColorsUsed(){
        return 0;
    }
    virtual uint32_t getLengthOfPictureData(){
        return 0;
    }
    virtual uint8_t* getPictureData(){
        return 0;
    }
};


#endif
