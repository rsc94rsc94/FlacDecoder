#include <iostream>
#include <vector>
#include <fstream>
#include "reader.h"
#include "streaminfo.h"
#include "metadataData.h"
#include "subframe.h"
#include "subheader.h"
#include "frame.h"

using namespace std;

ofstream out2;
void Subframe::parseSubframe(Reader &r, Frame *f, uint32_t bps){
    int xx = r.getOctPos();
    subheader.ParseHeader(r);

    cout << "Sub Frame Header Bits number : " << r.getOctPos() -xx << endl;
    out2.open("output.txt", ios_base::out | ios_base::app);
    out2 << "   WastedBits: " << subheader.WastedBPS;
    if(subheader.Type<1){
        predMethod = SubframeConstant;
        out2 << "   Type: " << "Constant";
    }
    else if(subheader.Type<2){
        predMethod = SubframeVerbatim;
        out2 << "   Type: " << "Verbatin";
    }
    else if(subheader.Type<8){
        predMethod = Reserved;
    }
    else if(subheader.Type<16){
        uint16_t order = (uint16_t)(subheader.Type & 0x07);
        if(order >4){
            predMethod = Reserved;
        }
        else{
            predMethod = SubframeFixed;
            predOrder = order;
            out2 << "   Type: " << "Fixed";
            out2 << "   Pred_Order: " << predOrder;
        }
    }
    else if(subheader.Type<32){
        predMethod = Reserved;
    }
    else{
        predMethod = SubframLPC;
        predOrder = (uint16_t)(subheader.Type & 0x1F)+1;
        out2 << "   Type: " << "LPC";
        out2 << "   Pred_Order: " << predOrder;
    }
    out2.close();
    NOfSamples = f->fHeader.getBlockSize();
    bps -= subheader.WastedBPS;
    xx = r.getOctPos();
    switch(predMethod){
        case SubframeConstant:{
            DecodeConstant(r,bps);
                break;
        }
        case SubframeVerbatim:{
            DecodeVerbatim(r,bps);
                break;
        }
        case SubframeFixed:{
            DecodeFixed(r,bps);
            break;
        }
        case SubframLPC:{
            DecodeLPC(r,bps);
                break;
        }
    }
    cout << "Frame Bits number : " << r.getOctPos() -xx << endl;
}

void Subframe::DecodeConstant(Reader &r, uint32_t bps){
    uint32_t x = r.read(bps);
    int32_t sample = signExtension(x, bps);

	for (int i = 0; i < NOfSamples; i++) {
		Samples.push_back(sample);
	}
}
void Subframe::DecodeVerbatim(Reader &r, uint32_t bps){
    for (int i = 0; i < NOfSamples; i++) {
        uint32_t x = r.read(bps);
        int32_t sample = signExtension(x, bps);
		Samples.push_back(sample);
	}
}
void Subframe::DecodeFixed(Reader &r, uint32_t bps){
    for (int i = 0; i < predOrder; i++) {
        uint32_t x = r.read(bps);
        int32_t sample = signExtension(x, bps);
		Samples.push_back(sample);
	}
	DecodeResidual(r);

}
void Subframe::DecodeLPC(Reader &r, uint32_t bps){


    uint64_t startOctPos = r.getOctPos();
    uint8_t startBitPos = r.getBitPos();
    warmupSamples = (int32_t*)malloc(predOrder);
    for (int i = 0; i < predOrder; i++) {
        uint32_t x = r.read(bps);
        int32_t sample = signExtension(x, bps);
        warmupSamples[i] = sample;
		//Samples.push_back(sample);
	}

    uint64_t endOctPos = r.getOctPos();
    uint8_t endBitPos = r.getBitPos();
    uint64_t bitsToBeRead =((endOctPos-startOctPos)*8)+(endBitPos-startBitPos);
    r.reset();
    r.readBytes((uint8_t*)malloc(startOctPos), startOctPos);
    r.read(startBitPos);
    string warmupSmaplesString = r.readBitString(bitsToBeRead);
    int32_t warmpbits =bitsToBeRead;








	qlpCoeff = r.read(4) + 1; // quantized liner predictor coefficient bits
    qlpCoeffShift= r.read(5) ;//qipl shift needed in bits
	qlpCoeffShift = signExtension(qlpCoeffShift,5);





	startOctPos = r.getOctPos();
    startBitPos = r.getBitPos();
    predCoefficients = (int32_t*)malloc(predOrder);
    for (int i = 0; i < predOrder; i++) {
        int32_t x = r.read(qlpCoeff);
        x  = signExtension(x, qlpCoeff);
        predCoefficients[i] = x;
	}

    endOctPos = r.getOctPos();
    endBitPos = r.getBitPos();
    bitsToBeRead =((endOctPos-startOctPos)*8)+(endBitPos-startBitPos);
    r.reset();
    r.readBytes((uint8_t*)malloc(startOctPos), startOctPos);
    r.read(startBitPos);
    string coefficientsSting = r.readBitString(bitsToBeRead);
    int32_t coeffBits = bitsToBeRead;

    out2.open("output.txt", ios_base::out | ios_base::app);
    out2 << "   qlp_coeff_precision: " << qlpCoeff;
    out2 << "   quantization_level: " << qlpCoeffShift << endl;;
    for(int i = 0; i<predOrder;i++){
        out2<< "            warmup["<<i<<"]: "<< warmupSamples[i] << endl;
    }
    out2<< "                warmup samples length "<<warmpbits<<" data: "  <<warmupSmaplesString << endl;
    for(int i = 0; i<predOrder;i++){
        out2<< "            pred_coeff["<<i<<"]: "<< predCoefficients[i] << endl;
    }
    out2<< "                predictor Coefficients length "<<coeffBits<< " data: "  <<coefficientsSting << endl;
    out2.close();




	DecodeResidual(r);

}

