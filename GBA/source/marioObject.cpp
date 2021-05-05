////////////////////////////////////////////////////////////
//	File Name:		marioObject.cpp
//	Description:	A header file for the class of the player object
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "marioObject.h"
#include "Mario_Sheet.h"
#include "Level1Data.h"
#include "BigCoins_Sheet.h"

#include "gba_dma.h"
#include "gba_input.h"
#include "gba_timers.h"
#include "gba_Particles.h"
#include "gba_mathUtil.h"

// Initialising all the variables used in objects of this class,
// basically a constructor without being a constructor
// as we cannot use those
void c_Mario::InitMario()
{
	// Tile memory settings
	u16_TileMemPos = 960;

	// Animation settings
	u8_AnimFrame = 0;
	u8_WalkStart = 0;

	// Position
	o_Pos.f_x = Int2Fix(120);
	o_Pos.f_y = Int2Fix(120);

	// Velocity
	o_Vel.f_x = Int2Fix(0);
	o_Vel.f_y = Int2Fix(0);

	// Max velocity
	o_MaxVel.f_x = Int2Fix(1.5);
	o_MaxVel.f_y = Int2Fix(5);

	o_MinVel.f_x = FixDiv(Int2Fix(1), Int2Fix(2));

	// Resetting of velocities
	u8_xResetVel = 0;
	u8_yResetVel = 0;

	// Gravity
	f_Grav = FixDiv(Int2Fix(1), Int2Fix(5));

	// Acceleration
	f_Accel = FixDiv(Int2Fix(1), Int2Fix(20));

	// Setting the vertical kill zone, 16 pixels from the bottom of the screen
	f_yKillZone = Int2Fix(160 - 16);

	// World position (screen position on world map)
	o_WorldPos.f_x = 0;
	o_WorldPos.f_y = 0;

	// Borders for screen movement
	u8_LeftBorder = 88;
	u8_RightBorder = 152;

	// When the camera should stop moving right as the map has ended
	// we subtract 240 as that is the width of the game camera
	// we do not need to define the left flag as that is 0
	// and Mario does not move the camera left anyway
	f_EndMapFlagRight = Int2Fix((Level1CollisionWidth << 3) - 240);

	// Copying data for palette and Mario tiles
	dma_word_cpy(pal_sp_mem, Mario_SheetPal, Mario_SheetPalLen);
	dma_word_cpy(&tile_mem[4][u16_TileMemPos], Mario_SheetTiles, Mario_SheetTilesSpriteLen);

	// Setting OAM
	this->o_Sprite.o_Attribute = &MEM_OAM[127];

	// And attributes
	o_Sprite.o_Attribute->u16_attr0 = SetAttrib0(Fix2Int(o_Pos.f_y), 0, 0, 0, A0_4BPP, A0_SQUARE);
	o_Sprite.o_Attribute->u16_attr1 = SetAttrib1(Fix2Int(o_Pos.f_x), 0, A1_SIZE_1);
	o_Sprite.o_Attribute->u16_attr2 = SetAttrib2(u16_TileMemPos, 0, 0);
	
	// Copying the coun particle palette and tile data into memory
	dma_word_cpy(pal_sp_memblock[4], BigCoinsPal, BigCoinsPalLen);
	dma_word_cpy(&tile_mem[4][u16_TileMemPos + 16], BigCoinsTiles, BigCoinsTilesLen);

	// Initialising Mario's particles
	for (u32 u32_i = 0; u32_i < 5; u32_i++)
	{
		InitParticle(&o_Particles[u32_i]);

		o_Particles[u32_i].o_Sprite.o_Attribute = &MEM_OAM[117 - u32_i];

		o_Particles[u32_i].o_Sprite.o_Attribute->u16_attr0 = SetAttrib0(0, 0, 0, 0, A0_4BPP, A0_SQUARE);
		o_Particles[u32_i].o_Sprite.o_Attribute->u16_attr1 = SetAttrib1(0, 0, A1_SIZE_1);
		o_Particles[u32_i].o_Sprite.o_Attribute->u16_attr2 = SetAttrib2(u16_TileMemPos + 16, 0, 4);
		obj_hide(o_Particles[u32_i].o_Sprite.o_Attribute);
	}

	// Set to true
	u8_MoveDown		=	1;
	u8_MoveUp		=	1;
	u8_MoveRight	=	1;
	u8_MoveLeft		=	1;

	// Set Mario's state so when we restart a game after dying we don't instantly die again
	this->eo_State = idle;
}

