////////////////////////////////////////////////////////////
//	File Name:		gba_input.h
//	Description:	A header file for the input functionality
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#include "gba_reg.h"
#include "gba_types.h"

#ifndef __GBA_INPUT_H__
#define __GBA_INPUT_H__

// This forces the compiler to regognise the following code as C
#ifdef __cplusplus
extern "C" {
#endif

// Define for Hardware Key Input Register
#define REG_KEYINPUT	*(vu16*)(REG_BASE + 0x130)

// Define for Hardware Key Interrupt Control Register
#define REG_KEYCNT		*(vu16*)(REG_BASE + 0x132)

// Enum for the different keys we can press on the GBA and the values they relate to
typedef enum e_KEYS
{
	A = (1 << 0),
	B = (1 << 1),
	SELECT = (1 << 2),
	START = (1 << 3),
	RIGHT = (1 << 4),
	LEFT = (1 << 5),
	UP = (1 << 6),
	DOWN = (1 << 7),
	R = (1 << 8),
	L = (1 << 9),

	KEYIRQ_ENABLE = (1 << 14),	/*!< Enable keypad interrupt */
	KEYIRQ_OR = (0 << 15),		/*!< interrupt logical OR mode */
	KEYIRQ_AND = (1 << 15),		/*!< interrupt logical AND mode */


}e_KEYS;

#define KEY_MASK 0x03FF
//Vars to hold the current and previous state of the hardware keys
extern u16 u16__currKeys, u16__prevKeys;
//Function to test for Hardware Key Depress
INLINE void PollKeys()
{
	u16__prevKeys = u16__currKeys;
	u16__currKeys = ~REG_KEYINPUT & KEY_MASK;
}

// Input functions
INLINE u16		CurrentKeyState() { return u16__currKeys; }
INLINE u16		PrevKeyState() { return u16__prevKeys; }
INLINE u32		KeyDown(u32 a_u32_key) { return u16__currKeys & a_u32_key; }
INLINE u32		KeyUp(u32 a_u32_key) { return ~u16__currKeys & a_u32_key; }
INLINE u32		KeyHeld(u32 a_u32_key) { return (u16__currKeys & u16__prevKeys) & a_u32_key; }
INLINE u32		KeyReleased(u32 a_u32_key) { return (~u16__currKeys & u16__prevKeys) & a_u32_key; }
INLINE u32		KeyHit(u32 a_u32_key) { return (u16__currKeys & ~u16__prevKeys) & a_u32_key; }
INLINE u32		KeyStateChange(u32 a_u32_key) { return (u16__currKeys ^ u16__prevKeys) & a_u32_key; }

// Enum for input axis left, right up, down
typedef enum e_AXIS
{
	HORIZONTAL = 0,
	VERTICAL,

}e_AXIS;

// Get the current value of an axis, horizontal/vertical
extern s32		GetAxis(e_AXIS a_eo_val);

#ifdef __cplusplus
}
#endif

#endif //__GBA_INPUT_H__