////////////////////////////////////////////////////////////
//	File Name:		gba_math.h
//	Description:	A header file for fixed point numbers
//					and sin/cosin lookup tables
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef __GBA_MATH_H__
#define __GBA_MATH_H__

#include "gba_types.h"
//\======================================================================
// Fixed Point Mathematics
//\======================================================================

// This fix shift defines the number of bits reserved for decimal values
// the GBA uses 8.8  fixed values for its affine's and transforms in OAM
#define FIX_SHIFT		8

#define FIX_SCALE		(1 << FIX_SHIFT		)
#define FIX_MASK		(FIX_SCALE - 1		)
#define FIX_SCALEF		((float)FIX_SCALE	)
#define FIX_SCALE_INV	(1.0f/FIX_SCALEF	)

#define FIX_ONE			FIX_SCALE

// Using a signed 32 bit value for fixed point 24.8 fixed
// The GBA uses signed 16 bit values as 8.8 fixed
// The additional 16 bit values will allow for better numerical overflow
typedef s32 fixed;
// Functions to convert from non-fixed point representations to fixed point
extern fixed	Int2Fix(s32 a_s32_i);
extern fixed	Float2Fix(float a_f);
// Functions to convert Fixed point to non-Fixed point representation
extern u32		Fix2uInt(fixed a_f);
extern u32		Fix2uFrac(fixed a_f);
extern s32		Fix2Int(fixed a_f);
extern float	Fix2Float(fixed a_f);
// Functions to undertake fixed point addition, subtraction, multiplication, and division
extern fixed	FixAdd(fixed a_f_a, fixed a_f_b);
extern fixed	FixSub(fixed a_f_a, fixed a_f_b);
extern fixed	FixMul(fixed a_f_a, fixed a_f_b);
extern fixed	FixDiv(fixed a_f_a, fixed a_f_b);
// Misc maths functions
extern fixed	FixAbs(fixed a_f);
extern fixed	FixSign(fixed a_f);


//\-------------------------------------------------------------------------------------
// Look up tables
extern fixed LU_Sin(u32 a_u32_theta);
extern fixed LU_Cos(u32 a_u32_theta);

#endif //__GBA_MATH_H__
