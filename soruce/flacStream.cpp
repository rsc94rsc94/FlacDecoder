#include <iostream>
#include <fstream>
#include<string>
#include <iomanip>
#include <vector>
#include <string>
#include <string.h>

#include "reader.h"
#include "metadata.h"
#include "flacStream.h"

#include "picture.h"


using namespace std;
using namespace tinyxml2;

string Flac::parseFlacMarker(uint8_t* ptr){
    string s="";
    for(int i = 0;i<4;i++)
        s+=*ptr++;
    return s;
}

//TODO
//Frame frames[]
Flac::Flac(){
}

uint8_t framedata[10000000];


Flac::Flac(string file){
    Reader r(file);
    uint8_t * flacmarker = (uint8_t*)malloc(32) ;
    r.readBytes(flacmarker,4);
    flacMarker = parseFlacMarker(flacmarker);
    int i =0;
    int pictureCount=0;
    do{
        Header header;
        Data* data;
        header.parseHeader(r);
        if(header.type == 0){
            data = &si;
            data->parseData(r,header);
            data->printData(header);
        }
        else if(header.type == 4){
            data = &vc;
            data->parseData(r,header);
            data->printData(header);
        }
        else if(header.type == 3){
            data = &st;
            data->parseData(r,header);
            data->printData(header);
        }
        else if( header.type == 6){
            //TODO to verify if its correct

            data = pictures+pictureCount;
            pictureCount++;
            data->parseData(r,header);
            data->printData(header);
        }
        else{
            uint8_t * notUsed = (uint8_t*)malloc(header.length);
            r.readBytes(notUsed,header.length);
        }
        Meta m = Meta(header, data);
        meta.push_back(m);
        i++;
    }while(meta[i-1].header.isLast==0);
    //uint32_t xrere = meta[0].data->getSampleRate();
//Parsing frames

    NumberOfFrames = 0;
//    ofstream out2;
//    out2.open("output.txt"  , ios_base::out);
//    out2.close();
//    while(!r.eof()){
//        ofstream out2;
//        out2.open("output.txt"  , ios_base::out | ios_base::app);
//        cout << "\nFrame Offset : " <<r.getOctPos() << endl;
//        out2 << "\nFrame Offset : " <<r.getOctPos() << endl;
//        out2.close();
//        Frame f;
//        int xx = r.getOctPos();
//        if(r.read(15) == 0x7ffc){
//            f.parseFrame(r, meta[0].data);
//            //f.fHeader.parseFrameHeader(r,meta[0].data);
//        // f.fHeader.printFrameHeader();
//        }
//        cout<< (uint16_t)r.getBitPos() << "   " << r.getOctPos() << endl;
//        uint16_t byteAlignedBits = r.getByteAlignedBits();
//        uint16_t t = r.read(byteAlignedBits);
//        cout<< (uint16_t)r.getBitPos() << "   " << r.getOctPos() << endl;
//        uint16_t crc16  = r.read(16);
//        cout << "  length: " << byteAlignedBits << " t: " << t << " crc16: "<< crc16 << endl;
//        cout<< (uint16_t)r.getBitPos() << "   " << r.getOctPos() << endl;
//        cout <<" Number of bits in frame : " <<  (((r.getOctPos()-xx)*8)+r.getBitPos()) << endl;
//        out2.open("output.txt"  , ios_base::out | ios_base::app);
//        out2 <<" Number of bits in frame : " <<  (((r.getOctPos()-xx)*8)+r.getBitPos()) << endl;
//        out2.close();
//        NumberOfFrames++;
//    }

    Frame f;
    uint8_t x = 0;
    uint8_t x1 = 0;
    r.read(15);
    if(!r.eof()){
        f.fHeader.parseFrameHeader(r,meta[0].data);
        f.fHeader.printFrameHeader();
    }
    ofstream out("hexout.txt",ofstream::out);
    while(!r.eof()){
        uint64_t num = 0;
        while(!r.eof()){
            x = x1;
            x1 = r.read(7);
            if((x==255 && x1==124)){
            break;
            }
            else{
                uint8_t xx = r.read(1);
                x1 = x1<<1;
                x1 = x1|xx;

                //sprintf(f.fData+(i*2),"%02X",x1);
                //out << setfill('0') << setw(2) << hex << +x1;
            }
            framedata[num++] = x1;

            //num++;
        }
        //out << "\n\n\n\n";
        cout << num << endl;
        f.LengthOfData = num-1;
        f.fData = asciiToHex(framedata,num-1);
        frames.push_back(f);
        NumberOfFrames++;
        //cout << f.fData << endl;
        if(!r.eof()){

            f.fHeader.parseFrameHeader(r,meta[0].data);
            f.fHeader.printFrameHeader();
        }
    }
    int counts = 0;
    while(!r.eof()){
        counts++;
        uint16_t  x =  r.readByte();
        out << setfill('0') << setw(2) << hex<< x;
        ;
    }
    cout<< counts << endl;
    out<< endl;
//    TODO:
//    frame.parse();
}



