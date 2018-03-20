#include <iostream>
#include <iomanip>
#include <stdio.h>

#include "utilities.h"

using namespace std;
char tem[10000000];
string asciiToHex(uint8_t* ascii, uint64_t size){
    string s;
    if(size*2+1 > 10000000){
        cout << "To Large Data " << endl;
    }
    //char tem[size*2+1];
    for(uint64_t i = 0;i<size;i++){
        sprintf(tem+(i*2),"%02X",ascii[i]);
    }
    s = string(tem);
    return s;
}

int32_t signExtension(int32_t x,uint16_t bps){
    // bps // number of bits representing the number in x
    //x    // sign extend this b-bit number to r
    int32_t r;      // resulting sign-extended number
    int32_t const m = 1U << (bps - 1); // mask can be pre-computed if b is fixed

    x = x & ((1U << bps) - 1);  // (Skip this if bits in x above position b are already zero.)
    r = (x ^ m) - m;
    return r;
}


int32_t zigZagDecode(uint32_t x){
    return ((x>>1)^(-1*(x&1)));
}
