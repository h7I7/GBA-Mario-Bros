////////////////////////////////////////////////////////////
//	File Name:		textLut.c
//	Description:	An ASCII lookup table for the alphabet in
//					capitals and numbers 0-9 plus some extra
//					characters
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

const unsigned short textLut[64]__attribute__((aligned(4))) =
{
	// Numbers in order
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
	// Letters in order
	65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,

	// Padding
	0, 0, 0, 0,
	// - and * symbol
	45, 120,
	// Pading cont.
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};