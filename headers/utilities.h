#ifndef Utilities_h
#define Utilities_h


#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>

using namespace std;


string asciiToHex(uint8_t* ascii, uint64_t size);
int32_t signExtension(int32_t x,uint16_t bps);
int32_t zigZagDecode(uint32_t x);
#endif
