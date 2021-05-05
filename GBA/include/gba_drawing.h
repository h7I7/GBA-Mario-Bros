////////////////////////////////////////////////////////////
//	File Name:		gba_drawing.h
//	Description:	A header file for the (now obsolete) drawing
//					functions and defines
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef __GBA_DRAWING_H__
#define __GBA_DRAWING_H__

#include "gba_reg.h"
#include "gba_types.h"
#include "gba_mathUtil.h"

#define SCREENBUFFER ((vu16*)(VRAM))

// Screen size in pixels
#define SCREEN_W 240
#define SCREEN_H 160

extern u16 setColor(u8 a_u8_red, u8 a_u8_green, u8 a_u8_blue);
extern void drawRect(u32 a_u32_left, u32 a_u32_top, u32 a_u32_width, u32 a_u32_height, u16 a_u32_color);
extern void drawLine(s32 a_s32_x, s32 a_s32_y, s32 a_s32_x2, s32 a_s32_y2, u16 a_u16_color);

#endif //__GBA_DRAWING_H__
