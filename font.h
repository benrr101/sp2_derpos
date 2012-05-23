#ifndef _FONT_H_
#define _FONT_H_

//#define FONT_DEBUG

#include "font_define.h"

unsigned char FONT[95][FONT_HEIGHT] = {

// Space
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00000000
//00000000
//00010000
//00000000
//00000000
//00000000
//00000000
{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00},
//"
//00100100
//00100100
//00100100
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
//#
//00000000
//00000000
//00100100
//00100100
//01111110
//00100100
//00100100
//01111110
//00100100
//00100100
//00000000
//00000000
//00000000
{0x00, 0x00, 0x24, 0x24, 0x7e, 0x24, 0x24, 0x7e, 0x24, 0x24, 0x00, 0x00, 0x00},
//$
//00000000
//00001000
//00011100
//00101010
//00101000
//00011100
//00001010
//00101010
//00011100
//00001000
//00000000
//00000000
//00000000
{0x00, 0x08, 0x1c, 0x2a, 0x28, 0x1c, 0x0a, 0x2a, 0x1c, 0x08, 0x00, 0x00, 0x00},
//%
//00000000
//00100000
//01010010
//00100100
//00001000
//00010000
//00100000
//01000100
//00001010
//00000100
//00000000
//00000000
//00000000
{0x00, 0x20, 0x52, 0x24, 0x08, 0x10, 0x20, 0x44, 0x0a, 0x04, 0x00, 0x00, 0x00},
//&
//00000000
//00000000
//00011000
//00100100
//00100100
//00011000
//00101010
//01000100
//01000100
//00111010
//00000000
//00000000
//00000000
{0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x2a, 0x44, 0x44, 0x3a, 0x00, 0x00, 0x00},
//'
//00100000
//00100000
//00100000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
//(
//00000100
//00001000
//00001000
//00010000
//00010000
//00010000
//00010000
//00001000
//00001000
//00000100
//00000000
//00000000
//00000000
{0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x00, 0x00, 0x00},
//)
//00100000
//00010000
//00010000
//00001000
//00001000
//00001000
//00001000
//00010000
//00010000
//00100000
//00000000
//00000000
//00000000
{0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00},
//*
//00000000
//00000000
//00000000
//01000100
//00101000
//01111100
//00101000
//01000100
//00000000
//00000000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x44, 0x28, 0x7c, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00},
//+
//00000000
//00000000
//00000000
//00010000
//00010000
//01111100
//00010000
//00010000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00},
//`
//00100000
//00100000
//00100000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
//-
//00000000
//00000000
//00000000
//00000000
//00000000
//01111110
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
//.
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00100000
//00100000
//00100000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x00, 0x00},
///
//00000010
//00000100
//00000100
//00001000
//00001000
//00010000
//00010000
//00100000
//00100000
//01000000
//00000000
//00000000
//00000000
//00000000
{0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x00, 0x00, 0x00},
//NUMBERS
//00111100
//01000010
//01000110
//01001010
//01001010
//01010010
//01010010
//01100010
//01000010
//00111100
//00000000
//00000000
//00000000
    {0x3c, 0x42, 0x46, 0x4a, 0x4a, 0x52, 0x52, 0x62, 0x42, 0x3c, 0x00, 0x00, 0x00},
//00001000
//00011000
//00101000
//00001000
//00001000
//00001000
//00001000
//00001000
//00001000
//01111110
//00000000
//00000000
//00000000
    {0x08, 0x18, 0x48, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x7e, 0x00, 0x00, 0x00},
//00111100
//01000010
//00000010
//00000010
//00000100
//00001000
//00010000
//00100000
//01000000
//01111110
//00000000
//00000000
//00000000
    {0x3c, 0x42, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7e, 0x00, 0x00, 0x00},
//00111100
//01000010
//00000010
//00000010
//00011100
//00000010
//00000010
//00000010
//01000010
//00111100
//00000000
//00000000
//00000000
    {0x3c, 0x42, 0x02, 0x02, 0x1c, 0x02, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00, 0x00},
//00000100
//00001100
//00010100
//00100100
//01000100
//01111110
//00000100
//00000100
//00000100
//00000100
//00000000
//00000000
//00000000
    {0x04, 0x0c, 0x14, 0x24, 0x44, 0x7e, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00},
//01111110
//01000000
//01000000
//01000000
//01111100
//00000010
//00000010
//00000010
//01000010
//00111100
//00000000
//00000000
//00000000
    {0x7e, 0x40, 0x40, 0x40, 0x7c, 0x02, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00, 0x00},
//00011100
//00100000
//01000000
//01000000
//01111100
//01000010
//01000010
//01000010
//01000010
//00111100
//00000000
//00000000
//00000000
    {0x1c, 0x20, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
//01111110
//00000010
//00000100
//00000100
//00001000
//00001000
//00010000
//00010000
//00100000
//00100000
//00000000
//00000000
//00000000
    {0x7e, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x00, 0x00, 0x00},
//00111100
//01000010
//01000010
//01000010
//00111100
//01000010
//01000010
//01000010
//01000010
//00111100
//00000000
//00000000
//00000000
    {0x3c, 0x42, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
//00111100
//01000010
//01000010
//01000010
//01000010
//00111110
//00000010
//00000100
//00001000
//00110000
//00000000
//00000000
//00000000
    {0x3c, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x04, 0x08, 0x30, 0x00, 0x00, 0x00},
//:
//00000000
//00000000
//00000000
//00011000
//00011000
//00000000
//00000000
//00011000
//00011000
//00000000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},
//;
//00000000
//00000000
//00000000
//00011000
//00011000
//00000000
//00000000
//00011000
//00011000
//01110000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00},
//<
//00000000
//00000000
//00000000
//00000100
//00001000
//00010000
//00100000
//00010000
//00001000
//00000100
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00},
//=
//00000000
//00000000
//00000000
//01111110
//00000000
//00000000
//01111110
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
//>
//00000000
//00000000
//00000000
//00100000
//00010000
//00001000
//00000100
//00001000
//00010000
//00100000
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00, 0x00},
//?
//00000000
//00011000
//00100100
//01000010
//00000010
//00000100
//00001000
//00010000
//00000000
//00000000
//00010000
//00000000
//00000000
//00000000
{0x00, 0x18, 0x24, 0x42, 0x02, 0x04, 0x08, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00},
//@
//00111100
//01000010
//01000010
//01001110
//01010010
//01010010
//01001110
//01000000
//01000000
//00111110
//00000000
//00000000
//00000000
{0x3c, 0x42, 0x42, 0x4e, 0x52, 0x52, 0x4e, 0x40, 0x40, 0x3e, 0x00, 0x00, 0x00},
///////////
// UPPER CASE
////////////
 
//00011000
//00011000
//00100100
//00100100
//00100100
//00111100
//01000010
//01000010
//01000010
//01000010
//00000000
//00000000
//00000000
{0x18, 0x18, 0x24, 0x24, 0x24, 0x3A, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
//01111100
//01000010
//01000010
//01000010
//01111100
//01000010
//01000010
//01000010
//01000010
//01111100
//00000000
//00000000
//00000000
{0x7c, 0x42, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00, 0x00},
//00111100
//01000010
//01000000
//01000000
//01000000
//01000000
//01000000
//01000000
//01000010
//00111100
//00000000
//00000000
//00000000
{0x3c, 0x42, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00, 0x00},
//01111000
//01000100
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//01000100
//01111000
//00000000
//00000000
//00000000
{0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78, 0x00, 0x00, 0x00},
//01111110
//01000000
//01000000
//01000000
//01111100
//01000000
//01000000
//01000000
//01000000
//01111110
//00000000
//00000000
//00000000
{0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00, 0x00},
//01111110
//01000000
//01000000
//01000000
//01111100
//01000000
//01000000
//01000000
//01000000
//01000000
//00000000
//00000000
//00000000
{0x7e, 0x40, 0x40, 0x40, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00},
//00111100
//01000010
//01000000
//01000000
//01000000
//01000000
//01001110
//01000010
//01000010
//00111100
//00000000
//00000000
//00000000
{0x3c, 0x42, 0x40, 0x40, 0x40, 0x40, 0x4e, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
//01000010
//01000010
//01000010
//01000010
//01111110
//01000010
//01000010
//01000010
//01000010
//01000010
//00000000
//00000000
//00000000
{0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
//00111110
//00001000
//00001000
//00001000
//00001000
//00001000
//00001000
//00001000
//00001000
//00111110
//00000000
//00000000
//00000000
{0x3e, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00, 0x00, 0x00},
//00001110
//00000010
//00000010
//00000010
//00000010
//00000010
//00000010
//01000010
//01000010
//00111100
//00000000
//00000000
//00000000
{0x0e, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
//01000010
//01000100
//01001000
//01010000
//01110000
//01001000
//01001000
//01000100
//01000100
//01000010
//00000000
//00000000
//00000000
{0x42, 0x44, 0x48, 0x50, 0x70, 0x48, 0x48, 0x44, 0x44, 0x42, 0x00, 0x00, 0x00},
//01000000
//01000000
//01000000
//01000000
//01000000
//01000000
//01000000
//01000000
//01000000
//01111110
//00000000
//00000000
//00000000
{0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00, 0x00},
//01000010
//01100110
//01011010
//01011010
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//00000000
//00000000
//00000000
{0x42, 0x66, 0x5a, 0x5a, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
//01000010
//01100010
//01010010
//01001010
//01001010
//01001010
//01000110
//01000010
//01000010
//01000010
//00000000
//00000000
//00000000
{0x42, 0x62, 0x52, 0x4a, 0x4a, 0x4a, 0x46, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
//00111100
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//00111100
//00000000
//00000000
//00000000
{0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
//01111100
//01000010
//01000010
//01000010
//01000010
//01111100
//01000000
//01000000
//01000000
//01000000
//00000000
//00000000
//00000000
{0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00},
//00111100
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//01001010
//01000100
//00111010
//00000010
//00000000
//00000000
{0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x4a, 0x44, 0x3b, 0x02, 0x00, 0x00},
//01111100
//01000010
//01000010
//01000010
//01111100
//01000100
//01000010
//01000010
//01000010
//01000010
//00000000
//00000000
//00000000
{0x7c, 0x42, 0x42, 0x42, 0x7c, 0x44, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
//00111100
//01000010
//01000000
//01000000
//00111100
//00000010
//00000010
//00000010
//01000010
//00111100
//00000000
//00000000
//00000000
{0x3c, 0x42, 0x40, 0x40, 0x3c, 0x02, 0x02, 0x02, 0x42, 0x3c, 0x00, 0x00, 0x00},
//01111100
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00000000
//00000000
//00000000
{0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00},
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//01000010
//00111100
//00000000
//00000000
//00000000
{0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
//01000010
//01000010
//01000010
//01000010
//00100100
//00100100
//00100100
//00100100
//00011000
//00011000
//00000000
//00000000
//00000000
{0x42, 0x42, 0x42, 0x42, 0x24, 0x24, 0x24, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00},
//01000010
//01000010
//01000010
//01000010
//01000010
//01011010
//01011010
//00111100
//00100100
//00100100
//00000000
//00000000
//00000000
{0x42, 0x42, 0x42, 0x42, 0x42, 0x5a, 0x5a, 0x3c, 0x24, 0x24, 0x00, 0x00, 0x00},
//01000010
//01000010
//01000010
//00100100
//00011000
//00011000
//00100100
//01000010
//01000010
//01000010
//00000000
//00000000
//00000000
{0x42, 0x42, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
//01000010
//01000010
//01000010
//01000010
//00100100
//00011000
//00011000
//00011000
//00011000
//00011000
//00000000
//00000000
//00000000
{0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00},
//01111110
//00000100
//00000100
//00001000
//00001000
//00010000
//00010000
//00100000
//00100000
//01111110
//00000000
//00000000
//00000000
{0x7e, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x7e, 0x00, 0x00, 0x00},
//[
//00000000
//00011110
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00011110
//00000000
//00000000
//00000000
{0x00, 0x1e, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1e, 0x00, 0x00, 0x00},
// \ forward slash
//01000000
//00100000
//00100000
//00010000
//00010000
//00001000
//00001000
//00000100
//00000100
//00000010
//00000000
//00000000
//00000000
{0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x00, 0x00, 0x00},
//]
//00000000
//01111000
//00001000
//00001000
//00001000
//00001000
//00001000
//00001000
//00001000
//01111000
//00000000
//00000000
//00000000
{0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00, 0x00, 0x00},
//^
//00010000
//00010000
//00101000
//00101000
//01000100
//01000100
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
//_
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//01111110
//00000000
//00000000
//00000000
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00},
//'
//00100000
//00100000
//00100000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
//00000000
{0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/////////////////
// LOWER CASE
/////////////////
/*
00000000
00000000
00000000
00000000
00111100
00000010
00111110
01000010
01000010
00111110
00000000
00000000
00000000

*/
{0x00, 0x00, 0x00, 0x00, 0x3c, 0x02, 0x3e, 0x42, 0x42, 0x3e, 0x00, 0x00, 0x00},
/*
01000000
01000000
01000000
01000000
01111100
01000010
01000010
01000010
01000010
01111100
00000000
00000000
00000000
*/
{0x40, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
00111100
01000010
01000000
01000000
01000010
00111100
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00, 0x00},
/*
00000010
00000010
00000010
00000010
00111110
01000010
01000010
01000010
01000010
00111110
00000000
00000000
00000000
*/
{0x02, 0x02, 0x02, 0x02, 0x3e, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
00111100
01000010
01111110
01000000
01000010
00111100
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x7e, 0x40, 0x42, 0x3c, 0x00, 0x00, 0x00},
/*
00011110
00100000
00100000
00100000
01111000
00100000
00100000
00100000
00100000
00100000
00000000
00000000
00000000
*/
{0x1e, 0x20, 0x20, 0x20, 0x78, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
00111110
01000010
01000010
01000010
00111110
00000010
00000010
00111100
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x3e, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x3c, 0x00},
/*
01000000
01000000
01000000
01000000
01111100
01000010
01000010
01000010
01000010
01000010
00000000
00000000
00000000
*/
{0x40, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
/*
00001000
00000000
00000000
00011000
00001000
00001000
00001000
00001000
00001000
00011100
00000000
00000000
00000000
*/
{0x08, 0x00, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c, 0x00, 0x00, 0x00},
/*
00000010
00000000
00000000
00000110
00000010
00000010
00000010
00000010
00000010
00000010
00000010
00111100
00000000
*/
{0x02, 0x00, 0x00, 0x06, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3c, 0x00},
/*
01000000
01000000
01000000
01000000
01000010
01000100
01001000
01111000
01000100
01000010
00000000
00000000
00000000
*/
{0x40, 0x40, 0x40, 0x40, 0x42, 0x44, 0x48, 0x78, 0x44, 0x42, 0x00, 0x00, 0x00},
/*
00011000
00001000
00001000
00001000
00001000
00001000
00001000
00001000
00001000
00001100
00000000
00000000
00000000
*/
{0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0c, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01101000
01010100
01010100
01010100
01010100
01010100
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x68, 0x54, 0x54, 0x54, 0x54, 0x54, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01111100
01000010
01000010
01000010
01000010
01000010
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
00111100
01000010
01000010
01000010
01000010
00111100
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01111100
01000010
01000010
01000010
01000010
01111100
01000000
01000000
01000000
*/
{0x00, 0x00, 0x00, 0x00, 0x7c, 0x42, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x40, 0x40},
/*
00000000
00000000
00000000
00000000
00111110
01000010
01000010
01000010
01000010
00111110
00000010
00000010
00000010
*/
{0x00, 0x00, 0x00, 0x00, 0x3e, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x02},
/*
00000000
00000000
00000000
00000000
01011100
01100010
01000000
01000000
01000000
01000000
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x5c, 0x62, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
00111100
01000010
00111100
00000010
01000010
00111100
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x3c, 0x42, 0x3c, 0x02, 0x42, 0x3c, 0x00, 0x00, 0x00},
/*
00000000
00000000
00100000
00100000
01111100
00100000
00100000
00100000
00100000
00011110
00000000
00000000
00000000
*/
{0x00, 0x00, 0x20, 0x20, 0x7c, 0x20, 0x20, 0x20, 0x20, 0x1e, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01000010
01000010
01000010
01000010
01000010
00111110
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01000010
01000010
00100100
00100100
00011000
00011000
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01000010
01000010
01011010
01011010
00100100
00100100
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x5a, 0x5a, 0x24, 0x24, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01000010
00100100
00011000
00011000
00100100
01000010
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00},
/*
00000000
00000000
00000000
00000000
01000010
01000010
01000010
01000010
01000010
00111110
00000010
00000010
00111100
*/
{0x00, 0x00, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3e, 0x02, 0x02, 0x3c},
/*
00000000
00000000
00000000
00000000
01111110
00000100
00001000
00010000
00100000
01111110
00000000
00000000
00000000
*/
{0x00, 0x00, 0x00, 0x00, 0x7e, 0x04, 0x08, 0x10, 0x20, 0x7e, 0x00, 0x00, 0x00},
//{
//00000000
//00001110
//00010000
//00010000
//00010000
//01100000
//00010000
//00010000
//00010000
//00001110
//00000000
//00000000
//00000000
{0x00, 0x0e, 0x10, 0x10, 0x10, 0x60, 0x10, 0x10, 0x10, 0x0e, 0x00, 0x00, 0x00},
//|
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00010000
//00000000
//00000000
//00000000
{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00},
//}
//00000000
//01110000
//00001000
//00001000
//00001000
//00000110
//00001000
//00001000
//00001000
//01110000
//00000000
//00000000
//00000000
{0x54, 0x2a, 0x54, 0x2a, 0x54, 0x2a, 0x54, 0x2a, 0x54, 0x2a, 0x00, 0x00, 0x00},
//~
//00000000
//00000000
//00000000
//00000000
//00000000
//00100010
//01011010
//01000100
//00000000
//00000000
//00000000
//00000000
//00000000
{0x20, 0x20, 0x20, 0x00, 0x00, 0x22, 0x5a, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00}

    };



#endif //_FONT_H_
