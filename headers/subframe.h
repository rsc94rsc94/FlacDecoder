#ifndef subframe_h
#define subframe_h

#include <iostream>
#include <vector>
#include "reader.h"
#include "streaminfo.h"
#include "metadataData.h"
#include "subheader.h"
#include "utilities.h"

using namespace std;


#define     SubframeConstant    0
#define     SubframeVerbatim    1
#define     SubframeFixed       2
#define     SubframLPC          3
#define     Reserved            4

#define     RICE                4
#define     RICE2               5
class Frame;

class Subframe{
public:
    Subheader subheader;
    vector<int32_t> Samples;
    uint32_t NOfSamples;
    uint16_t predMethod;
    uint16_t predOrder;
    uint16_t qlpCoeff;
    uint32_t partitionOrder;
    int16_t qlpCoeffShift;
    int32_t *warmupSamples;
    int32_t *predCoefficients;
    int32_t *riceParams;
    uint16_t encodingType;
    void parseSubframe(Reader &r, Frame *f, uint32_t bps);
    void DecodeConstant(Reader &r, uint32_t bps);
    void DecodeVerbatim(Reader &r, uint32_t bps);
    void DecodeFixed(Reader &r, uint32_t bps);
    void DecodeLPC(Reader &r, uint32_t bps);
    void DecodeResidual(Reader &r);
    void DecodeRicePartition(Reader &r,uint32_t paramSize);
    void DecodeRiceResidual(Reader &r, uint16_t param);
};



#endif
