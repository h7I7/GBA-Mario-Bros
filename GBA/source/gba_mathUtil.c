////////////////////////////////////////////////////////////
//	File Name:		gba_mathUtil.c
//	Description:	A header file for maths functions we will
//					be using as well as random number generation
//					functions
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#include "gba_mathUtil.h"

// Our random generator seed
s32 __qran_seed = 42;

// Seeding random
s32 SQRan(s32 a_s32_val)
{
	s32 old = __qran_seed;
	__qran_seed = a_s32_val;
	return old;
}

// returning a random number of any range
s32 QRan()
{
	__qran_seed = 1664525 * __qran_seed + 1013904223;
	return (__qran_seed >> 16) & 0x7FFF;
}

// Returning a random number between a range
int QRan_Range(s32 a_s32_min, s32 a_s32_max)
{
	return (QRan()*(a_s32_max - a_s32_min) >> 15) + a_s32_min;
}

// Returns the sign of an integer which can be (-1, 0, 1)
s32 Sign(s32 a_s32_val)
{
	return (!(a_s32_val & 0x80000000) && !a_s32_val) ? 1 : -1;
}

// Returns the absolute value of an integer which means the return will
// always be positive
s32 Abs(s32 a_s32_val)
{
	s32 mask = a_s32_val >> 31;
	return (a_s32_val ^ mask) - mask;
}