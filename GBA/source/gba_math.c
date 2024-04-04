////////////////////////////////////////////////////////////
//	File Name:		gba_math.c
//	Description:	A header file for fixed point numbers
//					and sin/cosin lookup tables
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#include "gba_math.h"

// Converting a signed integer to a fixed point value
fixed Int2Fix(s32 a_s32_i)
{
	return a_s32_i << FIX_SHIFT;
}

// Convert a floating point value to a fixed point value
fixed Float2Fix(float a_f)
{
	return (fixed)(a_f * FIX_SCALEF);
}


// Convert a fixed point value to an integer
u32	Fix2uInt(fixed a_f)
{
	return a_f >> FIX_SHIFT;
}

// Removing and returning the fractional component of a fixed point value
u32	Fix2uFrac(fixed a_f)
{
	return a_f & FIX_MASK;
}

// Converting a fixed point value to a signed integer
s32	Fix2Int(fixed a_f)
{
	return a_f / FIX_SCALE;
}

// Converting a fixed point value to a float
float Fix2Float(fixed a_f)
{
	return a_f / FIX_SCALEF;
}



// For these function to perform correctly fixed point numbers must be in
// the same format. The format for these numbers is 24.8
// Adding two fixed point values together
fixed FixAdd(fixed a_f_a, fixed a_f_b)
{
	return a_f_a + a_f_b;
}

// Subtracting fixed point values
fixed	FixSub(fixed a_f_a, fixed a_f_b)
{
	return a_f_a - a_f_b;
}

// Multiply two fixed point values together
// When multiplying the number of fractional bits needs to be adjusted
// a_fa has FIX_SHIFT decimal bits & a_fb has FIX_SHIFT decimal places
// This effectively shifts the number up and additional FIX_SHIFT number of bits
// THe result must be shifted down by FIX_SHIFT bits
fixed	FixMul(fixed a_f_a, fixed a_f_b)
{
	return (a_f_a * a_f_b) >> FIX_SHIFT;
}

// Divide two fixed point values
fixed	FixDiv(fixed a_f_a, fixed a_f_b)
{
	return ((a_f_a)* FIX_SCALE) / a_f_b;
}

// Return the absolute value
fixed FixAbs(fixed a_f)
{
	fixed mask = a_f >> 31;
	return (mask ^ a_f) - mask;
}

// Return the sign of a value (-1, 0, 1)
fixed FixSign(fixed a_f)
{
	return Int2Fix((a_f > 0) - (a_f < 0));
}

// These functions return values from the sin and cos lookup tables based on
// the input which acts as an index
#include "sinlut.h"
fixed LU_Sin(u32 a_u32_theta)
{
	return sin_lut[(a_u32_theta) & 0x1FF];
}

fixed LU_Cos(u32 a_u32_theta)
{
	return sin_lut[((a_u32_theta) + 128) & 0x1FF];
}