void Subframe::DecodeResidual(Reader &r){
    // 2 bits: Residual coding method.
	uint16_t x = r.read(2);
	// The 2 bits are used to specify the residual coding method as follows:
	//    00: Rice coding with a 4-bit Rice parameter.
	//    01: Rice coding with a 5-bit Rice parameter.
	//    10: reserved.
	//    11: reserved.
	switch(x){
        case 0x0:{
            encodingType = RICE;
            DecodeRicePartition(r, 4);
            break;
        }
        case 0x1:{
            encodingType = RICE2;
            DecodeRicePartition(r, 5);
            break;
        }
        default:
            printf("error in decoding residual ");
	}
}

void Subframe::DecodeRicePartition(Reader &r,uint32_t paramSize){
    // 4 bits: Partition order.
	partitionOrder = r.read(4);

	out2.open("output.txt", ios_base::out | ios_base::app);
	out2 << "               Partition_order: " << partitionOrder << endl;
	out2.close();
	// Parse Rice partitions; in total 2^partitionOrder partitions.
	//
	// ref: https://www.xiph.org/flac/format.html#rice_partition
	// ref: https://www.xiph.org/flac/format.html#rice2_partition

	uint32_t nofpartitions = 1 << partitionOrder;
	riceParams = (int32_t*)malloc(nofpartitions);
	for(int i = 0; i < nofpartitions; i++){
		// (4 or 5) bits: Rice parameter.
		uint16_t riceParam = r.read(paramSize);

		riceParams[i] = riceParam;

		// Determine the number of Rice encoded samples in the partition.
		int32_t nsamples;
		if (partitionOrder == 0) {
			nsamples = NOfSamples - predOrder;
		} else if (i != 0 ){
			nsamples = NOfSamples / nofpartitions;
		} else {
			nsamples = NOfSamples/nofpartitions - predOrder;
		}

		// TODO(u): Verify that decoding of subframes with Rice parameter escape
		// codes have been implemented correctly.
		if (paramSize == 4 && riceParam == 0xF || paramSize == 5 && riceParam == 0x1F) {
			// 1111 or 11111: Escape code, meaning the partition is in unencoded
			// binary form using n bits per sample; n follows as a 5-bit number.
            uint16_t x = r.read(5);

			uint32_t n = x;
			for(int j = 0; j < nsamples; j++) {
				uint32_t sample  = r.read(n);
				Samples.push_back(sample);
			}
			// TODO(u): Remove log message when the test cases have been extended.
			return;
		}

		// Decode the Rice encoded residuals of the partition.
		out2.open("output.txt"  , ios_base::out | ios_base::app);
		uint64_t startOctPos = r.getOctPos();
		uint8_t startBitPos = r.getBitPos();
		for (int j = 0; j < nsamples; j++) {
			DecodeRiceResidual(r, riceParam);
		}
		uint64_t endOctPos = r.getOctPos();
		uint8_t endBitPos = r.getBitPos();
		uint64_t bitsToBeRead =((endOctPos-startOctPos)*8)+(endBitPos-startBitPos);
		uint64_t bytes;
		if(startBitPos == 0)
            bytes = endOctPos-startOctPos;
        else
            bytes = endOctPos-startOctPos-1;
        uint8_t sbits = startBitPos!=0 ? (8-startBitPos):0;
		uint8_t ebits = endBitPos!=0? endBitPos:0;
		uint8_t *buff = new uint8_t[bytes];


		r.reset();

		r.readBytes((uint8_t*)malloc(startOctPos), startOctPos);
		r.read(startBitPos);
		string sbitstr;
		if(sbits!=0)
            sbitstr = r.readBitString(sbits);
		r.readBytes(buff,bytes);
		string hex = asciiToHex(buff,bytes);
		string ebitstr;
		if(ebits!=0)
            ebitstr = r.readBitString(ebits);
		out2<< "                        partition "  << i << "start bits length: "<< (uint16_t)sbits << " start bits: "<< sbitstr<< " Hex length " << bytes << " end Bit length: " << (uint16_t)ebits << " Rice_Param: " << riceParam << " Residual: " << ebitstr << " " <<hex << endl;
        //out2<< "                        partition "  << i <<  " length " << bitsToBeRead <<" Rice_Param: " << riceParam << " Residual: " <<r.readBitString(bitsToBeRead) << endl;
        out2.close();
	}
	return;
}

void Subframe::DecodeRiceResidual(Reader &r,uint16_t riceParam){
    // Read unary encoded most significant bits.
	uint16_t high = r.readUnary();

	// Read binary encoded least significant bits.
	uint16_t low= r.read(riceParam);
	int32_t residual = int32_t(high<<riceParam | low);

	// ZigZag decode.
	residual = zigZagDecode(residual);
	Samples.push_back(residual);

	return;
}
