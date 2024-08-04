#include "FrameWork.h"

#define FLAG_A 0x4000000

Perk_t gPerk[ 119 ] = {
    { NULL, NULL, 0x48,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 5, 0, 0, 0, 0, 0 } }, // 0
    { NULL, NULL, 0x49,  1, 0x0F,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 6, 0 } }, // 1
    { NULL, NULL, 0x4A,  3, 0x03,  11, 0x02, -1, 0x00, 0, -1, 0, 	{ 6, 0, 0, 0, 0, 6, 0 } }, // 2
    { NULL, NULL, 0x4B,  2, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 5, 0 } }, // 3
    { NULL, NULL, 0x4C,  2, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 6, 6 } }, // 4
    { NULL, NULL, 0x4D,  1, 0x0F,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 6, 0, 0, 6, 7, 0 } }, // 5
    { NULL, NULL, 0x4E,  3, 0x03,  13, 0x02, -1, 0x00, 0, -1, 0, 	{ 0, 6, 0, 0, 0, 0, 0 } }, // 6
    { NULL, NULL, 0x4F,  3, 0x03,  14, 0x02, -1, 0x00, 0, -1, 0, 	{ 0, 0, 6, 0, 0, 0, 0 } }, // 7
    { NULL, NULL, 0x50,  3, 0x06,  15, 0x05, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 6 } }, // 8
    { NULL, NULL, 0x51,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0,	{ 0, 6, 0, 0, 0, 0, 0 } }, // 9

    { NULL, NULL, 0x52,  3, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 6, 0, 0, 0 } }, // 10    
    { NULL, NULL, 0x53,  2, 0x06,  31, 0x0F, -1, 0x00, 0, -1, 0, 	{ 0, 0, 6, 0, 4, 0, 0 } }, // 11
    { NULL, NULL, 0x54,  3, 0x03,  24, 0x0A, -1, 0x00, 0, -1, 0, 	{ 0, 0, 6, 0, 0, 0, 6 } }, // 12
    { NULL, NULL, 0x55,  3, 0x03,  12, 0x32, -1, 0x00, 0, -1, 0, 	{ 6, 0, 6, 0, 0, 0, 0 } }, // 13
    { NULL, NULL, 0x56,  1, 0x09,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 7, 0, 0, 6, 0, 0 } }, // 14
    { NULL, NULL, 0x57,  1, 0x06,  -1, 0x00,  8, 0x32, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 6, 0 } }, // 15
    { NULL, NULL, 0x58,  1, 0x03,  -1, 0x00, 17, 0x28, 0, -1, 0, 	{ 0, 0, 6, 0, 6, 0, 0 } }, // 16
    { NULL, NULL, 0x59,  1, 0x0C,  -1, 0x00, 15, 0x4B, 0, -1, 0, 	{ 0, 0, 0, 7, 0, 0, 0 } }, // 17
    { NULL, NULL, 0x5A,  3, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0,	{ 0, 0, 0, 0, 6, 0, 0 } }, // 18
    { NULL, NULL, 0x5B,  2, 0x03,  -1, 0x00,  6, 0x28, 0, -1, 0, 	{ 0, 7, 0, 0, 5, 6, 0 } }, // 19

    { NULL, NULL, 0x5C,  1, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 8 } }, // 20    
    { NULL, NULL, 0x5D,  1, 0x09,  16, 0x14, -1, 0x00, 0, -1, 0, 	{ 0, 6, 0, 0, 0, 4, 6 } }, // 21
    { NULL, NULL, 0x5E,  1, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 7, 0, 0, 5, 0, 0 } }, // 22
    { NULL, NULL, 0x5F,  1, 0x18,  -1, 0x00,  3, 0x50, 0, -1, 0, 	{ 8, 0, 0, 0, 0, 8, 0 } }, // 23
    { NULL, NULL, 0x60,  1, 0x18,  -1, 0x00,  0, 0x50, 0, -1, 0, 	{ 0, 8, 0, 0, 0, 8, 0 } }, // 24
    { NULL, NULL, 0x61,  1, 0x12,  -1, 0x00,  8, 0x50, 2,  3, 0x50, 	{ 0, 0, 0, 0, 0, 10, 0 } }, // 25
    { NULL, NULL, 0x62,  2, 0x0C,   8, 0x01, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 5, 0 } }, // 26
    { NULL, NULL, 0x63,  1, 0x136, -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 27
    { NULL, NULL, 0x64,  2, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 4, 0, 0, 0, 0 } }, // 28
    { NULL, NULL, 0x65,  1, 0x09,   9, 0x05, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 6, 0 } }, // 29

    { NULL, NULL, 0x66,  2, 0x06,  32, 0x19, -1, 0x00, 0, -1, 0, 	{ 0, 0, 3, 0, 0, 0, 0 } }, // 30
    { NULL, NULL, 0x67,  1, 0x0C,  -1, 0x00, 13, 0x28, 1, 12, 0x28, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 31
    { NULL, NULL, 0x68,  1, 0x0C,  -1, 0x00,  6, 0x28, 1,  7, 0x28, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 32
    { NULL, NULL, 0x69,  1, 0x0C,  -1, 0x00, 10, 0x32, 2,  9, 0x32, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 33
    { NULL, NULL, 0x6A,  1, 0x09,  -1, 0x00, 14, 0x32, 0, -1, 0,	{ 0, 0, 0, 0, 0, 0, 0 } }, // 34
    { NULL, NULL, 0x6B,  3, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ -9, 0, 0, 0, 0, 0, 0 } }, // 35
    { NULL, NULL, 0x6C,  1, 0x136, -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 4, 0, 0, 0, 0, 0 } }, // 36
    { NULL, NULL, 0x6D,  1, 0x0F,  -1, 0x00, 10, 0x50, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 8, 0 } }, // 37
    { NULL, NULL, 0x6E,  1, 0x06,  -1, 0x00,  8, 0x3C, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 38
    { NULL, NULL, 0x6F,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 10, 0, 0, 0 } }, // 39

    { NULL, NULL, 0x70,  1, 0x136, -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 8 } }, // 40
    { NULL, NULL, 0x71,  1, 0x09,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 41
    { NULL, NULL, 0x72,  1, 0x136, -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 5, 0, 0, 0, 0 } }, // 42
    { NULL, NULL, 0x73,  2, 0x06,  -1, 0x00, 17, 0x28, 0, -1, 0, 	{ 0, 0, 6, 0, 0, 0, 0 } }, // 43
    { NULL, NULL, 0x74,  1, 0x136, -1, 0x00, 17, 0x19, 0, -1, 0, 	{ 0, 0, 0, 0, 5, 0, 0 } }, // 44
    { NULL, NULL, 0x75,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 7, 0, 0, 0, 0, 0 } }, // 45
    { NULL, NULL, 0x76,  1, 0x09,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 4 } }, // 46
    { NULL, NULL, 0x77,  1, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 6, 0, 0, 0, 0, 0 } }, // 47
    { NULL, NULL, 0x78,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 5, 0 } }, // 48
    { NULL, NULL, 0x79,  3, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 4, 0, 0 } }, // 49

    { NULL, NULL, 0x7A,  3, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 4, 0, 0 } }, //50
    { NULL, NULL, 0x7B,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, //51
    { NULL, NULL, 0x7C,  1, 0x09,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, //52
    { NULL, NULL, 0x7D, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 53
    { NULL, NULL, 0x7E, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ -2, 0, -2, 0, 0, -3, 0 } }, // 54
    { NULL, NULL, 0x7F, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, -3, -2, 0 } }, // 55
    { NULL, NULL, 0x80, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, -2, 0, 0 } }, // 56
    { NULL, NULL, 0x81, -1, 0x01,  31,  -14, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 57
    { NULL, NULL, 0x82, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 58
    { NULL, NULL, 0x83, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 59

    { NULL, NULL, 0x84, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 60
    { NULL, NULL, 0x85, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 61
    { NULL, NULL, 0x86, -1, 0x01,  31, 0x1E, -1, 0x00, 0, -1, 0, 	{ 3, 0, 0, 0, 0, 0, 0 } }, // 62
    { NULL, NULL, 0x87, -1, 0x01,  31, 0x14, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 63
    { NULL, NULL, 0x88, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 64
    { NULL, NULL, 0x89, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 65
    { NULL, NULL, 0x8A, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 66
    { NULL, NULL, 0x8B, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } }, // 67
    { NULL, NULL, 0x8C, -1, 0x01,  31, 0x3C, -1, 0x00, 0, -1, 0, 	{ 4, 0, 0, 0, 0, 0, 0 } }, // 68 Armor advanced I
    { NULL, NULL, 0x8D, -1, 0x01,  31, 0x4B, -1, 0x00, 0, -1, 0, 	{ 4, 0, 0, 0, 0, 0, 0 } }, // 69 Armor advanced II
    { NULL, NULL, 0x88, -1, 0x01,   8,   -1, -1, 0x00, 0, -1, 0, 	{ -1, -1, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x95, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, -2, 0, 0, -1, 0, -1 } },
    { NULL, NULL, 0x9A, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0,	{ 0, 0, 0, 2, 0, 0, 0 } },
    { NULL, NULL, 0x9E, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x9D, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x9D, -1, 0x01,   3,   -1, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xA8, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xA8, -1, 0x01,   3,   -1, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xAC, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x9B,  1, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ -10, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x9C,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 6, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x7A,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 6, 0, 0 } },
    { NULL, NULL, 0x27,  1, 0x09,  -1, 0x00, 11, 0x4B, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 4, 0 } },
    { NULL, NULL, 0x2C,  1, 0x06,  -1, 0x00, 16, 0x32, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x00,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ -10, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x01,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, -10, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x02,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, -10, 0, 0, 0, 0 } },
    { NULL, NULL, 0x03,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, -10, 0, 0, 0 } },
    { NULL, NULL, 0x04,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, -10, 0, 0 } },
    { NULL, NULL, 0x05,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, -10, 0 } },
    { NULL, NULL, 0x06,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, -10 } },
    { NULL, NULL, 0xA0,  1, 0x06,  -1, 0x00, 10, 0x32, 2, FLAG_A, 0x32, { 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xA1,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x9F,  1, 0x0C,  -1, 0x00,  3, 0x4B, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xA3,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 5, 0, 0, 5, 0 } },
    { NULL, NULL, 0xA2,  1, 0x09,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 6, 0, 0, 0 } },
    { NULL, NULL, 0xA4,  1, 0x09,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 5, 5 } },
    { NULL, NULL, 0xA5,  1, 0x0C,  -1, 0x00,  7, 0x3C, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xA6,  1, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, -10, 0, 0, 0 } },
    { NULL, NULL, 0x2B,  1, 0x06,  -1, 0x00, 15, 0x32, 2, 14, 0x32, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xA7,  1, 0x06,  12, 0x32, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xA9,  1, 0x09,  -1, 0x00,  1, 0x4B, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xAA,  1, 0x06,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 5, 0 } },
    { NULL, NULL, 0x79,  1, 0x06,  -1, 0x00, 15, 0x32, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xAB,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 6, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x26,  1, 0x03,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0xAD,  1, 0x0C,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ -7, 0, 0, 0, 0, 5, 0 } },
    { NULL, NULL, 0x68, -1, 0x01,  -1, 0x00,  7, 0x4B, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x8E, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x8E, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x34, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x34, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x68, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x68, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x23, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x23, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x9A, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x9A, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0, 	{ 0, 0, 0, 0, 0, 0, 0 } },
    { NULL, NULL, 0x40, -1, 0x01,  -1, 0x00, -1, 0x00, 0, -1, 0,	{ 0, 0, 0, 0, 0, 0, 0 } }
};


