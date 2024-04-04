////////////////////////////////////////////////////////////
//	File Name:		gba_timers.h
//	Description:	A header file for the timer defines and
//					functions
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef _GBA_TIMERS_H_
#define _GBA_TIMERS_H_

// This forces the compiler to regognise the following code as C
#ifdef __cplusplus
extern "C" {
#endif

#include "gba_types.h"

	// Timer data address pointers
#define TM0D			(*(vu16*)(0x04000100))
#define TM1D			(*(vu16*)(0x04000104))
#define TM2D			(*(vu16*)(0x04000108))
#define TM3D			(*(vu16*)(0x0400010C))

// Timer control address pointers
#define TM0C			(*(vu16*)(0x04000102))
#define TM1C			(*(vu16*)(0x04000106))
#define TM2C			(*(vu16*)(0x0400010A))
#define TM3C			(*(vu16*)(0x0400010E))

// Defines for timer frequencies
#define TM_FREQ1		0x0		// 1    cycle/tick
#define TM_FREQ64		0x0001	// 64   cycles/tick
#define TM_FREQ256		0x0002	// 256  cycles/tick
#define TM_FREQ1024		0x0003	// 1024 cycles/tick

// Turn on cascading mode
// Essentially pauses a timer without disabling it
// as it won't overflow in this mode
#define TM_CASCADE		0x0004

// Enabling and disabling timers completely
// If a timer is disabled then upon re-enabling
// it will be reset to 0 or whatever value is set
// to the timer data address
#define TM_ENABLE		0x0080

// Enable timer IRQ
#define TM_IRQ			0x0040

// Timing defines
#define TM_SECOND		(u16)-0x2025	// Roughly 1 second
#define TM_MILLISECOND	(u16)-0x0008	// Very roughly 1 millisecond

// Timer Functions
extern void Timer_Enable(u16 a_u16_tmrCtrl);
extern void Timer_Disable(u16 a_u16_tmrCtrl);
extern void Timer_Pause(u16 a_u16_tmrCtrl);
extern void Timer_UnPause(u16 a_u16_tmrCtrl);

// Delay functions for timing
// Used by calling Delay01/23
// Then check if TM1D/TM3D is larger
// Than the value you want to wait for
// If it is call StopDelay

// Using timers 0 and 1
extern void Delay01(u16 a_u16_timeScale);
extern void StopDelay01();

// Using timers 1 and 2
extern void Delay23(u16 a_u16_timeScale);
extern void StopDelay23();

// Make the application sleep for an amount of time
extern void Sleep(u32 a_u32_amount, u16 a_u32_timeScale);

#ifdef __cplusplus
}
#endif

#endif //_GBA_TIMERS_H_