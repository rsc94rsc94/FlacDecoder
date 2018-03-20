#include <iostream>
#include <fstream>
#include <iomanip>
#include "frame.h"
#include "UTF8.h"

#define LeftSide    0x08
#define SideRight   0x09
#define MidSide     0x0A

using namespace std;

uint32_t FrameHeader::count = 0;
void FrameHeader::parseFrameHeader(Reader &r, Data* si){
    count++;
    frameNum = count;
    syncCode = 0x3ffe;
    reserved = 0;//r.read(1);
    blockingStrategy = r.read(1);
    blockSizeBits = r.read(4);
    sampleRateBits = r.read(4);
    channelAssignment = r.read(4);
    sampleSizeBits = r.read(3);
    reserved2 =  r.read(1);
    frameOrSampleNumber = decodeUTF8(r);

    /* Parse Block size : */
    //
	// The 4 bits of n are used to specify t  he block size as follows:
	//    0000: reserved.
	//    0001: 192 samples.
	//    0010-0101: 576 * 2^(n-2) samples.
	//    0110: get 8 bit (block size)-1 from the end of the header.
	//    0111: get 16 bit (block size)-1 from the end of the header.
	//    1000-1111: 256 * 2^(n-8) samples.


	if(blockSizeBits == 0x0){
        cout<< "\ninvalid block size\n" << endl;
        blockSize = -1;
	}
	else if(blockSizeBits == 0x1){
        // 0001: 192 samples.
		blockSize = 192;
    }
    else if(blockSizeBits >= 0x2 && blockSizeBits <= 0x5){
		// 0010-0101: 576 * 2^(n-2) samples.
		blockSize = 576 * (1 << (blockSizeBits - 2));
    }
	else if(blockSizeBits == 0x6){
		// 0110: get 8 bit (block size)-1 from the end of the header.
		uint16_t x = r.read(8);
		blockSize = (x + 1);
    }
	else if(blockSizeBits == 0x7){
		// 0111: get 16 bit (block size)-1 from the end of the header.
		uint16_t x = r.read(16);
		blockSize = (x + 1);
	}
	else{
		//    1000-1111: 256 * 2^(n-8) samples.
		blockSize = 256 * (1 << (blockSizeBits - 8));
	}

    /* Parse sample rate.*/
	//
	// The 4 bits are used to specify the sample rate as follows:
	//    0000: unknown sample rate; get from StreamInfo.
	//    0001: 88.2 kHz.
	//    0010: 176.4 kHz.
	//    0011: 192 kHz.
	//    0100: 8 kHz.
	//    0101: 16 kHz.
	//    0110: 22.05 kHz.
	//    0111: 24 kHz.
	//    1000: 32 kHz.
	//    1001: 44.1 kHz.
	//    1010: 48 kHz.
	//    1011: 96 kHz.
	//    1100: get 8 bit sample rate (in kHz) from the end of the header.
	//    1101: get 16 bit sample rate (in Hz) from the end of the header.
	//    1110: get 16 bit sample rate (in daHz) from the end of the header.
	//    1111: invalid.
	switch(sampleRateBits) {
	case 0x0:{
		// 0000: unknown sample rate; get from StreamInfo.
		sampleRate = si->getSampleRate();
		break;
	}
	case 0x1:{
		// 0001: 88.2 kHz.
		sampleRate = 88200;
		break;
	}
	case 0x2:{
		// 0010: 176.4 kHz.
		sampleRate = 176400;
		break;
	}
    case 0x3:{
		// 0011: 192 kHz.
		sampleRate = 192000;
        break;
	}
	case 0x4:{
		// 0100: 8 kHz.
		sampleRate = 8000;
        break;
	}
	case 0x5:{
		// 0101: 16 kHz.
		sampleRate = 16000;
        break;
	}
	case 0x6:{
		// 0110: 22.05 kHz.
		sampleRate = 22050;
        break;
	}
	case 0x7:{
		// 0111: 24 kHz.
		sampleRate = 24000;
		break;
	}
	case 0x8:{
		// 1000: 32 kHz.
		sampleRate = 32000;
        break;
	}
	case 0x9:{
		// 1001: 44.1 kHz.
		sampleRate = 44100;
        break;
	}
	case 0xA:{
		// 1010: 48 kHz.
		sampleRate = 48000;
        break;
	}
	case 0xB:{
		// 1011: 96 kHz.
		sampleRate = 96000;
        break;
	}
	case 0xC:{
		// 1100: get 8 bit sample rate (in kHz) from the end of the header.
		uint32_t x = r.read(8);
		sampleRate = (x * 1000);
		break;
	}
    case 0xD:{
		// 1101: get 16 bit sample rate (in Hz) from the end of the header.
		sampleRate = r.read(16);
		break;
	}
	case 0xE:{
		// 1110: get 16 bit sample rate (in Hz) from the end of the header.
		uint32_t x = r.read(16);
		sampleRate = (x * 10);
		break;
	}
    default:
		// 1111: invalid.
		cout<< "\ninvalid sample rate\n" << endl;
		sampleRate = -1;
	}



	/* Parse Sample Size */
    //	  000 : get from STREAMINFO metadata block
    //    001 : 8 bits per sample
    //    010 : 12 bits per sample
    //    011 : reserved
    //    100 : 16 bits per sample
    //    101 : 20 bits per sample
    //    110 : 24 bits per sample
    //    111 : reserved

    switch(sampleSizeBits){
        case 0x0:{
            sampleSize = si->getBitsPerSample();
            break;
        }
        case 0x1:{
            sampleSize = 8;
            break;
        }
        case 0x2:{
            sampleSize = 12;
            break;
        }
        case 0x3:{
            cout << "Error : reserved sample size bits; " << endl;
            sampleSize = 0;
            break;
        }
        case 0x4:{
            sampleSize = 16;
            break;
        }
        case 0x5:{
            sampleSize = 20;
            break;
        }
        case 0x6:{
            sampleSize = 24;
            break;
        }
        case 0x7:{
            cout << "Error : reserved sample size bits; " << endl;
            sampleSize = 0;
            break;
        }
    }

    CRC8 =  r.readByte();
    Channels = si->getNumberOfChannels();
}

