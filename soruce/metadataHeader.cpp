#include <iostream>
#include "reader.h"
#include "metadataHeader.h"

using namespace std;

void Header::parseHeader(Reader &r){
    isLast = r.read(1);
    type = r.read(7);
    length = r.read(24);
}