void c_Mario::Update()
{
	// If Mario drops bellow the screen kill zone then kill him
	if (o_Pos.f_y > f_yKillZone)
	{
		this->eo_State = deathAnim;
	}

	// If Mario isn't dead then process his movement, collision, etc.
	// Else skip the animation phase where we can animation his death
	if (this->eo_State != deathAnim && this->eo_State != dead)
	{
		////////////////////////////////////////
		// Poll input
		PollKeys();

		////////////////////////////////////////
		// Get controls
		s32 s32_xAxis = GetAxis(HORIZONTAL);

		fixed f_xSpeedIncriment;

		////////////////////////////////////////
		// Set to true so that when we test collision
		// we don't need to do any processing on
		// directions that we can move in
		u8_MoveDown = 1;
		u8_MoveUp = 1;
		u8_MoveRight = 1;
		u8_MoveLeft = 1;

		////////////////////////////////////////
		// Move the player based on velocity
		o_Pos.f_y = FixAdd(o_Pos.f_y, o_Vel.f_y);

		////////////////////////////////////////
		// Manage X velocity
		// If we have a horizontal input
		if (s32_xAxis != 0)
		{
			// Increase velocity by 1/20
			f_xSpeedIncriment = FixMul(Int2Fix(s32_xAxis), f_Accel);
			o_Vel.f_x = FixAdd(o_Vel.f_x, f_xSpeedIncriment);
		}
		else
		{
			f_xSpeedIncriment = FixMul(o_Vel.f_x, f_Accel);

			if (o_Vel.f_x < o_MinVel.f_x && o_Vel.f_x > 0) // Moving right
			{
				o_Vel.f_x = 0; // Stop the player if less than minVel
			}
			else if (o_Vel.f_x > -o_MinVel.f_x && o_Vel.f_x < 0) // Moving left
			{
				o_Vel.f_x = 0; // Stop the player if less than minVel
			}
			else
			{
				o_Vel.f_x = FixSub(o_Vel.f_x, f_xSpeedIncriment); // Decrement velocity by xSpeedIncriment
			}
		}

		////////////////////////////////////////
		// Capping velocities
		// Making the velocity equal to the max velocity multiplied
		// by the sign of the current velocity
		// So if out velocity is 10 the sign is 1 so we set our velocity
		// to positive maxVel, if our velocity is -10 we set our
		// velocity to negative maxVel
		if (o_Vel.f_x > o_MaxVel.f_x || o_Vel.f_x < -o_MaxVel.f_x)
		{
			o_Vel.f_x = FixMul(o_MaxVel.f_x, FixSign(o_Vel.f_x));
		}

		if (o_Vel.f_y > o_MaxVel.f_y || o_Vel.f_y < -o_MaxVel.f_y)
		{
			o_Vel.f_y = FixMul(o_MaxVel.f_y, FixSign(o_Vel.f_y));
		}

		////////////////////////////////////////
		// Add gravity to the players Y velocity
		o_Vel.f_y = FixAdd(o_Vel.f_y, f_Grav);

		////////////////////////////////////////
		// Manage Y velocity
		if (KeyHit(A) && eo_State != jumping)
		{
			eo_State = jumping;
			o_Vel.f_y = FixSub(o_Vel.f_y, o_MaxVel.f_y);

			u8 u8_ParticleAmount = QRan_Range(3, 5);

			// Initialise particles
			for (u32 u32_i = 0; u32_i < u8_ParticleAmount; u32_i++)
			{
				InitParticle(&o_Particles[u32_i]);
			}

			// Emit particles
			for (u32 u32_i = 0; u32_i < u8_ParticleAmount; u32_i++)
			{
				obj_unhide(o_Particles[u32_i].o_Sprite.o_Attribute, A0_4BPP);
				EmitParticle(this->o_Pos, &o_Particles[u32_i]);
			}
		}

		////////////////////////////////////////
		// Testing collision
		testCollision(o_Vel.f_x, o_Vel.f_y);

		////////////////////////////////////////
		// Sprite movement, directions, and states
		switch (Fix2Int(FixSign(o_Vel.f_x)))
		{
			// Moving Right
		case 1:
		{
			// If we can't move right
			if (u8_MoveRight == 0)
			{
				// Set our animation state to idle
				eo_State = idle;
				// Set out velocity to 0
				o_Vel.f_x = 0;
				// Break out of the case
				break;
			}

			// Moving player
			// If we are at the right edge of the screen
			if (Fix2Int(o_Pos.f_x + o_Vel.f_x) > u8_RightBorder)
			{
				// If we are at the end of the map
				if (o_WorldPos.f_x > f_EndMapFlagRight)
				{
					// Break out of the case
					break;
				}
				// Move the camera/viewport by the player velocity
				o_WorldPos.f_x = FixAdd(o_WorldPos.f_x, o_Vel.f_x);
			}
			// If we are not at the right edge of the screen
			else
			{
				// move the player by the player velocity
				o_Pos.f_x = FixAdd(o_Pos.f_x, o_Vel.f_x);
			}

			// If our veloctiy is so low that we need to stop moving
			if (o_Vel.f_x < o_MinVel.f_x)
			{
				// Set the state to idle
				eo_State = idle;
			}

			// Flip the sprite horizontally to the right
			o_Sprite.o_Attribute->u16_attr1 &= 0xefff;

			// If we are not in the middle of a jump
			if (eo_State != jumping)
				eo_State = walking;
			break;
		}
		// Moving left
		case -1:
		{
			// If we can't move left
			if (u8_MoveLeft == 0)
			{
				// Set the state to idle
				eo_State = idle;
				// Set the velocity to 0
				o_Vel.f_x = 0;
				// Break out of the case
				break;
			}

			// Moving player
			// If we are at the left edge of the screen
			if (Fix2Int(o_Pos.f_x + o_Vel.f_x) < u8_LeftBorder)
			{
				// If we are at the left end of the map which is 0
				// 8 is 1 in fixed, saves converting every update
				if (o_WorldPos.f_x < 8)
				{
					// Break out of the case
					break;
				}
				// Move the camera/viewport by the player velocity
				o_WorldPos.f_x = FixAdd(o_WorldPos.f_x, o_Vel.f_x);
			}
			else
			{
				o_Pos.f_x = FixAdd(o_Pos.f_x, o_Vel.f_x);
			}

			if (o_Vel.f_x > -o_MinVel.f_x)
			{
				eo_State = idle;
			}

			// Animation
			o_Sprite.o_Attribute->u16_attr1 |= 0x1000;

			if (eo_State != jumping)
				eo_State = walking;
			break;
		}
		// Not moving
		case 0:
		{
			// If mario is dead then break
			if (this->eo_State == dead)
			{
				break;
			}

			// If the player is not jumping then they need to be idling
			if (eo_State != jumping)
				eo_State = idle;
			break;
		}
		}
	} // End of the death if statement

	////////////////////////////////////////
	// Aniamtion sprites states
	switch (eo_State)
	{
	case idle:
	{
		dma_word_cpy(&tile_mem[4][u16_TileMemPos], Mario_SheetTiles, Mario_SheetTilesSpriteLen);
		u8_AnimFrame = 0;
		break;
	}
	case walking:
	{
		// If we just started walking or we have just changed our sprite animation frame
		if (u8_WalkStart == 0)
		{
			// Start a delay
			Delay01(TM_MILLISECOND);
			u8_WalkStart = 1;
		}
	
		// If the delay has reached the threshhold
		if (TM1D > 150)
		{
			// Restart the delay
			u8_WalkStart = 0;
			// Increase the frame that we're on
			u8_AnimFrame++;
			// Clamp and wraparound the sprite animation frame so that we just have the running animation and not the idle,
			// jumping, or dead animation frames
			if (u8_AnimFrame > 3)
			{
				u8_AnimFrame = 1;
			}
			// Copy the appropriate frame from obj tile mem to the OAM object
			dma_word_cpy(&tile_mem[4][u16_TileMemPos], &Mario_SheetTiles[u8_AnimFrame * Mario_SheetTilesSpriteLenHalf], Mario_SheetTilesSpriteLen);
			// Stop the delay, this will also clear the timer allowing us to restart it
			StopDelay01();
		}
	
		break;
	}
	case jumping:
	{
		u8_AnimFrame = 5;
		dma_word_cpy(&tile_mem[4][u16_TileMemPos], &Mario_SheetTiles[u8_AnimFrame * Mario_SheetTilesSpriteLenHalf], Mario_SheetTilesSpriteLen);
		break;
	}
	// The death animation
	case deathAnim:
	{
		// Set the animation frame to be dead mario
		u8_AnimFrame = 6;
		dma_word_cpy(&tile_mem[4][u16_TileMemPos], &Mario_SheetTiles[u8_AnimFrame * Mario_SheetTilesSpriteLenHalf], Mario_SheetTilesSpriteLen);

		// Create a cutoff for the death animation, when he gets below this points we will exit to the main menu
		fixed u8_AnimationCutoff = Int2Fix(160);

		// Give him a velocity so he will bounce up a little and then fall down
		o_Vel.f_y = Int2Fix(-3);

		while (o_Pos.f_y < u8_AnimationCutoff)
		{
			// Add gravity to the players Y velocity
			o_Vel.f_y = FixAdd(o_Vel.f_y, f_Grav);

			// Move the player based on veloctity
			o_Pos.f_y = FixAdd(o_Pos.f_y, o_Vel.f_y);

			// Update the position
			SetObjectPosition(o_Sprite.o_Attribute, Fix2Int(o_Pos.f_x), Fix2Int(o_Pos.f_y));

			Vsync();
		}

		// Wait for 1 second
		Sleep(1, TM_SECOND);

		// Change our state to dead
		this->eo_State = dead;

		break;
	}
	case dead:
	{
		// If Mario ever ends up in this state the game should quit back to the main menu
		break;
	}
	}

	// Update Mario's particles
	for (u32 u32_i = 0; u32_i < 5; u32_i++)
	{
		// If the particle is active update it
		if (o_Particles[u32_i].u8_IsActive == 1)
		{
			UpdateParticle(&o_Particles[u32_i]);
		}
	}

	////////////////////////////////////////
	// Setting positions of the camera/viewport and player
	REG_BG_OFS[0].s16_x = Fix2Int(o_WorldPos.f_x);
	SetObjectPosition(o_Sprite.o_Attribute, Fix2Int(o_Pos.f_x), Fix2Int(o_Pos.f_y));
}