void FrameHeader::printFrameHeader(){
    cout<< "------------------------------------Frame Header: " << frameNum << "------------------------------------------- "<< endl;
    cout<< "        Field           " << "Decimal           "     << "     Hex                 " << endl;
    cout<< "syncCode:               " << left << setw(20) << syncCode		        << hex << "   0x" << left << setw(20) << syncCode               <<  dec << endl;
	cout<< "reserved:               " << left << setw(20) << reserved	            << hex << "   0x" << left << setw(20) << reserved               <<  dec << endl;
	cout<< "blockingStrategy:       " << left << setw(20) << blockingStrategy		<< hex << "   0x" << left << setw(20) << blockingStrategy       <<  dec << endl;
	cout<< "blockSize:              " << left << setw(20) << blockSize		        << hex << "   0x" << left << setw(20) << blockSize              <<  dec << endl;
	cout<< "sampleRate:             " << left << setw(20) << sampleRate		    	<< hex << "   0x" << left << setw(20) << sampleRate             <<  dec << endl;
	cout<< "channelAssignment:      " << left << setw(20) << channelAssignment		<< hex << "   0x" << left << setw(20) << channelAssignment      <<  dec << endl;
	cout<< "sampleSize:             " << left << setw(20) << sampleSize		        << hex << "   0x" << left << setw(20) << sampleSize             <<  dec << endl;
	cout<< "reserved2:              " << left << setw(20) << reserved2  	        << hex << "   0x" << left << setw(20) << reserved2              <<  dec << endl;
	cout<< "frameOrSampleNumber:    " << left << setw(20) << frameOrSampleNumber  	<< hex << "   0x" << left << setw(20) << frameOrSampleNumber    <<  dec << endl;
	cout<< "CRC8:                   " << left << setw(20) << CRC8  	                << hex << "   0x" << left << setw(20) << CRC8                   <<  dec << endl;
    cout<< endl << endl;
}

uint32_t FrameHeader::getSyncCode(){
    return syncCode;
}
uint16_t FrameHeader::getReserved(){
    return reserved;
}
uint16_t FrameHeader::getBlockingStrategy(){
    return blockingStrategy;
}
uint8_t FrameHeader::getBlockSizeBits(){
    return blockSizeBits;
}
uint16_t FrameHeader::getBlockSize(){
    return blockSize;
}
uint8_t FrameHeader::getSampleRateBits(){
    return sampleRateBits;
}
uint32_t FrameHeader::getSampleRate(){
    return sampleRate;
}
uint16_t FrameHeader::getChannelAssignment(){
    return channelAssignment;
}
uint8_t FrameHeader::getSampleSizeBits(){
    return sampleSizeBits;
}
uint16_t FrameHeader::getSampleSize(){
    return sampleSize;
}
uint16_t FrameHeader::getReserved2(){
    return reserved2;
}
uint64_t FrameHeader::getFrameOrSampleNumber(){
    return frameOrSampleNumber;
}
uint16_t FrameHeader::getCRC8(){
    return CRC8;
}


uint64_t Frame::getLengthOfData(){
    return LengthOfData;
}
string Frame::getData(){
    return fData;
}

void Frame::parseFrame(Reader &r,Data *si){
    int xx = r.getOctPos();
    fHeader.parseFrameHeader(r,si);
    fHeader.printFrameHeader();

    cout << "Frame Header Bits number : " << r.getOctPos()-xx << endl;
     ofstream out2;
        out2.open("output.txt", ios_base::out | ios_base::app);
        out2 << "   BlockSize: " << fHeader.getBlockSize();
        out2 << "   SampleRate: " << fHeader.getSampleRate();
        out2 << "   Channels: " << fHeader.Channels;
        out2 << "   ChannelAssignment: ";
        if(fHeader.channelAssignment == SideRight)
            out2 << "RightSide" << endl;
        else if(fHeader.channelAssignment==MidSide)
            out2 << "MidSide" << endl;
        else if(fHeader.channelAssignment==LeftSide)
            out2 << "LeftSide" << endl;
        out2.close();
    for(int channel = 0;channel<fHeader.Channels;channel++){
        ofstream out2;
        out2.open("output.txt", ios_base::out | ios_base::app);
        out2 << "       Subframe: " << channel;
        out2.close();
        uint32_t bps = fHeader.getSampleSize();
        switch(fHeader.channelAssignment) {
            case SideRight:{
                // channel 0 is the side channel.
                if(channel == 0){
                    bps++;
                }
                break;
            }
            case LeftSide:
            case MidSide:{
                // channel 1 is the side channel.
                if(channel == 1){
                    bps++;
                }
            }
        }

        Subframe sub;
        sub.parseSubframe(r,this,bps);
        subframes.push_back(sub);
    }
}
