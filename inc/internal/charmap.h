#ifndef __CHARMAP_H
#define __CHARMAP_H

// convert ascii to smb1 charset
// space
#pragma charmap ( 0x20, 0x24 )
// !
#pragma charmap ( 0x21, 0x2B )
// -
#pragma charmap ( 0x2D, 0x28 )
// x to cross
#pragma charmap ( 0x78, 0x29 )
// 0x to coin
#pragma charmap ( 0x24, 0x2e )
// c to copyright
#pragma charmap ( 0x62, 0x2f )
// m to mushroom
#pragma charmap ( 0x6D, 0x2a )
// / to chain
#pragma charmap ( 0x2F, 0x2d )
// .
#pragma charmap ( 0x2E, 0x2c )
// <  (arrow left)
#pragma charmap ( 0x3C, 0xEE )
// >  (arrow right)
#pragma charmap ( 0x3E, 0xEF )

// 0
#pragma charmap ( 0x30, 0x00 )
// 1
#pragma charmap ( 0x31, 0x01 )
// 2
#pragma charmap ( 0x32, 0x02 )
// 3
#pragma charmap ( 0x33, 0x03 )
// 4
#pragma charmap ( 0x34, 0x04 )
// 5
#pragma charmap ( 0x35, 0x05 )
// 6
#pragma charmap ( 0x36, 0x06 )
// 7
#pragma charmap ( 0x37, 0x07 )
// 8
#pragma charmap ( 0x38, 0x08 )
// 9
#pragma charmap ( 0x39, 0x09 )

// A
#pragma charmap ( 0x41, 0x0A )
// B
#pragma charmap ( 0x42, 0x0B )
// C
#pragma charmap ( 0x43, 0x0C )
// D
#pragma charmap ( 0x44, 0x0D )
// E
#pragma charmap ( 0x45, 0x0E )
// F
#pragma charmap ( 0x46, 0x0F )
// G
#pragma charmap ( 0x47, 0x10 )
// H
#pragma charmap ( 0x48, 0x11 )
// I
#pragma charmap ( 0x49, 0x12 )
// J
#pragma charmap ( 0x4A, 0x13 )
// K
#pragma charmap ( 0x4B, 0x14 )
// L
#pragma charmap ( 0x4C, 0x15 )
// M
#pragma charmap ( 0x4D, 0x16 )
// N
#pragma charmap ( 0x4E, 0x17 )
// O
#pragma charmap ( 0x4F, 0x18 )
// P
#pragma charmap ( 0x50, 0x19 )
// Q
#pragma charmap ( 0x51, 0x1A )
// R
#pragma charmap ( 0x52, 0x1B )
// S
#pragma charmap ( 0x53, 0x1C )
// T
#pragma charmap ( 0x54, 0x1D )
// U
#pragma charmap ( 0x55, 0x1E )
// V
#pragma charmap ( 0x56, 0x1F )
// W
#pragma charmap ( 0x57, 0x20 )
// X
#pragma charmap ( 0x58, 0x21 )
// Y
#pragma charmap ( 0x59, 0x22 )
// Z
#pragma charmap ( 0x5A, 0x23 )


#endif