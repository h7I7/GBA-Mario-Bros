////////////////////////////////////////////////////////////
//	File Name:		Text.c
//	Description:	The tiles and palette for the text sprites
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

const unsigned short TextTiles[768] __attribute__((aligned(4))) =
{
	0x1100, 0x0001, 0x0010, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110,
		0x0011, 0x0110, 0x0110, 0x0010, 0x1100, 0x0001, 0x0000, 0x0000,
		0x1000, 0x0001, 0x1100, 0x0001, 0x1000, 0x0001, 0x1000, 0x0001,
		0x1000, 0x0001, 0x1000, 0x0001, 0x1110, 0x0111, 0x0000, 0x0000,
		0x1110, 0x0011, 0x0011, 0x0110, 0x0000, 0x0111, 0x1100, 0x0011,
		0x1110, 0x0001, 0x0111, 0x0000, 0x1111, 0x0111, 0x0000, 0x0000,
		0x1110, 0x0111, 0x0000, 0x0011, 0x1000, 0x0001, 0x1100, 0x0011,
		0x0000, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,

		0x1000, 0x0011, 0x1100, 0x0011, 0x0110, 0x0011, 0x0011, 0x0011,
		0x1111, 0x0111, 0x0000, 0x0011, 0x0000, 0x0011, 0x0000, 0x0000,
		0x1111, 0x0011, 0x0011, 0x0000, 0x1111, 0x0011, 0x0000, 0x0110,
		0x0000, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,
		0x1100, 0x0011, 0x0110, 0x0000, 0x0011, 0x0000, 0x1111, 0x0011,
		0x0011, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,
		0x1111, 0x0111, 0x0011, 0x0110, 0x0000, 0x0011, 0x1000, 0x0001,
		0x1100, 0x0000, 0x1100, 0x0000, 0x1100, 0x0000, 0x0000, 0x0000,

		0x1110, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011,
		0x0011, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,
		0x1110, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110, 0x1110, 0x0111,
		0x0000, 0x0110, 0x0000, 0x0011, 0x1110, 0x0001, 0x0000, 0x0000,
		0x1100, 0x0001, 0x0110, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110,
		0x1111, 0x0111, 0x0011, 0x0110, 0x0011, 0x0110, 0x0000, 0x0000,
		0x1111, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110, 0x1111, 0x0011,
		0x0011, 0x0110, 0x0011, 0x0110, 0x1111, 0x0011, 0x0000, 0x0000,

		0x1100, 0x0011, 0x0110, 0x0110, 0x0011, 0x0000, 0x0011, 0x0000,
		0x0011, 0x0000, 0x0110, 0x0110, 0x1100, 0x0011, 0x0000, 0x0000,
		0x1111, 0x0001, 0x0011, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110,
		0x0011, 0x0110, 0x0011, 0x0011, 0x1111, 0x0001, 0x0000, 0x0000,
		0x1111, 0x0111, 0x0011, 0x0000, 0x0011, 0x0000, 0x1111, 0x0011,
		0x0011, 0x0000, 0x0011, 0x0000, 0x1111, 0x0111, 0x0000, 0x0000,
		0x1111, 0x0111, 0x0011, 0x0000, 0x0011, 0x0000, 0x1111, 0x0011,
		0x0011, 0x0000, 0x0011, 0x0000, 0x0011, 0x0000, 0x0000, 0x0000,

		0x1100, 0x0111, 0x0110, 0x0000, 0x0011, 0x0000, 0x0011, 0x0111,
		0x0011, 0x0110, 0x0110, 0x0110, 0x1100, 0x0111, 0x0000, 0x0000,
		0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110, 0x1111, 0x0111,
		0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110, 0x0000, 0x0000,
		0x1110, 0x0111, 0x1000, 0x0001, 0x1000, 0x0001, 0x1000, 0x0001,
		0x1000, 0x0001, 0x1000, 0x0001, 0x1110, 0x0111, 0x0000, 0x0000,
		0x1000, 0x0111, 0x0000, 0x0110, 0x0000, 0x0110, 0x0000, 0x0110,
		0x0011, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,

		0x0011, 0x0110, 0x0011, 0x0011, 0x1011, 0x0001, 0x1111, 0x0000,
		0x1111, 0x0001, 0x1011, 0x0011, 0x0011, 0x0111, 0x0000, 0x0000,
		0x0110, 0x0000, 0x0110, 0x0000, 0x0110, 0x0000, 0x0110, 0x0000,
		0x0110, 0x0000, 0x0110, 0x0000, 0x1110, 0x0111, 0x0000, 0x0000,
		0x0011, 0x0110, 0x0111, 0x0111, 0x1111, 0x0111, 0x1111, 0x0111,
		0x1011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110, 0x0000, 0x0000,
		0x0011, 0x0110, 0x0111, 0x0110, 0x1111, 0x0110, 0x1111, 0x0111,
		0x1011, 0x0111, 0x0011, 0x0111, 0x0011, 0x0110, 0x0000, 0x0000,

		0x1110, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110,
		0x0011, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,
		0x1111, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110,
		0x1111, 0x0011, 0x0011, 0x0000, 0x0011, 0x0000, 0x0000, 0x0000,
		0x1110, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110,
		0x1011, 0x0111, 0x0011, 0x0011, 0x1110, 0x0101, 0x0000, 0x0000,
		0x1111, 0x0011, 0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0111,
		0x1111, 0x0001, 0x1011, 0x0011, 0x0011, 0x0111, 0x0000, 0x0000,

		0x1110, 0x0001, 0x0011, 0x0011, 0x0011, 0x0000, 0x1110, 0x0011,
		0x0000, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,
		0x1110, 0x0111, 0x1000, 0x0001, 0x1000, 0x0001, 0x1000, 0x0001,
		0x1000, 0x0001, 0x1000, 0x0001, 0x1000, 0x0001, 0x0000, 0x0000,
		0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110,
		0x0011, 0x0110, 0x0011, 0x0110, 0x1110, 0x0011, 0x0000, 0x0000,
		0x0011, 0x0110, 0x0011, 0x0110, 0x0011, 0x0110, 0x0111, 0x0111,
		0x1110, 0x0011, 0x1100, 0x0001, 0x1000, 0x0000, 0x0000, 0x0000,

		0x0011, 0x0110, 0x0011, 0x0110, 0x1011, 0x0110, 0x1111, 0x0111,
		0x1111, 0x0111, 0x0111, 0x0111, 0x0011, 0x0110, 0x0000, 0x0000,
		0x0011, 0x0110, 0x0111, 0x0111, 0x1110, 0x0011, 0x1100, 0x0001,
		0x1110, 0x0011, 0x0111, 0x0111, 0x0011, 0x0110, 0x0000, 0x0000,
		0x0110, 0x0110, 0x0110, 0x0110, 0x0110, 0x0110, 0x1100, 0x0011,
		0x1000, 0x0001, 0x1000, 0x0001, 0x1000, 0x0001, 0x0000, 0x0000,
		0x1111, 0x0111, 0x0000, 0x0111, 0x1000, 0x0011, 0x1100, 0x0001,
		0x1110, 0x0000, 0x0111, 0x0000, 0x1111, 0x0111, 0x0000, 0x0000,

		0x1100, 0x0011, 0x0010, 0x0100, 0x1001, 0x1001, 0x0101, 0x1000,
		0x0101, 0x1000, 0x1001, 0x1001, 0x0010, 0x0100, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1110, 0x0111,
		0x1110, 0x0111, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0010, 0x0010, 0x0100, 0x0001,
		0x1000, 0x0000, 0x0100, 0x0001, 0x0010, 0x0010, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x1000, 0x0001, 0x1100, 0x0011, 0x1100, 0x0011, 0x1100, 0x0011,
		0x1000, 0x0001, 0x1000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000,

		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

const unsigned short TextPal[16] __attribute__((aligned(4))) =
{
	0x7E80,0x7FFF,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x7E80,0x021F,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};