Msg_t gMsgPerk;
int *gPerkLvlData; // [gPambCount][119]
int gPerkUnk01;

void PerkInit()
{
    int i;
    char stmp[260];
    MsgLine_t MsgLine;

    gPerkLvlData = (int *)Malloc( 119 * sizeof(int) * gPartyCount ); // 119 * 4 * ...    
    if( !gPerkLvlData ) return;    
    memset( gPerkLvlData, 0, 119 * sizeof(int) * gPartyCount );
    PerkReset();
    if( MessageInit( &gMsgPerk ) != 1 ) return;
    sprintf( stmp, "%s%s", gGamePath, "perk.msg" );
    if( MessageLoad( &gMsgPerk, stmp ) != 1 ) return;
    for( i = 0; i < 119; i++ ){
        MsgLine.Id = i + 101;
        if( MessageGetMsg( &gMsgPerk, &MsgLine ) == 1 ) gPerk[ i ].Name = MsgLine.Text;
        MsgLine.Id = i + 1101;
        if( MessageGetMsg( &gMsgPerk, &MsgLine ) == 1 ) gPerk[ i ].Dsc = MsgLine.Text;
    }
}

int PerkZero()
{
    PerkReset();
    return 0;
}

void PerkClose()
{
    MessageClose( &gMsgPerk );
    if( !gPerkLvlData ) return;
    Free( gPerkLvlData );
    gPerkLvlData = NULL;
}

