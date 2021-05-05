////////////////////////////////////////////////////////////
//	File Name:		gba_input.c
//	Description:	A header file for the input functionality
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "gba_input.h"

//set the extern valuse for key values
u16 u16__currKeys = 0, u16__prevKeys = 0;

// Get the direction of keys in the form of a sign value (-1, 0, 1)
// that represents which direction the player is trying to move on either
// the horizontal or vertical axis
s32		GetAxis(e_AXIS a_eo_val)
{
	switch (a_eo_val)
	{
	case HORIZONTAL:
		//shift __currKeys down 4 to move the value for RIGHT to the lowest bit & with 1 do the same for LEFT and subtract
		//if right is pressed the equation becomes 1 - 0 = 1, if Left is pressed then 0 - 1 = -1.
		return ((u16__currKeys >> 4) & 1) - ((u16__currKeys >> 5) & 1);
	case VERTICAL:
		//This is the same logic as above however uses UP and DOWN.
		return ((u16__currKeys >> 6) & 1) - ((u16__currKeys >> 7) & 1);
	default:
		return 0;
	}
}