#include "gba_mathUtil.h"
#include "gba_Particles.h"

// Initialise all the particle member variables
void InitParticle(s_Particle* a_o_P)
{
	a_o_P->o_Pos.f_x = 0; a_o_P->o_Pos.f_y = 0; a_o_P->o_Vel.f_x = 0; a_o_P->o_Vel.f_y = 0;
	a_o_P->u32_Life = 0;
	a_o_P->u8_AnimFrame = 0;
	a_o_P->u8_IsActive = 0;
}

// When a particle is emitted 
void EmitParticle(s_Vector2 a_o_Origin, s_Particle* a_o_p_P)
{
	// Chane the particle position to that of the origin
	a_o_p_P->o_Pos.f_x = a_o_Origin.f_x;
	a_o_p_P->o_Pos.f_y = a_o_Origin.f_y;

	// Initialise the velocity so that it will start moving up and out from the origin
	a_o_p_P->o_Vel.f_x = Int2Fix(QRan_Range(-5, 5));
	a_o_p_P->o_Vel.f_y = Int2Fix(-14 + QRan_Range(0,6));
	
	// Set life to 0, this is used for animation control
	a_o_p_P->u32_Life = 0;
	// Set the particle as active
	a_o_p_P->u8_IsActive = 1;
}

// For updating particles
void UpdateParticle(s_Particle* a_o_p_P)
{
	// Set the position based off the velocity
	a_o_p_P->o_Pos.f_x = FixAdd(a_o_p_P->o_Pos.f_x, a_o_p_P->o_Vel.f_x);
	a_o_p_P->o_Pos.f_y = FixAdd(a_o_p_P->o_Pos.f_y, a_o_p_P->o_Vel.f_y);

	// Add gravity to the particles velocity
	a_o_p_P->o_Vel.f_y = FixAdd(a_o_p_P->o_Vel.f_y, PARTICLEGRAVITY);

	// Set the position of the sprite
	SetObjectPosition(a_o_p_P->o_Sprite.o_Attribute, a_o_p_P->o_Pos.f_x >> 8, a_o_p_P->o_Pos.f_y >> 8);

	// Check if the particle has fallen to the bottom of the screen
	// If it has deactivate it
	// 38912 is 16 pixels from the bottom of the screen in fixed point
	if (a_o_p_P->o_Pos.f_y > 38912)
	{
		obj_hide(a_o_p_P->o_Sprite.o_Attribute);
		a_o_p_P->u8_IsActive = 0;
	}

	// Increase the particles life by 1
	a_o_p_P->u32_Life += 1;

	// If the life has been incrimented 16 times then change the sprite to the next animation frame
	if ((a_o_p_P->u32_Life & 0x1F) == 15)
	{
		// Incriment the animation frame
		a_o_p_P->u8_AnimFrame++;
		// Clamp the value in between 0 and 3
		a_o_p_P->u8_AnimFrame &= 3;
		// Set the new tile index to the next frame of animation, this may look a bit like floating numbers but
		// I will always copy the sprites to sprite tile mem position 976 which is 16 tile down from Mario
		// I do this so if I create sprites with the sprite manager it won't have any overwriting issues
		SetObjectTileIndex(a_o_p_P->o_Sprite.o_Attribute, 976 + (a_o_p_P->u8_AnimFrame * 4));
	}
}