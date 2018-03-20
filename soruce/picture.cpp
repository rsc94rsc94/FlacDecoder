
#include <iostream>
#include <string>
#include <string.h>
#include "metadataData.h"
#include "picture.h"

using namespace std;
uint32_t Picture::count = 0;

void Picture::parseData(Reader &r, Header header){
    count++;
    pictureNumber = count;
    pictureType = r.read(32);
    lengthOfMIMEType = r.read(32);
    MIMEType = (uint8_t*)malloc(lengthOfMIMEType);
    r.readBytes(MIMEType, lengthOfMIMEType);// size: lengthOfMIMEType
    lengthOfDescription = r.read(32);
    Description = (uint8_t*)malloc(lengthOfDescription);
    r.readBytes(Description,lengthOfDescription);//[lengthOfDescription]
    widthOfPicturePixels = r.read(32);
    heightOfPicturePixels = r.read(32);
    bitsPerPixel = r.read(32); // color depth of picture
    numberOfColorsUsed = r.read(32); // for indexed picture(GIF) , 0 for others
    lengthOfPictureData = r.read(32);
    pictureData = (uint8_t*)malloc(lengthOfPictureData);
    r.readBytes(pictureData,lengthOfPictureData);//[lengthOfPictureData];


}
void Picture::printData(Header header){
    cout<< "------------------------------------Picture Metadata block : " << pictureNumber << " ------------------------------------------- "<< endl;
    cout<< "        Field           " << "Decimal           "     << "     Hex                 " << endl;
    cout<< "pictureType:            " << left << setw(20) << pictureType		    << hex << "   0x" << left << setw(20) << pictureType            <<  dec << endl;
	cout<< "lengthOfMIMEType:       " << left << setw(20) << lengthOfMIMEType	    << hex << "   0x" << left << setw(20) << lengthOfMIMEType       <<  dec << endl;
	cout<< "lengthOfDescription:    " << left << setw(20) << lengthOfDescription	<< hex << "   0x" << left << setw(20) << lengthOfDescription    <<  dec << endl;
	cout<< "widthOfPicturePixels:   " << left << setw(20) << widthOfPicturePixels	<< hex << "   0x" << left << setw(20) << widthOfPicturePixels   <<  dec << endl;
	cout<< "heightOfPicturePixels:  " << left << setw(20) << heightOfPicturePixels	<< hex << "   0x" << left << setw(20) << heightOfPicturePixels  <<  dec << endl;
	cout<< "bitsPerPixel:           " << left << setw(20) << bitsPerPixel		    << hex << "   0x" << left << setw(20) << bitsPerPixel           <<  dec << endl;
	cout<< "numberOfColorsUsed:     " << left << setw(20) << numberOfColorsUsed		<< hex << "   0x" << left << setw(20) << numberOfColorsUsed     <<  dec << endl;
	cout<< "lengthOfPictureData:    " << left << setw(20) << lengthOfPictureData  	<< hex << "   0x" << left << setw(20) << lengthOfPictureData    <<  dec << endl;
    cout<< endl << endl;

}


uint32_t Picture::getPictureType(){
    return pictureType;
}
uint32_t Picture::getLengthOfMIMEType(){
    return lengthOfMIMEType ;
}
uint8_t* Picture::getMIMEType(){
    return MIMEType;
}
uint32_t Picture::getLengthOfDescription(){
    return lengthOfDescription;
}
uint8_t* Picture::getDescription(){
    return Description;
}
uint32_t Picture::getWidthOfThePictureInPixels(){
    return widthOfPicturePixels;
}
uint32_t Picture::getHeightOfThePictureInPixels(){
    return heightOfPicturePixels;
}
uint32_t Picture::getBitsPerPixel(){
    return bitsPerPixel;
}
uint32_t Picture::getNumberOfColorsUsed(){
    return numberOfColorsUsed;
}
uint32_t Picture::getLengthOfPictureData(){
    return lengthOfPictureData;
}
uint8_t* Picture::getPictureData(){
    return pictureData;
}


