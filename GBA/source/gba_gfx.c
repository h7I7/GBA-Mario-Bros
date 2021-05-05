////////////////////////////////////////////////////////////
//	File Name:		gba_gfx.c
//	Description:	A header file for all the rendering and
//					screen functionality
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "Intellisense.h"
#include "gba_gfx.h"
#include "sinlut.h"
#include "gba_dma.h"

// Container for all the regular sprite attributes
s_ObjectAttribute o_Obj_Buffer[128] = {};
// Container for all the affine sprite attributes
s_ObjectAffine *const cnst_o_p_Obj_Aff_Buffer = (s_ObjectAffine*)o_Obj_Buffer;

void Vsync()
{
	while (REG_VCOUNT >= 160);
	while (REG_VCOUNT < 160);
}

// A pointer to the current video page, by default it is set to the back page
u16* u16_p_vid_page = vid_page_back;

// Flip the video page from the back to the front
u16* Page_Flip()
{
	u16_p_vid_page = (u16*)((u32)u16_p_vid_page ^ VRAM_PAGE_SIZE);
	REG_DISPLAYCONTROL ^= VIDEO_PAGE;	// update control register	

	return u16_p_vid_page;
}

//function to return the memory address (pointer) to one of the four tile block locations (0-3)
u16* Tile_Block_Address(u32 a_u32_blockNum)
{
	//each block is 16KB in size
	return (u16*)(VRAM + (a_u32_blockNum * TILE_BLOCK_SIZE));
}
//function to return a pointer to address space for the tile map blocks location (0-31)
u16* TileMap_Block_Address(u32 a_u32_blockNum)
{
	//each tilemap block is 2KB in size
	return (u16*)(VRAM + (a_u32_blockNum * TILEMAP_BLOCK_SIZE));
}

//Function to set BG Control Register
u16 SetBG_Control_Register(u8 a_u8_priority, u8 a_u8_tileBlockID, u8 a_u8_mosaic, u8 a_u8_colourMode, u8 a_u8_mapBlockID, u8 a_u8_affineWrap, u8 a_u8_bgSize)
{
	vu16 control = (a_u8_priority & BGCNT_PRIORITY_MASK) | ((a_u8_tileBlockID & BGCNT_TBID_MASK) << 2) | ((a_u8_mosaic & BGCNT_MOS_MASK) << 6) | ((a_u8_colourMode & BGCNT_COLOR_MASK) << 7) |
		((a_u8_mapBlockID & BGCNT_TMID_MASK) << 8) | ((a_u8_affineWrap & BGCNT_AFW_MASK) << 13) | ((a_u8_bgSize & BGCNT_SIZE_MASK) << 14);
	return control;
}

// Returns the tile block address of a sprite
u16* Sprite_Tile_Block_Address(u32 a_u32_TileNumber)
{
	// Each block is 16kb in size, sprites are in block 4 of tile memory, the other blocks are used for backgrounds
	return (u16*)(VRAM + (4 * TILE_BLOCK_SIZE) + ((a_u32_TileNumber & 0x3FF) * sizeof(s_TILE)));
}

// Load in the level 1 background data and load the first part of
// the level 1 map, enough to fill the screen
#include "Level1Data.h"
#include "GameTileSet.h"
#include "Menu1.h"
#include "Menu2.h"
void LoadLevel1()
{
	// Load palette
	dma_word_cpy(pal_bg_mem, GameTileSetPal, GameTileSetPalLen);
	// Load tile memory location
	// & with 0x3 so that we cannot access outside of layers 0-3
	u16* u16_p_TileMemLoc = Tile_Block_Address(0);
	// Copy the background data into our tile memory location
	dma_word_cpy(u16_p_TileMemLoc, GameTileSetTiles, GameTileSetTilesLen);

	//Background
	u16* u16_p_TileMapMemLoc = TileMap_Block_Address(17);
	dma_word_cpy(u16_p_TileMapMemLoc, Menu1, Menu1Len);

	u16_p_TileMapMemLoc = TileMap_Block_Address(18);
	dma_word_cpy(u16_p_TileMapMemLoc, Menu1, Menu1Len);

	u16_p_TileMapMemLoc = TileMap_Block_Address(19);
	dma_word_cpy(u16_p_TileMapMemLoc, Menu2, Menu2Len);

	// Set up the background control register
	REG_BGCNT[0] = SetBG_Control_Register(0, 0, 0, 0, 16, 0, BG_REG_SIZE_32x32);
	// Background
	REG_BGCNT[1] = SetBG_Control_Register(1, 0, 0, 0, 17, 0, BG_REG_SIZE_32x32);
	REG_BGCNT[2] = SetBG_Control_Register(2, 0, 0, 0, 18, 0, BG_REG_SIZE_32x32);
	REG_BGCNT[3] = SetBG_Control_Register(3, 0, 0, 0, 19, 0, BG_REG_SIZE_32x32);

	u16* u16_p_TilemapMemLoc = TileMap_Block_Address(16);
	// Draw the screen manually before we start the game as we will stream the rest in
	for (u32 u32_i = 0; u32_i < 32; u32_i++)
	{
		for (u32 u32_jj = 0; u32_jj < 32; u32_jj++)
		{
			u16_p_TilemapMemLoc[(u32_i * 32) + u32_jj] = Level1Map[(u32_i * Level1MapWidth) + u32_jj];
		}
	}
}

