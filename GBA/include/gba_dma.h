////////////////////////////////////////////////////////////
//	File Name:		gba_dma.h
//	Description:	A header file for all the direct memory access
//					defines and memory copying routines
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef _GBA_DMA_H_
#define _GBA_DMA_H_

#include "gba_types.h"

// Define a register for DMA
// Takes source destination and a count for the number of things to copy
typedef struct s_DMA_REC
{
	const void* cnst_p_src;
	void* p_dst;
	u32 u32_cnt;
}s_DMA_REC;

// Get a pointer to the DMA memory location
// access this as an array to get addresses 0 - 3
#define REG_DMA ((volatile s_DMA_REC*)0x040000B0)

// Define our DMA control register variables
////////////////////////////////////////
// How we go through the destination
// Incriment from a value
#define DMA_DST_INC		(0x00 << 21)
// Decriment from a value
#define DMA_DST_DEC		(0x01 << 21)
// Fixed, destination doesn't change
#define DMA_DST_FIXED	(0x02 << 21)
// Once we're done, reload the destinaion pointer to where it was before the copy
#define DMA_DST_RELOAD	(0x03 << 21)

////////////////////////////////////////
// How we go through the sources
// Incriment 
#define DMA_SRC_INC		(0x00 << 23)
// Decriment 
#define DMA_SRC_DEC		(0x01 << 23)
// Fixed
#define DMA_SRC_FIXED	(0x02 << 23)

////////////////////////////////////////
// Repeat the copying
#define DMA_REPEAT		(0x01 << 25)

////////////////////////////////////////
// Sizes of the blocks we want to copy
// 16 bits
#define DMA_16			(0x00 << 26)
// 32 bits
#define DMA_32			(0x01 << 26)

////////////////////////////////////////
// When to copy
// Copy now
#define DMA_AT_NOW		(0x00 << 28)
// Copy at VBlank
#define DMA_AT_VBLANK	(0x01 << 28)
// Copy at HBlank
#define DMA_AT_HBLANK	(0x02 << 28)
// Copy at refresh
#define DMA_AT_REFRESH	(0x03 << 28)

////////////////////////////////////////
// Enable interrupts
#define DMA_IRQ			(0x01 << 30)

////////////////////////////////////////
// Enable, set this flag to use DMA
#define DMA_ENABLE		(0x01 << 31)

////////////////////////////////////////
// Some macro defines
// Start copying now
#define DMA_NOW			(DMA_ENABLE | DMA_AT_NOW)
// Copy 16 bit now
#define DMA_16NOW		(DMA_NOW | DMA_16)
// COpy 32 bit now
#define DMA_32NOW		(DMA_NOW | DMA_32)

////////////////////////////////////////
// DMA copy flags
// Copy 16 bit
#define DMA_CPY16		DMA_16NOW
//COpy 32 bit
#define DMA_CPY32		DMA_32NOW

////////////////////////////////////////
// DMA fill flags
// Fill 16 bit
#define DMA_FILL16		(DMA_NOW | DMA_SRC_FIXED | DMA_16)
// Copy 32 bit
#define DMA_FILL32		(DMA_NOW | DMA_SRC_FIXED | DMA_32)

////////////////////////////////////////
// DMA transfer, copy and fill routines
// General DMA transfer
#define DMA_TRANSFER(_dst, _src, _count, _ch, _mode)\
do{													\
	REG_DMA[_ch].cnt = 0;							\
	REG_DMA[_ch].src = (const void*)(_src);			\
	REG_DMA[_ch].dst = (void*)(_dst);				\
	REG_DMA[_ch].cnt = _count | _mode;				\
}while(0)

// General DMA copier
INLINE void dma_cpy(void* a_p_dst, const void* a_cnst_p_src, u32 a_u32_count, u32 a_u32_mode)
{
	REG_DMA[3].u32_cnt = 0;
	REG_DMA[3].cnst_p_src = a_cnst_p_src;
	REG_DMA[3].p_dst = a_p_dst;
	REG_DMA[3].u32_cnt = a_u32_count | a_u32_mode;
}

// General DMA fill
INLINE void dma_fill(void* a_p_dst, volatile u32 a_v_u32_src, u32 a_u32_count, u32 a_u32_mode)
{
	REG_DMA[3].u32_cnt = 0;
	REG_DMA[3].cnst_p_src = (const void*)&a_v_u32_src;
	REG_DMA[3].p_dst = a_p_dst;
	REG_DMA[3].u32_cnt = a_u32_count | a_u32_mode | DMA_SRC_FIXED;
}

// Word copy using DMA 3
INLINE void dma_word_cpy(void* a_p_dst, const void* a_c_p_src, u32 a_u32_size)
{
	dma_cpy(a_p_dst, a_c_p_src, a_u32_size >> 2, DMA_CPY32);
}

// Word fill using DMA 3
INLINE void dma_word_fill(void* a_p_dst, volatile u32 a_v_u32_src, u32 a_u32_size)
{
	dma_fill(a_p_dst, a_v_u32_src, a_u32_size >> 2, DMA_FILL32);
}

// HBlank triggered DMA define
#define DMA_HDMA	(DMA_ENABLE | DMA_REPEAT | DMA_AT_HBLANK | DMA_DST_FIXED)

#endif // _GBA_DMA_H_