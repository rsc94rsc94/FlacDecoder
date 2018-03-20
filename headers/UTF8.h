#ifndef UTF8_h
#define UTF8_h


#include <iostream>
#include <vector>
#include "reader.h"

const uint8_t   t1 = 0x00; // 0000 0000
const uint8_t	tx = 0x80; // 1000 0000
const uint8_t	t2 = 0xC0; // 1100 0000
const uint8_t	t3 = 0xE0; // 1110 0000
const uint8_t	t4 = 0xF0; // 1111 0000
const uint8_t	t5 = 0xF8; // 1111 1000
const uint8_t	t6 = 0xFC; // 1111 1100
const uint8_t	t7 = 0xFE; // 1111 1110
const uint8_t	t8 = 0xFF; // 1111 1111

const uint8_t	maskx = 0x3F; // 0011 1111
const uint8_t	mask2 = 0x1F; // 0001 1111
const uint8_t	mask3 = 0x0F; // 0000 1111
const uint8_t	mask4 = 0x07; // 0000 0111
const uint8_t	mask5 = 0x03; // 0000 0011
const uint8_t	mask6 = 0x01; // 0000 0001

using namespace std;

uint64_t decodeUTF8(Reader &r);

#endif
