////////////////////////////////////////////////////////////
//	File Name:		sprite.h
//	Description:	A header file for the class of the sprite object that
//					is used for animation and changing of attribute & affine
//					data
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "gba_gfx.h"

// An enum for all the states a sprite can take
typedef enum e_SpriteState
{
	idle = 0,
	walking,
	jumping,
	deathAnim,
	dead
}e_SpriteState;

// The sprite stuct contains an attribute and an affine
typedef struct s_Sprite
{
	s_ObjectAttribute* o_Attribute;
	s_ObjectAffine* o_Affine;
}s_Sprite;

#endif //_SPRITE_H_