int PerkLoad( xFile_t *fh )
{
    int i, j, *p;
    for( i = 0; i < gPartyCount; i++ ){
	p = &gPerkLvlData[ 119 * i ];
	for( j = 0; j < 119; j++ ){
    	    if( dbgetBei( fh, &p[ j ] ) == -1 ) return -1;
        }
    }
    return 0;                
}

int PerkSave( xFile_t *fh )
{
    int i, j, *p;

    for( i = 0; i < gPartyCount; i++ ){
	p = &gPerkLvlData[ 119 * i ];
	for( j = 0; j < 119; j++ ){
    	    if( dbputBei( fh, p[ j ] ) == -1 ) return -1;
        }
    }
    return 0;                
}

int *PerkGetLevelData( Obj_t *dude )
{
    int i;
    
    if( dude == gObjDude ) return gPerkLvlData;
    for( i = 1; i < gPartyCount; i++ ){
        if( dude->Pid == gPartyPids[ i ] ) return &gPerkLvlData[ 119 * i ];
    }
    eprintf("Error: perkGetLevelData: Can't find party member match!");
    return gPerkLvlData;    
}

int PerkLvlUpAvail( Obj_t *dude, unsigned int PerkIdx )
{
    Perk_t *Perk;
    unsigned int bb;
    int dd, *LevelData,cc, i11, v15, i;

    dd = 1;
    if( PerkIdx >= 119 ) return 0;
    Perk = &gPerk[ PerkIdx ];
    if( Perk->MaxLvl == -1 ) return 0;
    LevelData = PerkGetLevelData( dude );
    if( LevelData[ PerkIdx ] >= Perk->MaxLvl ) return 0;
    if( dude == gObjDude ){
        if( Perk->Pe > FeatGetPoints( FEAT_PERCEPTION ) ) return 0;
    }
    if( Perk->i08 != -1 ){
	bb = Perk->i08;
	cc = 0;
	if( Perk->i08 & 0x4000000 ){
    	    cc = 1;
	    bb = Perk->i08 & ~0x4000000;
	}
	if( Perk->i09 < 0 ){
	    if( cc ){
        	if( GlobVarGet( bb ) >= Perk->i09 ) dd = 0;
	    } else {
		if( SkillGetTotal( gObjDude, bb ) >= -Perk->i09 ) dd = 0;
	    }
	} else if( cc ){
	    if( GlobVarGet(bb) < Perk->i09 ) dd = 0;
	} else {
	    if( SkillGetTotal( gObjDude, bb ) < Perk->i09 ) dd =0;
	}
    }
    if( !dd || Perk->i10 == 2 ){
        if( !Perk->i10 ) return 0;
        if( !dd && Perk->i10 == 2 ) return 0;
        i11 = Perk->i11;
        v15 = 0;
        if( i11 != -1 && (i11 & 0x4000000) != 0 ){
            v15 = 1;
            i11 &= ~0x4000000;
        }
        if( i11 == -1 ) return 0;
        if( Perk->i12 >= 0 ){
            if( v15 ){
                if( GlobVarGet( i11 ) < Perk->i12 ) return 0;
            } else {
        	if( SkillGetTotal( gObjDude, i11 ) < Perk->i12 ) return 0;
            }
        } else {
    	    if( v15 ){
    		if( GlobVarGet( i11 ) >= Perk->i12 ) return 0;
    	    } else {
        	if( SkillGetTotal( gObjDude, i11 ) >= -Perk->i12 ) return 0;
    	    }
        }
    }
    for( i = 0; i < 7; i++ ){
        if( Perk->FeatsMod[ i ] < 0 ){
            if( FeatGetVal( gObjDude, i ) >= -Perk->FeatsMod[ i ] ) return 0;
        } else {
            if( FeatGetVal( gObjDude, i ) < Perk->FeatsMod[ i ] ) return 0;
        }
    }
    return 1;
}

