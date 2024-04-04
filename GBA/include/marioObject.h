////////////////////////////////////////////////////////////
//	File Name:		marioObject.h
//	Description:	A header file for the class of the player object
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef _MARIOOBJECT_H_
#define _MARIOOBJECT_H_

#include "gba_types.h"
#include "gba_gfx.h"
#include "gba_vector2.h"
#include "sprite.h"
#include "gba_Particles.h"

class c_Mario
{
public:
	// A sprite object can hold the object normal/affine attributes
	s_Sprite	o_Sprite;

	// Particles for when mario jumps
	s_Particle o_Particles[5];

	// The state tracks which state of animation the player is currently in
	// also controls death
	e_SpriteState eo_State;

	// Which tile in TILEMEM we will store Mario's sprite
	u16 u16_TileMemPos;

	// Movement variables
	//	Position vector
	s_Vector2 o_Pos;
	//	Velocity vector
	s_Vector2 o_Vel;
	//	The minimum velocity we can have before the player stops moving and animating
	s_Vector2 o_MinVel;
	//	The maximum velocity the player can have whilst moving
	s_Vector2 o_MaxVel;
	// The gravity to be added to the player's velocity
	fixed f_Grav;
	// The amount we accelerate by when moving each update
	// Since this will need to be 0.05 (1/20) it saves dividing every update
	fixed f_Accel;
	// This is the vertical kill zone for Mario, if he falls bellow this he will die
	fixed f_yKillZone;

	// The position of the screen (origin is top left) in REG_BG_OFS[0]
	s_Vector2 o_WorldPos;

	// The left and right border of the screen
	// Mario can walk in between these without the camera/view port moving but if he
	// moves out of the them he will stay still and the camera/view port will move instead.
	// This coupled with map streaming gives the illusion of movement and a bigger map
	u8 u8_LeftBorder;
	u8 u8_RightBorder;

	// Similar to the left and right border only on a map scale.
	// Obviously we don't need the left one as that will always be 0
	// Simply the pixel amount the screen can travel to the right before it needs to stop
	// so the player doesn't run off the map
	fixed f_EndMapFlagRight;

	// The animation frame variable tracks which frame of animation the player is on
	u8 u8_AnimFrame;
	// A u8 being used as a bool for walking animation control
	u8 u8_WalkStart;

	// More u8's being used as bools to let us know if we can move left, right, up, and down
	u8 u8_MoveDown, u8_MoveUp, u8_MoveRight, u8_MoveLeft;
	// More u8's being used as bools to let us know if we need to reset the velocity on either
	// axis
	u8 u8_xResetVel, u8_yResetVel;

	// Initialise Mario
	void InitMario();
	// Update Mario
	void Update();
	// Test collision based off the direction the object is moving in
	void testCollision(fixed a_f_xDir, fixed a_f_yDir);
};

#endif // _MARIOOBJECT_H_