// Update the current map based on the camera/viewport position
// this will create the illusion of very large maps, larger than
// 64x64. This example only works on the x axis as the game only
// needs the map on that axis however to extend it into the y axis
// would not be much work
void UpdateMap(s_Vector2 a_mapPos, s_Vector2 a_playerVel)
{
	u16* u16_p_TilemapMemLoc = TileMap_Block_Address(16);

	s8 s8_DirX = Fix2Int(FixSign(a_playerVel.f_x));
	switch (s8_DirX)
	{
	case 1:
	{
		u32 u32_MapOffset1 = (Fix2Int(a_mapPos.f_x) >> 3) + 30;
		u32 u32_MapOffset2 = (Fix2Int(a_mapPos.f_x) >> 3) + 31;

		u32 u32_MemOffset1 = ((Fix2Int(a_mapPos.f_x) >> 3) + 30) & 0x1F;
		u32 u32_MemOffset2 = ((Fix2Int(a_mapPos.f_x) >> 3) + 31) & 0x1F;

		for (u32 u32_i = 0; u32_i < 32; u32_i++)
		{
			u16_p_TilemapMemLoc[(u32_i * 32) + u32_MemOffset1]		= Level1Map[(u32_i * Level1MapWidth) + u32_MapOffset1];
			u16_p_TilemapMemLoc[(u32_i * 32) + u32_MemOffset2]		= Level1Map[(u32_i * Level1MapWidth) + u32_MapOffset2];
		}
		break;
	}
	case -1:
	{
		u32 u32_MapOffset1 = (Fix2Int(a_mapPos.f_x) >> 3);
		u32 u32_MapOffset2 = (Fix2Int(a_mapPos.f_x) >> 3) - 1;

		u32 u32_MemOffset1 = ((Fix2Int(a_mapPos.f_x) >> 3)) & 0x1F;
		u32 u32_MemOffset2 = ((Fix2Int(a_mapPos.f_x) >> 3) - 1) & 0x1F;

		for (u32 u32_i = 0; u32_i < 32; u32_i++)
		{
			u16_p_TilemapMemLoc[(u32_i * 32) + u32_MemOffset1]			= Level1Map[(u32_i * Level1MapWidth) + u32_MapOffset1];
			u16_p_TilemapMemLoc[(u32_i * 32) + u32_MemOffset2]			= Level1Map[(u32_i * Level1MapWidth) + u32_MapOffset2];
		}
		break;
	}
	case 0:
	{
		// If the camera/viewport has not moved then don't process anything
		break;
	}
	}
}

//====================================================================================
//Sprites & OAM
//====================================================================================

//functionality to set attribute 0 properties
u16 SetAttrib0(u8 a_u8_y, u8 a_u8_objectMode, u8 a_u8_gfxMode, u8 a_u8_mosiac, u8 a_u8_colorMode, u8 a_u8_shape)
{
	u16 attrib0 = (a_u8_y & A0_YMASK) | ((a_u8_objectMode & 0x3) << 8) | ((a_u8_gfxMode & 0x3) << 10) |
		((a_u8_mosiac & 0x1) << 12) | ((a_u8_colorMode & 0x1) << 13) | ((a_u8_shape & 0x3) << 14);
	return attrib0;
}
//functionality to set attribute 1 properties
u16 SetAttrib1(u8 a_u8_x, u8 a_u8_flip, u8 a_u8_size)
{
	u16 attrib1 = (a_u8_x & A1_XMASK) | ((a_u8_flip & A1_FLIPMASK) << 12) | ((a_u8_size & 3) << 14);
	return attrib1;
}

// Set the attribute affine index
void SetAttrib1AffineIndex(s_ObjectAttribute* a_o_p_object, u8 a_u8_index)
{
	//use 0xC1FF to strip out bits 9 -> D
	a_o_p_object->u16_attr1 = (a_o_p_object->u16_attr1 & 0xC1FF) | ((a_u8_index & A1_AFF_INDEX_MASK) << 9);
}

//functionality to set attribute 2 properties
u16 SetAttrib2(u16 a_u16_tileIndex, u8 a_u8_priority, u8 a_u8_paletteBank)
{
	u16 attrib2 = (a_u16_tileIndex & 0x3FF) | ((a_u8_priority & 3) << 10) | ((a_u8_paletteBank & 0xF) << 12);
	return attrib2;
}

