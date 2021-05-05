////////////////////////////////////////////////////////////
//	File Name:		Text.h
//	Description:	The tiles and palette for the text sprites
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef _TEXT_H
#define _TEXT_H

#define TextTilesWidth 16
#define TextTilesHeight 3

#define TextTilesCharLen 96
#define TextTilesLen 1536
extern const unsigned short TextTiles[768];

#define TextPalLen		32
#define TextPalLenHalf	16
extern const unsigned short TextPal[16];

#endif //_TEXT_H