void PerkReset()
{
    int i;

    for( i = 0; i < gPartyCount; i++ ){
        memset( gPerkLvlData + 119 * i, 0, 119 );
    }
}

int PerkLvlInc( Obj_t *dude, unsigned int PerkIdx )
{
    if( PerkIdx >= 119 ) return -1;
    if( !PerkLvlUpAvail( dude, PerkIdx ) ) return -1;
    PerkGetLevelData( dude ) [PerkIdx ]++;
    PerkAddEffect( dude, PerkIdx );
    return 0;
}

int PerkLvlUp( Obj_t *dude, unsigned int SkillIdx )
{
    int *LevelData, MaxLvl;

    if( SkillIdx >= 119 ) return -1;
    LevelData = PerkGetLevelData( dude );
    MaxLvl = gPerk[ SkillIdx ].MaxLvl;
    if( LevelData[ SkillIdx ] >= MaxLvl && MaxLvl != -1 ) return -1;
    LevelData[ SkillIdx ]++;
    PerkAddEffect( dude, SkillIdx );
    return 0;
}

int PerkLvlDec( Obj_t *dude, unsigned int PerkIdx )
{
    int *LevelData;

    if( PerkIdx >= 119 ) return -1;
    LevelData = PerkGetLevelData( dude );
    if( LevelData[ PerkIdx ] < 1 ) return -1;
    LevelData[ PerkIdx ]--;
    PerkRemoveEffect( dude, PerkIdx );
    return 0;
}

