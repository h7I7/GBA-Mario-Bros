////////////////////////////////////////////////////////////
//	File Name:		gba_drawing.c
//	Description:	A header file for the (now obsolete) drawing
//					functions and defines
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "gba_drawing.h"

// Returns a color when given an RGB value
u16 setColor(u8 a_u8_red, u8 a_u8_green, u8 a_u8_blue)
{
	return (a_u8_red & 0x1F) | (a_u8_green & 0x1F) << 5 | (a_u8_blue & 0x1f) << 10;
}

// Draws a rectangle on screen
void drawRect(u32 a_u32_left, u32 a_u32_top, u32 a_u32_width, u32 a_u32_height, u16 a_u16_color)
{
	for (u32 u32_y = 0; u32_y < a_u32_height; ++u32_y)
	{
		for (u32 u32_x = 0; u32_x < a_u32_width; ++u32_x)
		{
			SCREENBUFFER[(a_u32_top + u32_y) * SCREEN_W + a_u32_left + u32_x] = a_u16_color;
		}
	}
}

// Draws a line from one point on the screen to another
void drawLine(s32 a_u32_x, s32 a_u32_y, s32 a_u32_x2, s32 a_u32_y2, u16 a_u16_color)
{
	s32 s32_w = a_u32_x2 - a_u32_x;
	s32 s32_h = a_u32_y2 - a_u32_y;
	s32 s32_dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (s32_w<0) s32_dx1 = -1; else if (s32_w>0) s32_dx1 = 1;
	if (s32_h<0) dy1 = -1; else if (s32_h>0) dy1 = 1;
	if (s32_w<0) dx2 = -1; else if (s32_w>0) dx2 = 1;
	s32 s32_longest = Abs(s32_w);
	s32 s32_shortest = Abs(s32_h);
	if (!(s32_longest>s32_shortest)) {
		s32_longest = Abs(s32_h);
		s32_shortest = Abs(s32_w);
		if (s32_h<0) dy2 = -1; else if (s32_h>0) dy2 = 1;
		dx2 = 0;
	}
	s32 s32_numerator = s32_longest >> 1;
	for (s32 s32_i = 0; s32_i <= s32_longest; s32_i++) {
		SCREENBUFFER[a_u32_y * SCREEN_W + a_u32_x] = a_u16_color;
		s32_numerator += s32_shortest;
		if (!(s32_numerator<s32_longest)) {
			s32_numerator -= s32_longest;
			a_u32_x += s32_dx1;
			a_u32_y += dy1;
		}
		else {
			a_u32_x += dx2;
			a_u32_y += dy2;
		}
	}
}