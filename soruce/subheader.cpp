
#include <iostream>
#include <vector>
#include "reader.h"
#include "streaminfo.h"
#include "metadataData.h"
#include "subheader.h"

using namespace std;

void Subheader::ParseHeader(Reader &r){
    ZeroBitPadding =  r.read(1);
    Type = r.read(6);
    uint16_t x = r.read(1);
    if(x!=0)
        WastedBPS = r.readUnary();
    else
        WastedBPS=0;

}
