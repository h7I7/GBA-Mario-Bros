////////////////////////////////////////////////////////////
//	File Name:		gba_timers.c
//	Description:	A header file for the timer defines and
//					functions
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "gba_timers.h"

// Enable a timer control
void Timer_Enable(u16 a_u16_tmrCtrl)
{
	a_u16_tmrCtrl |= TM_ENABLE;
}

// Disable a timer control
void Timer_Disable(u16 a_u16_tmrCtrl)
{
	a_u16_tmrCtrl ^= TM_ENABLE;
}

// Pause a timer without losing its current state
void Timer_Pause(u16 a_u16_tmrCtrl)
{
	a_u16_tmrCtrl |= TM_CASCADE;
}

// Unpause a timer
void Timer_UnPause(u16 a_u16_tmrCtrl)
{
	a_u16_tmrCtrl ^= TM_CASCADE;
}

// Start timer 0 cascading into timer 1 with a timescale
// This allows us to semi-accurately time things in seconds
// or milliseconds (whatever timescale we have the patience
// to figure out)
// Compared to the Sleep function this does not pause the
// application

// To count this timer you would use if tm1d == whatever time
// you want

// This function works at timer 1 is set to cascading mode meaning
// that whenever the timer before it, in this case timer 0, overflows
// timer 1 will be incrimented by 1.
// If we set timer 0 to the timescale and frequency that we want we can
// make timer1 tick at any rate we please.
// I have adde defines for seconds and milliseconds for convenience
void Delay01(u16 a_timeScale)
{
	TM0D = 0; TM1D = 0;
	TM0C = 0; TM1C = 0;

	TM0D = a_timeScale;
	TM0C = TM_ENABLE | TM_FREQ1024;

	TM1C = TM_ENABLE | TM_CASCADE;
}

// Stop the timers 0 and 1 used for the first delay set
void StopDelay01()
{
	Timer_Disable(TM0C);
	Timer_Disable(TM1C);
}

// Another delay function using timers 2 and 3
void Delay23(u16 a_timeScale)
{
	TM2D = 0; TM3D = 0;
	TM2C = 0; TM3C = 0;
	
	TM2D = a_timeScale;
	TM2C = TM_ENABLE | TM_FREQ1024;

	TM3C = TM_ENABLE | TM_CASCADE;
}

// Stop timers 2 and 3
void StopDelay23()
{
	Timer_Disable(TM2C);
	Timer_Disable(TM3C);
}

// The sleep function is identical to the Delay01 and Delay23 functions
// except this function pauses the application until the timer reaches
// it's goal time
void Sleep(u32 a_amount, u16 a_timeScale)
{
	TM2D = 0; TM3D = 0;
	TM2C = 0; TM3C = 0;

	TM2D = a_timeScale;
	TM2C = TM_ENABLE | TM_FREQ1024;

	TM3C = TM_ENABLE | TM_CASCADE;

	while (1)
	{
		if (TM3D > a_amount)
			return;
	}
}