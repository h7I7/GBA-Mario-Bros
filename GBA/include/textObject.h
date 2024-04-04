////////////////////////////////////////////////////////////
//	File Name:		textObject.h
//	Description:	A header file for the class of the text object class.
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef _TEXTOBJECT_H_
#define _TEXTOBJECT_H_

#include "gba_types.h"

// The text class contains an array of indexes, and a length
// The array of indexes are the OAM locations of all the sprites
// that makes up the object text
// The length is long the work we want to display is
// 16 is the max number of characters we can have because
// I don't imagine I'll ever want to have a word longer than that
// If I do need it longer I will just use multiple c_Text objects
// If I had the array of indexes be too long it would waste memory
class c_Text
{
public:
	u8			u8_Index[16];
	u8			u8_Length;
};

#endif // _TEXTOBJECT_H_
