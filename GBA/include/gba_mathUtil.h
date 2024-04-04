////////////////////////////////////////////////////////////
//	File Name:		gba_mathUtil.h
//	Description:	A header file for maths functions we will
//					be using as well as random number generation
//					functions
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef __GBA_MATHUTIL_H__
#define __GBA_MATHUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gba_types.h"

// RNG functions
// The random seed
extern s32 __qran_seed;
// A function to seed random with a different value
extern s32 SQRan(s32 a_s32_val);
// Returns a random number
extern s32 QRan();
// Returns a random number from a range
extern int QRan_Range(s32 a_s32_min, s32 a_s32_max);
// Gets the sign of a value
extern s32 Sign(s32 a_s32_val);
// Gets the absolute value of the number passed in
extern s32 Abs(s32 a_s32_val);

#ifdef __cplusplus
}
#endif

#endif //__GBA_MATHUTIL_H__