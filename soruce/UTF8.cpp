#include <iostream>
#include <iomanip>
#include "UTF8.h"

using namespace std;

uint64_t decodeUTF8(Reader &r){
    uint8_t c0 = r.readByte();
    if(c0<tx){
        return (uint64_t) c0;
    }
    if(c0<t2)
        return -1;

    uint64_t n;
    int l;

    if(c0<t3){
        l = 1;
        n = c0 & mask2;
    }
    else if(c0<t4){
        l = 2;
        n = c0 & mask3;
    }
    else if(c0<t5){
        l = 3;
        n = c0 & mask4;
    }
    else if(c0<t6){
        l = 4;
        n = c0 & mask5;
    }
    else if(c0<t7){
         l = 5;
         n = c0 & mask6;
    }
    else if(c0<t8){
        l = 6;
        n = 0;
    }
    else
        return -2;
    int i;
    for (i =0; i < l; i++) {
		n <<= 6;
		uint8_t c = r.readByte();
		if (c < tx || t2 <= c) {
			return -3;
		}
		n |= (c & maskx);
	}
    return n;
}