void Flac::makeXML(char* fileName){
    XMLDocument doc;

    //XMLUnknown * text = doc.NewUnknown("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    //XMLText * text = doc.NewText("\<\?xml version=\"1.0\" encoding=\"utf-8\"\?>\n");
    //doc.InsertFirstChild(text);
    XMLElement * pRoot = doc.NewElement("QFAST");
    pRoot->SetAttribute("xmlns","QFastGrammarXSD.xsd");
    pRoot->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    pRoot->SetAttribute("xsi:schemaLocation", "QFastGrammarXSD.xsd file://na/qctasw/tools/Installers/QFAST/XSD/QFastGrammarXSD.xsd");
    pRoot->SetAttribute("version", "01.01");
    doc.InsertFirstChild(pRoot);

    XMLElement * config = doc.NewElement("Configuration");
    config->SetAttribute("maxFields", "2");
    pRoot->InsertFirstChild(config);

    XMLElement * datamodel = doc.NewElement("DataModel");
    pRoot->InsertEndChild(datamodel);

    XMLElement * interface = doc.NewElement("Interface");
    interface->SetAttribute("name", fileName);
    interface->SetAttribute("endian","big");
    datamodel->InsertFirstChild(interface);

    XMLComment * comment = doc.NewComment("++++++++++++++++++++++++++++++++++++++++++++++++ Starting Flac stream ++++++++++++++++++++++++++++++++++++++++");
    interface->InsertFirstChild(comment);

    XMLElement *outerBlock = doc.NewElement("Block");
    interface->InsertAfterChild(comment,outerBlock);

    comment = doc.NewComment("Flac stream Marker");
    outerBlock->InsertFirstChild(comment);

    XMLElement *field = doc.NewElement("Field");
    field->SetAttribute("inputType","string");
    field->SetAttribute("name","StreamMarker");
    field->SetAttribute("mutable","no");
    field->SetAttribute("outputType","ascii");

    XMLElement * length  = doc.NewElement("Length");
    length->SetText("4");
    field->InsertFirstChild(length);


    XMLElement * value = doc.NewElement("Value");
    value->SetText("fLaC");
    field->InsertEndChild(value);

    outerBlock->InsertEndChild(field);

    comment = doc.NewComment("++++++++++++++++++++++++++++++++ Meta data blocks ++++++++++++++++++++++++++++++");
    outerBlock->InsertEndChild(comment);

    XMLElement * metadataBlock = doc.NewElement("Block");

    //meta data blocks
    vector<Meta> :: iterator it = meta.begin();

    while(it!=meta.end()){
        Meta m = *it;
        Header h = m.header;
        Data *data = m.data;
        XMLComment* comment;
        if(h.type==0){
            comment = doc.NewComment("++++++++++++++++++++++++++++++++++++++ Stream info meta data block ++++++++++++++++++++++++++++++++++");

        }
        else if(h.type == 3){
            comment = doc.NewComment("++++++++++++++++++++++++++++++++++++ Seek Table meta data block ++++++++++++++++++++++++++++++++++++++++++");
        }
        else if(h.type == 4){
            comment = doc.NewComment("++++++++++++++++++++++++++++++++++++++++++ Vorbis Comments meta data block ++++++++++++++++++++++++++++++++++++++++");
        }
        else if(h.type == 6){
            comment = doc.NewComment("++++++++++++++++++++++++++++++++++++++++ Picture meta data block ++++++++++++++++++++++++++++++++++++++++++");
        }

        metadataBlock->InsertEndChild(comment);
        XMLElement * metaData = doc.NewElement("Block");
        metadataBlock->InsertEndChild(metaData);
        comment = doc.NewComment("++++++++++++++++++++++++++++++++++++++++ Meta data Header ++++++++++++++++++++++++++++++++++");
        metaData->InsertEndChild(comment);

        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","IsLastMetadataBlock");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        XMLElement * length  = doc.NewElement("Length");
        length->SetText("1");
        field->InsertFirstChild(length);


        XMLElement * value = doc.NewElement("Value");
        value->SetText((uint16_t)h.isLast);
        field->InsertEndChild(value);

        metaData->InsertEndChild(field);

        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","BlockType");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText("7");
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(h.type);
        field->InsertEndChild(value);

        metaData->InsertEndChild(field);

        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","LengthOfMetaDataBlockInBytes");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText("24");
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(h.length);
        field->InsertEndChild(value);

        metaData->InsertEndChild(field);

        comment = doc.NewComment("++++++++++++++++++++++++++++++++++++++ Metadata Data ++++++++++++++++++++++++++++++++");
        metaData->InsertEndChild(comment);
        if(h.type == 0){
            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","MinimunBlockSize");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            XMLElement * length  = doc.NewElement("Length");
            length->SetText("16");
            field->InsertFirstChild(length);


            XMLElement * value = doc.NewElement("Value");
            value->SetText(data->getMinimumBlockSize());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","MaximunBlockSize");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("16");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getMaximumBlockSize());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","MinimumFrameSize");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("24");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getMinimumFrameSize());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","MaximumFrameSize");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("24");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getMaximumFrameSize());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","SampleRateInHz");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("20");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getSampleRate());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","NumberOfChannelsMinusOne");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("3");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getNumberOfChannels()-1);
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","BitsPerSampleMinusOne");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("5");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getBitsPerSample()-1);
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","bit");
            field->SetAttribute("name","TotalSamplesInStream");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("36");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getTotalSamplesInStream());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","hex");
            field->SetAttribute("name","MD5Singnature");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText("16");
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(asciiToHex(data->getMD5Signature(),16).c_str());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);
        }
        else if(h.type == 3){
                vector<Seekpoint> sp = data->getSeekPoints();
                vector<Seekpoint> :: iterator it1 = sp.begin();
                int num=0;
                while(it1!=sp.end()){
                    char snum[10];
                    itoa(num,snum,10);
                    Seekpoint s = *it1;
                    char sampleNumber[100] = "sampleNumberOfFisrtSampleInTargetFrame";
                    strcat(sampleNumber,snum);//+ ss.str();
                    char offset[100]="Offset";
                    strcat(offset,snum);//+snum;
                    char numberOfSamples[100] = "numberOfSamplesInTargetFrame";
                    strcat(numberOfSamples,snum);//+snum;
                   // cout << sampleNumber << "   " << offset << "    " << numberOfSamples << endl;
                    num++;

                    char com[200] = "++++++++++++++++++++++++++++++++++ Seekpoint ";
                    strcat(com,snum);
                    strcat(com," ++++++++++++++++++++++++++++++++++");
                    XMLComment *comm = doc.NewComment(com);
                    metaData->InsertEndChild(comm);

                    field = doc.NewElement("Field");
                    field->SetAttribute("inputType","byte");
                    field->SetAttribute("name",sampleNumber);
                    field->SetAttribute("mutable","no");
                    field->SetAttribute("outputType","ascii");

                    length  = doc.NewElement("Length");
                    length->SetText(8);
                    field->InsertFirstChild(length);


                    value = doc.NewElement("Value");
                    value->SetText(s.sampleNumberOfFisrtSampleInTargetFrame);
                    field->InsertEndChild(value);

                    metaData->InsertEndChild(field);



                    field = doc.NewElement("Field");
                    field->SetAttribute("inputType","byte");
                    field->SetAttribute("name",offset);
                    field->SetAttribute("mutable","no");
                    field->SetAttribute("outputType","ascii");

                    length  = doc.NewElement("Length");
                    length->SetText(8);
                    field->InsertFirstChild(length);


                    value = doc.NewElement("Value");
                    value->SetText(s.Offset);
                    field->InsertEndChild(value);

                    metaData->InsertEndChild(field);

                    field = doc.NewElement("Field");
                    field->SetAttribute("inputType","byte");
                    field->SetAttribute("name",numberOfSamples);
                    field->SetAttribute("mutable","no");
                    field->SetAttribute("outputType","ascii");

                    length  = doc.NewElement("Length");
                    length->SetText(2);
                    field->InsertFirstChild(length);


                    value = doc.NewElement("Value");
                    value->SetText(s.numberOfSamplesInTargetFrame);
                    field->InsertEndChild(value);

                    metaData->InsertEndChild(field);
                    it1++;
                }
        }
        else if(h.type==4){
            field = doc.NewElement("Field");
            field->SetAttribute("inputType","hex");
            field->SetAttribute("name","VorbisComments");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(h.length);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(asciiToHex(data->getVorbisComments(),h.length).c_str());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);
        }
        else if(h.type == 6){
            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","PictureType");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getPictureType());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","LengthOfTheMIMEType");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getLengthOfMIMEType());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","hex");
            field->SetAttribute("name","MIMEType");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(data->getLengthOfMIMEType());
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(asciiToHex(data->getMIMEType(),data->getLengthOfMIMEType()).c_str());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","LengthOfDescription");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getLengthOfDescription());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","hex");
            field->SetAttribute("name","Description");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(data->getLengthOfDescription());
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(asciiToHex(data->getDescription(),data->getLengthOfDescription()).c_str());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","WidthOfThePictureInPixels");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getWidthOfThePictureInPixels());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","HeightOfThePictureInPixels");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getHeightOfThePictureInPixels());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","BitsPerPixel");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getBitsPerPixel());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);
            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","NumberOfColorsUsed");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getNumberOfColorsUsed());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","byte");
            field->SetAttribute("name","LengthOfPictureData");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(4);
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(data->getLengthOfPictureData());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);

            field = doc.NewElement("Field");
            field->SetAttribute("inputType","hex");
            field->SetAttribute("name","PictureData");
            field->SetAttribute("mutable","no");
            field->SetAttribute("outputType","ascii");

            length  = doc.NewElement("Length");
            length->SetText(data->getLengthOfPictureData());
            field->InsertFirstChild(length);


            value = doc.NewElement("Value");
            value->SetText(asciiToHex(data->getPictureData(),data->getLengthOfPictureData()).c_str());
            field->InsertEndChild(value);

            metaData->InsertEndChild(field);
        }

        it++;
    }


    outerBlock->InsertEndChild(metadataBlock);

    XMLElement* framesBlock = doc.NewElement("Block");

    vector<Frame> :: iterator it2 = frames.begin();
    uint64_t frameNumber = 0;
    while(it2!=frames.end()){
        XMLElement * frm = doc.NewElement("Block");

        char comm[1000] = "++++++++++++++++++++++++# Frame Number: ";
        char numoff[100];
        itoa(frameNumber,numoff,10);
        strcat(comm,numoff);
        strcat(comm," ++++++++++++++++++++++++++++++++++++++#");
        XMLComment * comment = doc.NewComment(comm);
        framesBlock->InsertEndChild(comment);

        comment = doc.NewComment("++++++++++++++++++++++++++++++++++ Frame Header ++++++++++++++++++++++++++++++++++++++#");
        frm->InsertEndChild(comment);

        Frame f = *it2;

        XMLElement *field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","SyncCode");
        field->SetAttribute("mutable","no");
        field->SetAttribute("endian","big");
        field->SetAttribute("outputType","ascii");

        XMLElement * length  = doc.NewElement("Length");
        length->SetText("14");
        field->InsertFirstChild(length);


        XMLElement * value = doc.NewElement("Value");
        value->SetText(f.fHeader.getSyncCode());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);



        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","Reserved");
        field->SetAttribute("mutable","no");
        field->SetAttribute("endian","big");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(1);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getReserved());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","BlockingStrategy");
        field->SetAttribute("endian","big");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(1);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getBlockingStrategy());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","BlockSizeBits");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(4);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getBlockSizeBits());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","SampleRateBits");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(4);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getSampleRateBits());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","ChannelAssignment");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(4);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getChannelAssignment());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","SampleSizeBits");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(3);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getSampleSizeBits());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","Reserved2");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(1);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getReserved2());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","byte");
        field->SetAttribute("name","FrameOrSampleNumer");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(8);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getFrameOrSampleNumber());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);


        field = doc.NewElement("Field");
        field->SetAttribute("inputType","bit");
        field->SetAttribute("name","CRC8");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(8);
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.fHeader.getCRC8());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);




        comment = doc.NewComment("++++++++++++++++++++++++++++++++++++++ Frame Data ++++++++++++++++++++++++++++++++");
        frm->InsertEndChild(comment);

        field = doc.NewElement("Field");
        field->SetAttribute("inputType","hex");
        field->SetAttribute("name","FrameData");
        field->SetAttribute("mutable","no");
        field->SetAttribute("outputType","ascii");

        length  = doc.NewElement("Length");
        length->SetText(f.getLengthOfData());
        field->InsertFirstChild(length);


        value = doc.NewElement("Value");
        value->SetText(f.getData().c_str());
        field->InsertEndChild(value);

        frm->InsertEndChild(field);



        framesBlock->InsertEndChild(frm);
        frameNumber++;
        it2++;
    }
    outerBlock->InsertEndChild(framesBlock);

    XMLError err = doc.SaveFile(strcat(fileName,".xml"));
}




