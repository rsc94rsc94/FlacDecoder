#ifndef picture_h
#define picture_h

#include <iostream>
#include "metadataData.h"
#include <iomanip>

using namespace std;

class Picture : public Data{
public:
    static uint32_t count;
    uint32_t pictureNumber;

    uint32_t pictureType;
	uint32_t lengthOfMIMEType;
	uint8_t *MIMEType;// size: lengthOfMIMEType
	uint32_t lengthOfDescription;
	uint8_t *Description;//[lengthOfDescription]
	uint32_t widthOfPicturePixels;
	uint32_t heightOfPicturePixels;
	uint32_t bitsPerPixel; // color depth of picture
	uint32_t numberOfColorsUsed; // for indexed picture(GIF) , 0 for others
	uint32_t lengthOfPictureData;
	uint8_t *pictureData;//[lengthOfPictureData];

    void parseData(Reader &r, Header headerh);
    void printData(Header headerh);
    uint32_t getPictureType();
    uint32_t getLengthOfMIMEType();
    uint8_t* getMIMEType();
    uint32_t getLengthOfDescription();
    uint8_t* getDescription();
    uint32_t getWidthOfThePictureInPixels();
    uint32_t getHeightOfThePictureInPixels();
    uint32_t getBitsPerPixel();
    uint32_t getNumberOfColorsUsed();
    uint32_t getLengthOfPictureData();
    uint8_t* getPictureData();
};


#endif
