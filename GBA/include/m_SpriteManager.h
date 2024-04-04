////////////////////////////////////////////////////////////
//	File Name:		m_SpriteManager.h
//	Description:	A header file containing the sprite manager
//					class and a define for the max amount of
//					normal objects we can have
//	Author:			Lily Raeburn
////////////////////////////////////////////////////////////

#ifndef _M_SPRITEMANAGER_H_
#define _M_SPRITEMANAGER_H_

// The maximum sprites we can fit into OAM
#define MAXSPRITES 128

#include "gba_gfx.h"
#include "textObject.h"

// The sprite manager class, manages sprite create, deletion, drawing text, and changing text
class SpriteManager
{
public:

	// Index for obj tile mem
	u32		u32_TileIndex;
	// Index for sprite palettes
	u8		u8_PaletteIndex;
	// Count for how many sprites we have active
	u8		u8_ActiveSpriteAttributes;
	u8		u8_ActiveSpriteAffines;
	// An array for the empty OAM slots
	u8		u8_EmptyOAMSlots[128];
	// Spacing for text
	u8		u8_TextSpacing;

	// Initialise the values we need for the sprite manager to work
	void InitSpriteManager();

	// Creat a sprite
	u8 CreateSprite(const u16* a_palette, u8 a_paletteLen, const u16* a_spriteData, u16 a_spriteDataLen, u8 a_x, u8 a_y, u8 a_bpp, u8 a_shape, u8 a_size, u32 a_tileIndex = 0);
	// Destroy a sprite
	void DestroySprite(u8* a_u8_OAMIndex, u8 a_u8_Length);

	// Draw text
	void DrawText(char a_text[], u8* a_u8_p_IndexArray, u8 a_x, u8 a_y, u8 a_length = 1);
	// Change currently existing text
	void UpdateText(char a_text[], c_Text a_textObj, u8 a_length);
};

#endif //_M_SPRITEMANAGER_H_