//functionality to set position properties for an object
void SetObjectPosition(s_ObjectAttribute* a_o_p_object, u8 a_u8_x, u8 a_u8_y)
{
	a_o_p_object->u16_attr0 = (a_o_p_object->u16_attr0 & 0xFF00) | (a_u8_y & 0x00FF);
	a_o_p_object->u16_attr1 = (a_o_p_object->u16_attr1 & 0xFE00) | (a_u8_x & 0x01FF);
}

// Set the palette bank of an object attribute
void SetObjectPaletteBank(s_ObjectAttribute* a_o_p_object, u8 a_u8_paletteBank)
{
	a_o_p_object->u16_attr2 &= 0x0FFF;
	a_o_p_object->u16_attr2 |= (0x0000 | ((a_u8_paletteBank & 0xF) << 12));
}

// Set the tile index of a object attribute
void SetObjectTileIndex(s_ObjectAttribute* a_o_p_object, u16 a_u16_tileIndex)
{
	a_o_p_object->u16_attr2 &= 0xFC00;
	a_o_p_object->u16_attr2 |= (0x0000 | ((a_u16_tileIndex & 0x3FF)));
}

// Get an attributes position from the attribute data
s_Vector2 GetObjectPosition(s_ObjectAttribute* a_o_p_obj)
{
	s_Vector2 temp;
	temp.f_x = Int2Fix(a_o_p_obj->u16_attr0 >> 8 << 8);
	temp.f_y = Int2Fix(a_o_p_obj->u16_attr1 >> 12 << 12);
	return temp;
}

// Hide an object
void obj_hide(s_ObjectAttribute* a_o_p_obj)
{
	a_o_p_obj->u16_attr0 = (a_o_p_obj->u16_attr0 & A0_MODE_MASK) | (A0_MODE_HIDE << 8);
}

// Unhide an object
void obj_unhide(s_ObjectAttribute* a_o_p_obj, u8 a_u8_mode)
{
	a_o_p_obj->u16_attr0 = (a_o_p_obj->u16_attr0 & A0_MODE_MASK) | ((a_u8_mode & 0x3) << 8);
}

// Initilise OAM by assigning all the OAM slots with blank memory and making then hidden
void oam_init(s_ObjectAttribute* a_o_p_obj, u8 a_u8_count)
{
	u32 u32_nn = a_u8_count;
	s_ObjectAttribute* o_p_dst = a_o_p_obj;

	// Hide each object
	while (u32_nn--)
	{
		o_p_dst->u16_attr0	= 0;
		o_p_dst->u16_attr1	= 0;
		o_p_dst->u16_attr2	= 0;
		o_p_dst->s16_pad	= 0;

		obj_hide(o_p_dst);
		++o_p_dst;
	}

	// Init OAM
	oam_copy(MEM_OAM, a_o_p_obj, a_u8_count);
}


void oam_copy(s_ObjectAttribute* a_o_p_dst, s_ObjectAttribute* a_o_p_src, u8 a_u8_count)
{
	u32* dstw = (u32*)a_o_p_dst, *srcw = (u32*)a_o_p_src;
	while (a_u8_count--)
	{
		*dstw++ = *srcw++;
		*dstw++ = *srcw++;
	}
}

// Set the affine object matrix to an identity matrix
void obj_affine_identity(s_ObjectAffine* a_o_p_obj)
{
	a_o_p_obj->s16_pa = 0x01001;	a_o_p_obj->s16_pb = 0;
	a_o_p_obj->s16_pc = 0;			a_o_p_obj->s16_pd = 0x01001;
}

// Set the rotation and scale of the object
// uses the sin lookup table to retrieve a value for sine and cosine
void obj_affine_rotscale(s_ObjectAffine* a_o_p_obj, fixed a_f_sx, fixed a_f_sy, u16 a_u16_rot)
{
	int ss = LU_Sin(a_u16_rot), cc = LU_Cos(a_u16_rot);
	// Shifts values down 12 to account for fixed point multiply to keep values as .8 fixed
	a_o_p_obj->s16_pa = cc * a_f_sx >> 12;	a_o_p_obj->s16_pb = -ss * a_f_sx >> 12;
	a_o_p_obj->s16_pc = ss * a_f_sy >> 12;	a_o_p_obj->s16_pd = cc * a_f_sy >> 12;
}

// Set the rotation of an affine object
void obj_affine_rot(s_ObjectAffine* a_o_p_obj, u16 a_u16_rot)
{
	int ss = LU_Sin(a_u16_rot), cc = LU_Cos(a_u16_rot);

	a_o_p_obj->s16_pa = cc >> 4;	a_o_p_obj->s16_pb = -ss >> 4;
	a_o_p_obj->s16_pc = ss >> 4;	a_o_p_obj->s16_pd = cc >> 4;
}