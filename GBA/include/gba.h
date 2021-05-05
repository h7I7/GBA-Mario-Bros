////////////////////////////////////////////////////////////
//	File Name:		gba.h
//	Description:	A header file containing all the includes
//					we will need in this projects
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef __GBA_H__
#define __GBA_H__

// The screen size in pixels
#define SCREENWIDTH 240
#define SCREENHEIGHT 160

// The screen size in tiles
#define SCREENWIDTHTILES 32
#define SCREENHEIGHTTILES 20

// This forces the compiler to regognise the following code as C
#ifdef __cplusplus
extern "C" {
#endif

#include "m_GameManager.h"

#include "textObject.h"

#include "gba_types.h"
#include "gba_reg.h"
#include "gba_input.h"
#include "gba_mathUtil.h"
#include "gba_math.h"
#include "gba_gfx.h"
#include "gba_drawing.h"
#include "gba_timers.h"
#include "gba_dma.h"

#ifdef __cplusplus
};
#endif

#endif //__GBA_H__