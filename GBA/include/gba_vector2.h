////////////////////////////////////////////////////////////
//	File Name:		gba_vector2.h
//	Description:	A header file for the vector2 class
//					which is a container for 2 fixed variables
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef _GBA_VECTOR2_H_
#define _GBA_VECTOR2_H_

#include "gba_math.h"

// A struct containing two fixed point values
typedef struct s_Vector2
{
	fixed f_x, f_y;
}__attribute__((aligned(4)))s_Vector2;

#endif // !_GBA_VECTOR2_H_