int PerkGetAvail( Obj_t *dude, int *Avail )
{
    int i, n;

    for( i = 0, n = 0; i < 119; i++ ){
        if( PerkLvlUpAvail( dude, i ) ) Avail[ n++ ] = i;
    }
    return n;
}

int PerkLvl( Obj_t *dude, unsigned int PerkIdx )
{
    if( PerkIdx >= 119 ) return 0;
    return PerkGetLevelData( dude )[ PerkIdx ];
}

char *PerkGetName( unsigned int PerkIdx )
{
    if( PerkIdx >= 119 ) return 0;
    return gPerk[ PerkIdx ].Name;    
}

char *PerkGetDsc( unsigned int PerkIdx )
{
    if( PerkIdx >= 119 ) return 0;
    return gPerk[ PerkIdx ].Dsc;
}

int PerkGetImgId( unsigned int PerkIdx )
{
    if( PerkIdx >= 119 ) return 0;
    return gPerk[ PerkIdx ].ImgId;
}

void PerkAddEffect( Obj_t *dude, int PerkIdx )
{
    int *lvl, i;

    if( OBJTYPE( dude->Pid ) != TYPE_CRIT ){ 
	eprintf( "ERROR: perk_remove_effect: Was called on non-critter!" );
	return;
    }
    if( PerkIdx >= 119 ) return;
    if( gPerk[ PerkIdx ].i06 != -1 ) FeatSetBoost( dude, gPerk[ PerkIdx ].i06, FeatGetBoost( dude, gPerk[ PerkIdx ].i06 ) + gPerk[ PerkIdx ].i07 );
    if( PerkIdx == 92 ){ // here and now!
        lvl = PerkGetLevelData( dude );
        lvl[ 92 ]--;
        gPerkUnk01 = FeatPtsPerLvl( FeatGetPoints( 1 ) + 1 ) - FeatGetPoints( 2 );
        FeatLvlUpStats( gPerkUnk01, 0 );
        lvl[ 92 ]++;
    }
    if( gPerk[ PerkIdx ].MaxLvl == -1 ){
        for( i = 0; i < 7; i++ ) FeatSetBoost( dude, i, FeatGetBoost(dude, i) + gPerk[ PerkIdx ].FeatsMod[i] );    
    }
}