void c_Mario::testCollision(fixed a_f_xDir, fixed a_f_yDir)
{
	// Getting the signs of the x and y velocity so we can see what directions
	// we need to check the collision
	s8 s8_CheckX = Fix2Int(FixSign(a_f_xDir));
	s8 s8_CheckY = Fix2Int(FixSign(a_f_yDir));

	// Convert the players map position in fixed format and convert it to tile format
	s32 s32_PlayerTileX = Fix2Int(FixAdd(FixAdd(o_Pos.f_x, o_WorldPos.f_x), a_f_xDir)) >> 3;
	s32 s32_PlayerTileY = Fix2Int(FixAdd(FixAdd(o_Pos.f_y, o_WorldPos.f_y), a_f_yDir)) >> 3;

	// Make the tile position wrap around so that if the player comes off the map
	// or if something bad happens the collision will still stay inside the collision
	// map
	while (s32_PlayerTileX < 0)
		s32_PlayerTileX += Level1CollisionWidth;
	while (s32_PlayerTileX >= Level1CollisionWidth)
		s32_PlayerTileX -= Level1CollisionWidth;

	while (s32_PlayerTileY < 0)
		s32_PlayerTileY += Level1CollisionHeight;
	while (s32_PlayerTileY >= Level1CollisionHeight)
		s32_PlayerTileY -= Level1CollisionHeight;

	// Scale the y value with the collision map width
	s32_PlayerTileY *= Level1CollisionWidth;

	/*
	This image represents the tiles surrounding mario
	and the variable names for those tiles

	The tiles are made up of 8x8 squares.
	the origin (top left) is marked with an o and the
	corners are marked with x

		|	|	| 
	----o-------x----
		|TL	| TR| RRT
	-----------------
		|BL	|	| RRB
	----x-------x----
		|BBL| BBR|
	*/

	// Check vertical collision
	switch (s8_CheckY)
	{
	// Up
	case -1:
	{
		// Top Sections
		u8 u8_TL = Level1Collision[s32_PlayerTileX + s32_PlayerTileY];
		u8 u8_TR = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + 1];

		if (u8_TL != 0 || u8_TR != 0)
			o_Vel.f_y = 0;
		u8_MoveUp = 0;

		break;
	}
	// Down
	case 1:
	{
		// Bottom Sections
		u8 u8_BBL = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + (Level1CollisionWidth * 2)];
		u8 u8_BBR = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + (Level1CollisionWidth * 2) + 1];

		if (u8_BBL != 0 || u8_BBR != 0)
		{
			eo_State = idle;
			u8_MoveDown = 0;
			if (o_Vel.f_y > 0)
				o_Vel.f_y = 0;
		}


		break;
	}
	}

	// Check horizontal collision
	switch (s8_CheckX)
	{
	case -1:
	{
		// Left sections
		u8 u8_TL = Level1Collision[s32_PlayerTileX + s32_PlayerTileY];
		u8 u8_BL = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + Level1CollisionWidth];

		if (u8_TL != 0 || u8_BL != 0)
			u8_MoveLeft = 0;

		break;
	}
	case 1:
	{
		// Right sections
		u8 u8_RRT = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + 2];
		u8 u8_RRB = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + Level1CollisionWidth + 2];

		if (u8_RRT != 0 || u8_RRB != 0)
			u8_MoveRight = 0;

		break;
	}
	}

	// If we collide on the x axis then we need to recheck the y axis collision
	// as if we don't we could miss collision and clip inside objects
	if (u8_MoveRight == 0 || u8_MoveLeft == 0)
	{
		// Check vertical collision
		switch (s8_CheckY)
		{
		case -1:
		{
			// Top Sections
			u8 u8_TL = Level1Collision[s32_PlayerTileX + s32_PlayerTileY];
			u8 u8_TR = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + 1];

			if (u8_TL != 0 || u8_TR != 0)
				o_Vel.f_y = 0;
			u8_MoveUp = 0;

			break;
		}
		case 1:
		{
			// Bottom Sections
			u8 u8_BBL = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + (Level1CollisionWidth * 2)];
			u8 u8_BBR = Level1Collision[s32_PlayerTileX + s32_PlayerTileY + (Level1CollisionWidth * 2) + 1];

			if (u8_BBL != 0 || u8_BBR != 0)
			{
				eo_State = idle;
				u8_MoveDown = 0;
				if (o_Vel.f_y > 0)
					o_Vel.f_y = 0;
			}


			break;
		}
		}
	}
}