////////////////////////////////////////////////////////////
//	File Name:		gba_gfx.h
//	Description:	A header file for all the rendering and
//					screen functionality
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef __GBA_GFX_H__
#define __GBA_GFX_H__
#include "Intellisense.h"

// This forces the compiler to regognise the following code as C
#ifdef __cplusplus
extern "C" {
#endif

#include "gba_reg.h"
#include "gba_types.h"
#include "gba_math.h"
//====================================================================================
// Video and Rendering
//====================================================================================

//vcount is used for testing for vertical blank
#define REG_VCOUNT (*(vu16*)(REG_BASE + 0x06))

extern void Vsync();
//=======================================================================
//Functionality for setting up page flipping in bitmap rendering modes
#define VIDEO_PAGE			0x0010	//!< Page indicator

// The video page size
#define VRAM_PAGE_SIZE		0x0A000
// Back page
#define VRAM_BACK	(VRAM+ VRAM_PAGE_SIZE)

// The screen
#define vid_mem			((u16*)VRAM)
// Front page
#define vid_page_front	((u16*)VRAM)
// Back page
#define vid_page_back	((u16*)VRAM_BACK)

//=======================================================================
// Pointer to the current page
extern u16* u16_p_vid_page;
// Flip the video page
extern u16* Page_Flip();

//=======================================================================
//Control Register defines for background tiled rendering
#define ENABLE_OBJECTS 0x1000
#define MAPPINGMODE_1D 0x0040

//base address pointer for base register
#define REG_DISPLAYCONTROL *((vu32*)(REG_BASE))

//====================================================================================
// Background and Video modes
//====================================================================================

// Defines for settng up different video modes
#define VIDEOMODE_0 0x0000  // Sprite Mode 0
#define VIDEOMODE_1 0x0001  // Sprite Mode 1
#define VIDEOMODE_2 0x0002  // Sprite Mode 2
#define VIDEOMODE_3 0x0003	// < Mode 3; 240x160 @ 16bpp
#define VIDEOMODE_4	0x0004	// < Mode 4; 240x160 @ 8 bpp
#define VIDEOMODE_5 0x0005  // < Mode 5; 

// Defines for enabling different backgrounds.
// Different background modes give differing
// amounts of regular layers and affine layers.
#define BGMODE_0	0x0100
#define BGMODE_1	0x0200
#define BGMODE_2	0x0400
#define BGMODE_3	0x0800

//define a base registry address for the background controllers
//accessing this as REG_BGCNT[0] is controller 0
//accessing this as REG_BGCNT[1] is controller 1 or memory address REG_BGCNT + (sizeof(vu16) * 1) = 0x0400:000A
//accessing this as REG_BGCNT[2] is controller 2 or memory address REG_BGCNT + (sizeof(vu16) * 2) = 0x0400:000C
//accessing this as REG_BGCNT[3] is controller 3 or memory address REG_BGCNT + (sizeof(vu16) * 3) = 0x0400:000E
#define REG_BGCNT ((vu16*)(REG_BASE+0x0008))

//structure to control background offset x & y flags
typedef struct s_BackgroundOffsets
{
	s16 s16_x, s16_y;
}__attribute__((packed, aligned(4))) s_BGOffset;

//set up define for REG_BG_OFS accessed as an array to access offset data for bg 0-3 
//REG_BG_OFS[1].x accesses x .y accesses y value
#define REG_BG_OFS  ((s_BGOffset*)(REG_BASE+0x0010))

#define TILE_BLOCK_SIZE 0x4000
#define TILEMAP_BLOCK_SIZE 0x0800

#define BGCNT_PRIORITY_MASK 0x3
#define BGCNT_TBID_MASK		0x3
#define BGCNT_MOS_MASK		0x1
#define BGCNT_COLOR_MASK	0x1
#define BGCNT_TMID_MASK		0x1F
#define BGCNT_AFW_MASK		0x1
#define BGCNT_SIZE_MASK		0x3

#define BG_REG_SIZE_32x32	0x0
#define BG_REG_SIZE_64x32	0x1
#define BG_REG_SIZE_32x64	0x2
#define BG_REG_SIZE_64x64	0x3

#define BG_REG_AFF_16x16	0x0
#define BG_REG_AFF_32x32	0x1
#define BG_REG_AFF_64x64	0x2
#define BG_REG_AFF_128x128	0x3

// Function to return the memory address (pointer) to one of the four tile block locations (0-3)
extern u16* Tile_Block_Address(u32 a_u32_blockNum);
// Function to return a pointer to address space for the tile map blocks location (0-31)
extern u16* TileMap_Block_Address(u32 a_u32_blockNum);
// Returns the tile block address of a sprite
extern u16* Sprite_Tile_Block_Address(u32 a_u32_TileNumber);

// Function to set a background control register's values
extern u16 SetBG_Control_Register(u8 a_u8_priority, u8 a_u8_tileBlockID, u8 a_u8_mosaic, u8 a_u8_colourMode, u8 a_u8_mapBlockID, u8 a_u8_affineWrap, u8 a_u8_bgSize);

extern void LoadLevel1();

#include "gba_vector2.h"
extern void UpdateMap(s_Vector2 a_o_MapPos, s_Vector2 a_o_PlayerVel);

//====================================================================================
// defines for palette locations
//====================================================================================
// --- memmap ---
#define MEM_PALETTE		0x05000000
#define pal_bg_mem		((u16*)MEM_PALETTE)
#define pal_sp_mem		((u16*)(MEM_PALETTE + 0x200))

typedef struct s_PalBlock { u8 u8_data; }__attribute__((packed, aligned(4)))s_PalBlock;
typedef s_PalBlock palBlockAccess[8];

#define pal_sp_memblock ((palBlockAccess*)pal_sp_mem)


//====================================================================================
// defines and functionality for Sprites & OAM
//====================================================================================
//Defines and structures to map tiles in to memory blocks
typedef struct s_TILE { u32 u32_data[8]; }__attribute__((packed, aligned(4)))s_TILE, s_TILE4;
typedef struct s_TILE8 { u32 u32_data[16]; }__attribute__((packed, aligned(4)))s_TILE8;

// Typedefs and defines for accessing Tile Mem in different sized chunks
// You can access Tile Mem in 4bit or 8bit chunks
typedef s_TILE TILEBLOCK[512];
typedef s_TILE8 TILEBLOCK8[256];

#define tile_mem ((TILEBLOCK*)0x06000000)
#define tile8_mem ((TILEBLOCK8*)0x06000000)

// Enable OAM objects
#define ENABLE_OBJECTS 0x1000
// Change sprite tile mem to map in 1 dimension
#define MAPPINGMODE_1D 0x0040

// Container for sprite attribute data
typedef struct s_ObjectAttribute {
	u16 u16_attr0;
	u16 u16_attr1;
	u16 u16_attr2;
	s16 s16_pad;
}__attribute__((packed, aligned(4)))s_ObjectAttribute;

// Container for sprite affine data
typedef struct s_ObjectAffine {
	u16 u16_fill0[3];
	s16 s16_pa;
	u16 u16_fill1[3];
	s16 s16_pb;
	u16 u16_fill2[3];
	s16 s16_pc;
	u16 u16_fill3[3];
	s16 s16_pd;

}__attribute__((packed, aligned(4)))s_ObjectAffine;

// Setting up accessing OAM by index
extern s_ObjectAttribute o_Obj_Buffer[128];
extern s_ObjectAffine *const cnst_o_p_Obj_Aff_Buffer;

#define MEM_OAM ((s_ObjectAttribute*) 0x07000000)
#define MEM_AFF_OAM ((s_ObjectAffine*) 0x07000000)

// Attribute defines
#define A0_YMASK 0xFF
#define A0_MODE_MASK 0xFCFF

//defines for Object mode - affine or regular, or double size affine
#define A0_MODE_REG		0x00
#define A0_MODE_AFF		0x01
#define A0_MODE_HIDE	0x02
#define A0_MODE_AFF_DBL 0x03

//colour mode defines
#define A0_4BPP 0x0
#define A0_8BPP 0x1

//sprite shape
#define A0_SQUARE 0x00
#define A0_WIDE   0x01
#define A0_TALL   0x02

//x position mask
#define A1_XMASK 0x1FF
#define A1_H_FLIP 0x01
#define A1_V_FLIP 0x02
#define A1_FLIPMASK 0x3
#define A1_SIZE_0 0x00
#define A1_SIZE_1 0x01
#define A1_SIZE_2 0x02
#define A1_SIZE_3 0x03
#define A1_AFF_INDEX_MASK 0x1F

// Functions for setting attributes and sprite settings
extern u16 SetAttrib0(u8 a_u8_y, u8 a_u8_objectMode, u8 a_u8_gfxMode, u8 a_u8_mosiac, u8 a_u8_colorMode, u8 a_u8_shape);
extern u16 SetAttrib1(u8 a_u8_x, u8 a_u8_flip, u8 a_u8_size);
extern void SetAttrib1AffineIndex(s_ObjectAttribute* a_o_p_object, u8 a_u8_index);
extern u16 SetAttrib2(u16 a_u16_tileIndex, u8 a_u8_priority, u8 a_u8_paletteBank);
extern void SetObjectPosition(s_ObjectAttribute* a_o_p_object, u8 a_u8_x, u8 a_u8_y);
extern void SetObjectPaletteBank(s_ObjectAttribute* a_o_p_object, u8 a_u8_paletteBank);
extern void SetObjectTileIndex(s_ObjectAttribute* a_o_p_object, u16 a_u16_tileIndex);

// Gets an object position from it's attribute data
extern s_Vector2 GetObjectPosition(s_ObjectAttribute* a_o_p_obj);

// Hide an object
extern void obj_hide(s_ObjectAttribute* a_o_p_obj);

// Unhide an object
extern void obj_unhide(s_ObjectAttribute* a_o_p_obj, u8 a_u8_mode);

// Initialise Object attribute memory
extern void oam_init(s_ObjectAttribute* a_o_p_obj, u8 a_u8_count);
extern void oam_copy(s_ObjectAttribute* a_o_p_dst, s_ObjectAttribute* a_o_p_src, u8 a_u8_count);

// Affine sprite manipulation
extern void obj_affine_identity(s_ObjectAffine* a_o_p_obj);
extern void obj_affine_rotscale(s_ObjectAffine* a_o_p_obj, fixed a_f_sx, fixed a_f_sy, u16 a_u16_rot);
extern void obj_affine_rot(s_ObjectAffine* a_o_p_obj, u16 a_u16_rot);

#ifdef __cplusplus
}
#endif

#endif //__GBA_GFX_H__