void PerkRemoveEffect( Obj_t *dude, unsigned int PerkIdx )
{
    Perk_t *p;
    int i;

    if( OBJTYPE( dude->Pid ) != TYPE_CRIT ){
        eprintf("ERROR: perk_remove_effect: Was called on non-critter!");
        return;
    }
    if( PerkIdx >= 119 ) return;
    p = &gPerk[ PerkIdx ];
    if( p->i06 != -1 ) FeatSetBoost( dude, p->i06, FeatGetBoost( dude, p->i06 ) - p->i07 );    
    if( PerkIdx == 92 ) FeatSetPoints( 2, FeatGetPoints( 2 ) - gPerkUnk01 );
    if( p->MaxLvl == -1 ){
        for( i = 0; i < 7; i++ ){
            FeatSetBoost( dude, i, FeatGetBoost( dude, i ) - p->FeatsMod[ i ] );
        }
    }
}

int PerkGrantPts( Obj_t *dude, int lvl )
{
    int pts;

    pts = 0;
    switch( lvl ){
        case 6:
    	    if( PerkGetLevelData( dude )[ 32 ] ) pts = 10;
            if( PerkGetLevelData( dude )[ 107 ] ) return pts + 5;
            return pts;            
        case 7:
    	    if( PerkGetLevelData( dude )[ 32 ] ) pts = 10;
    	    if( PerkGetLevelData( dude )[ 97 ] ) pts += 10;
            if( PerkGetLevelData( dude )[ 107 ] ) return pts + 5;
            return pts;            
        case 8:
            if( PerkGetLevelData( dude )[ 38 ] ){
        	if( ObjGetLightIntensity( gObjDude ) <= 45875 ) pts = 20;
    	    }
    	case 3:
        case 9 ... 11:
    	    if( PerkGetLevelData( dude )[ 105 ] ) pts = 10;
    	    if( PerkGetLevelData( dude )[ 33 ] ){
    		if( lvl == 10 || lvl == 9 ) pts += 15;
    	    }
            if( PerkGetLevelData( dude )[ 91 ] == 0 ) return pts;
            if( lvl != 10 ) pts += 20;
            return pts;
        case 12 ... 13:
            if( PerkGetLevelData(dude)[31] == 0 ) return pts;
            return pts + 10;
        case 14:
    	    if( PerkGetLevelData( dude )[ 34 ] ) pts = 20;
    	    if( PerkGetLevelData( dude )[ 116 ] ) pts += 5;
        case 15:
            if( PerkGetLevelData( dude )[ 99 ] ) pts += 10;
	    if( lvl != 15 ) return pts;
	    if( PerkLvl( dude, 103 ) )pts += 20;
	    return pts;            
        case 16:
            if( !PerkGetLevelData( dude )[ 83 ] ) return pts;
            return 20;
        case 17:
    	    if( PerkGetLevelData( dude )[ 47 ] ) pts = 15;    	    
            if( PerkGetLevelData( dude )[ 16 ] ) pts += 25;
            return pts;
    }
    return 0;
}

