////////////////////////////////////////////////////////////
//	File Name:		m_SpriteManager.cpp
//	Description:	A header file containing the sprite manager
//					class and a define for the max amount of
//					normal objects we can have
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "m_SpriteManager.h"

#include "gba_dma.h"
#include "gba_gfx.h"

#include "Text.h"
// Initialise the sprite managers variables
void SpriteManager::InitSpriteManager()
{
	// Clear OAM
	oam_init(MEM_OAM, 128);
	
	// Set the sprite attribute and affine counters to 0
	u8_ActiveSpriteAttributes = 0;
	u8_ActiveSpriteAffines = 0;

	// Cleat the empty OAM slot array
	for (u32 u32_i = 0; u32_i < 128; u32_i++)
	{
		u8_EmptyOAMSlots[u32_i] = 0;
	}

	// Text will always need to be 8 pixels apart
	u8_TextSpacing = 8;
	u32_TileIndex = 0;
	u8_PaletteIndex = 0;

	// Get palette for white text
	dma_word_cpy(pal_sp_memblock[15], &TextPal[0], TextPalLenHalf);
	//Get palette for yellow text
	dma_word_cpy(pal_sp_memblock[14], &TextPal[8], TextPalLenHalf);

	// Copy the image tiles into obj tile mem for text
	dma_word_cpy(&tile_mem[4][896], TextTiles, TextTilesLen);
}

// Creating a sprite by dynamically finding an empty spot in the sprite tile mem and OAM and copying the new sprite to them both
u8 SpriteManager::CreateSprite(const u16* a_cnst_u16_p_Palette, u8 a_u8_PaletteLen, const u16* a_cnst_u16_p_SpriteData, u16 a_u16_SpriteDataLen, u8 a_u8_x, u8 a_u8_y, u8 a_u8_Bpp, u8 a_u8_Shape, u8 a_u8_Size, u32 a_u32_TileIndex)
{
	u32 u32_TilesUsed = 0;

	// Find out how many tiles the sprite will use in the obj tile memory
	switch (a_u8_Shape)
	{
		// For square tiles
	case A0_SQUARE:
	{
		switch (a_u8_Size)
		{
		case A1_SIZE_0:
		{
			u32_TilesUsed = 1;
			break;
		}
		case A1_SIZE_1:
		{
			u32_TilesUsed = 4;
			break;
		}
		case A1_SIZE_2:
		{
			u32_TilesUsed = 16;
			break;
		}
		case A1_SIZE_3:
		{
			u32_TilesUsed = 64;
			break;
		}
		}
		break;
	}
	default:
	{
		switch (a_u8_Size)
		{
		case A1_SIZE_0:
		{
			u32_TilesUsed = 2;
			break;
		}
		case A1_SIZE_1:
		{
			u32_TilesUsed = 4;
			break;
		}
		case A1_SIZE_2:
		{
			u32_TilesUsed = 8;
			break;
		}
		case A1_SIZE_3:
		{
			u32_TilesUsed = 32;
			break;
		}
		}
		break;
	}
	}

	// Create the sprite
	if (u8_ActiveSpriteAttributes < MAXSPRITES)
	{
		if (a_cnst_u16_p_Palette != NULL)
		{
			// Copy palette
			dma_word_cpy(pal_sp_memblock[u8_PaletteIndex], a_cnst_u16_p_Palette, a_u8_PaletteLen);
		}

		// Copy image (4bpp/8bpp)
		if (a_u8_Bpp == 0)
			dma_word_cpy(&tile_mem[4][u32_TileIndex], a_cnst_u16_p_SpriteData, a_u16_SpriteDataLen);
		else
			dma_word_cpy(&tile_mem[4][u32_TileIndex], a_cnst_u16_p_SpriteData, a_u16_SpriteDataLen);
		
		u8 u8_OAMPos = 0;

		// Find a blank OAM slot
		for (u32 u32_i = 0; u32_i < 128; u32_i++)
		{
			if (u8_EmptyOAMSlots[u32_i] == 0)
			{
				u8_OAMPos = u32_i;
				// Set the slot to taken
				u8_EmptyOAMSlots[u32_i] = 1;
				break;
			}
		}

		// Create a new blank sprite
		s_ObjectAttribute* temp = &MEM_OAM[u8_OAMPos];

		// Set the attributes
		temp->u16_attr0 = SetAttrib0(a_u8_y, 0, 0, 0, a_u8_Bpp, a_u8_Shape);
		temp->u16_attr1 = SetAttrib1(a_u8_x, 0, a_u8_Size);
		temp->u16_attr2 = SetAttrib2(0, 0, u8_PaletteIndex);

		// Add on the tiles we have used in tile obj memory
		u32_TileIndex = u32_TileIndex + u32_TilesUsed;
		// Increment paletteIndex
		u8_PaletteIndex++;

		// Incriment the active sprites
		u8_ActiveSpriteAttributes++;

		// Return the index of the sprite in OAM
		return u8_ActiveSpriteAttributes;
	}

	// Return the index of the sprite in OAM
	return u8_ActiveSpriteAttributes;
}

