////////////////////////////////////////////////////////////
//	File Name:		gba_Particles.h
//	Description:	A header file for all the particles structs
//					and functionality
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef _GBA_PARTICLES_H_
#define _GBA_PARTICLES_H_

#include "gba_vector2.h"
#include "sprite.h"

#ifdef __cplusplus
extern "C" {
#endif

// 1 Bitshifted 8 times to the left
// The same as 1 to the power of 8
#define PARTICLEGRAVITY 256

//Particle struct
typedef struct s_Particle
{
	s_Sprite	o_Sprite;
	s_Vector2	o_Pos;
	s_Vector2	o_Vel;
	// Used for animation control
	u32			u32_Life;
	// Along with this variable which stores which frame of animation we are on
	u8			u8_AnimFrame;
	u8			u8_IsActive;
}__attribute__((aligned(4)))s_Particle;

// Initialising a particle
extern void InitParticle(s_Particle* a_o_P);

// emitting a particle
extern void EmitParticle(s_Vector2 a_o_Origin, s_Particle* a_o_p_P);

// Updating particles
extern void UpdateParticle(s_Particle* a_o_p_P);

#ifdef __cplusplus
}
#endif

#endif //_GBA_PARTICLES_H_