// Destroying text sprites, can also be used for single sprites
void SpriteManager::DestroySprite(u8* a_u8_OAMIndex, u8 a_u8_Length)
{
	// For all the sprite OAM indexes we pass in
	for (u32 u32_i = 0; u32_i < a_u8_Length; u32_i++)
	{
		// Set the attributes of that sprite to 0
		MEM_OAM[a_u8_OAMIndex[u32_i]].u16_attr0 = 0;
		MEM_OAM[a_u8_OAMIndex[u32_i]].u16_attr1 = 0;
		MEM_OAM[a_u8_OAMIndex[u32_i]].u16_attr2 = 0;
		// Hide the sprite too
		obj_hide(&MEM_OAM[a_u8_OAMIndex[u32_i]]);

		// Clear the slot in the empty OAM slots array so another sprite can take its place
		u8_EmptyOAMSlots[a_u8_OAMIndex[u32_i]] = 0;

		// Decriment active sprite attributes
		u8_ActiveSpriteAttributes--;
	}
}

// Drawing text
#include "textlut.h"
void SpriteManager::DrawText(char a_text[], u8* a_u8_p_IndexArray, u8 a_x, u8 a_y, u8 a_length)
{
	u8 u8_CharValue;
	// Default tile pos is blank space
	u8 u8_TilePos = 38;

	// Going through the text array
	for (u32 u32_i = 0; u32_i < a_length; u32_i++)
	{
		u8_CharValue = (u8)a_text[u32_i];

		for (u32 u32_jj = 0; u32_jj < textLutLen; ++u32_jj)
		{
			if (u8_CharValue == textLut[u32_jj])
			{
				u8_TilePos = u32_jj;
				break;
			}
		}

		// Find a blank OAM slot
		u8 u8_OAMPos = 0;
		for (u32 u32_jj = 0; u32_jj < 128; u32_jj++)
		{
			if (u8_EmptyOAMSlots[u32_jj] == 0)
			{
				u8_OAMPos = u32_jj;
				a_u8_p_IndexArray[u32_i] = u32_jj;
				u8_EmptyOAMSlots[u32_jj] = 1;
				break;
			}
		}

		//Create a new sprite and put it into the empty OAm slot we just found
		s_ObjectAttribute* o_p_ObjectChar;
		o_p_ObjectChar = &MEM_OAM[u8_OAMPos];

		// Set the attributes for a 1 tile big sprite
		o_p_ObjectChar->u16_attr0 = SetAttrib0(a_y, A0_MODE_AFF, 0, 0, A0_4BPP, A0_SQUARE);
		o_p_ObjectChar->u16_attr1 = SetAttrib1(a_x + (u8_TextSpacing * u32_i), 0, A1_SIZE_0);
		// Set the tile pos so that the sprite will display the right character
		// All the characters are stored from sprite tile mem index 896
		o_p_ObjectChar->u16_attr2 = SetAttrib2(896 + u8_TilePos, 0, 15);

		// Make an affine for the text sprite in case we want to scale or rotate the sprite
		SetAttrib1AffineIndex(o_p_ObjectChar, u8_ActiveSpriteAffines);

		// Store the new affine sprite in affine OAM
		s_ObjectAffine* o_p_ObjectCharAff;
		o_p_ObjectCharAff = &MEM_AFF_OAM[u8_ActiveSpriteAffines];

		// The scale of text can be controlled here
		// We just need to uncomment the commented line below and change the values to scale text

		fixed a_f_Scale = Int2Fix(1);
		//a_f_Scale = FixDiv(Int2Fix(64), Int2Fix(64));
		obj_affine_rotscale(o_p_ObjectCharAff, a_f_Scale, a_f_Scale, 0);

		u8_ActiveSpriteAttributes++;
		u8_ActiveSpriteAffines++;
		
		// Reset the default tile pos to a blank space in preparation for the next character
		u8_TilePos = 38;
	}
}

// Change what an existing text object says
void SpriteManager::UpdateText(char a_text[], c_Text a_textObj, u8 a_length)
{
	u8 u8_CharValue;
	// Default tile pos is blank space
	u8 u8_TilePos = 38;

	// Find the tilepos based on ASCII value
	for (u32 u32_i = 0; u32_i < a_length; ++u32_i)
	{
		u8_CharValue = (u8)a_text[u32_i];

		for (u32 u32_jj = 0; u32_jj < textLutLen; ++u32_jj)
		{
			if (u8_CharValue == textLut[u32_jj])
			{
				u8_TilePos = u32_jj;
				break;
			}
		}

		// Reset the tile index, text is always stored from sprite tile mem position 896
		SetObjectTileIndex(&MEM_OAM[a_textObj.u8_Index[u32_i]], 896 + u8_TilePos);

		// Set tilepos back to a blank space
		u8_TilePos = 38;
	}
}