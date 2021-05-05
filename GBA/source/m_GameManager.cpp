 ////////////////////////////////////////////////////////////
//	File Name:		m_GameManager.cpp
//	Description:	A header file for the enums, defines,
//					classes and functionality needed for
//					managing the game
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "gba.h"

//--------------------------------------------------
//	Initialisation

// Initialising variables for game states and menu selection states
void c_GameManager::InitGameManager()
{
	this->eo_GameState = intro;
	this->eo_MenuSelection = startgame;
	this->eo_OptionsMenuSelection = Option_Vsync;

	// Set the cloud background positions
	o_BG1Pos.f_x = 0;
	o_BG1Pos.f_y = Int2Fix(-10);
	o_BG2Pos.f_x = Int2Fix(45);
	o_BG2Pos.f_y = Int2Fix(45);

	// Set the cloud bobbing look up table index to 0
	u16_yLutIndex = 0;

	// Init game options
	u8_VSYNC = 1;
	u8_Parallaxing = 1;
	u8_DisableTimer = 0;
	u8_CollisionDebug = 0;

	// Seeding random
	SQRan(32452843);
}

// The main game loop
// Controls the game states
void c_GameManager::Run()
{
	////////////////////////////////////////
	// Game manager will initialise itself
	InitGameManager();

	////////////////////////////////////////
	// The main game loop
	while (1)
	{
		switch (this->eo_GameState)
		{
		case intro:
		{
			this->Intro();
			break;
		}
		case menu:
		{
			this->Menu();
			break;
		}
		case game:
		{
			this->Level1();
			break;
		}
		// Deafult game loop is the intro so if something goes really wrong the game will restart
		default:
		{
			this->eo_GameState = intro;
			break;
		}
		}
	}
}

//--------------------------------------------------
//	Game intro
#include "Title.h"

void c_GameManager::Intro()
{
	//set GBA rendering to MODE 3 Bitmap rendering
	REG_DISPLAYCONTROL = VIDEOMODE_4 | BGMODE_2;

	// Copy the title image into the video memory
	dma_word_cpy(u16_p_vid_page, TitleBitmap, TitleBitmapLen);
	dma_word_cpy(pal_bg_mem, TitlePal, TitlePalLen);

	// Wait for vsync
	Vsync();

	// Flip the page
	Page_Flip();

	// Sleep for 3 seconds
	Sleep(3, TM_SECOND);

	// Change the game state to menu
	this->eo_GameState = menu;
}

#include "MenuBanner.h"
#include "Menu0.h"
#include "Menu1.h"
#include "Menu2.h"
#include "GameTileSet.h"
#include "Mario_Sheet.h"
#include "sinlut.h"
void c_GameManager::Menu()
{
	o_SpriteManager.InitSpriteManager();

	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | BGMODE_1 | BGMODE_2 | BGMODE_3 | MAPPINGMODE_1D;

	// Set the camera/viewport positions
	REG_BG_OFS[0].s16_x = 0;
	REG_BG_OFS[0].s16_y = 0;
	REG_BG_OFS[1].s16_x = 0;
	REG_BG_OFS[1].s16_y = 0;
	REG_BG_OFS[2].s16_x = 0;
	REG_BG_OFS[2].s16_y = 0;
	REG_BG_OFS[3].s16_x = 0;
	REG_BG_OFS[3].s16_y = 0;

	// Load the game text
	LoadGameText();

	// Load the mario bros banner
	dma_word_cpy(pal_sp_mem, MenuBannerPal, MenuBannerPalLen);
	dma_word_cpy(&tile_mem[5][0], MenuBannerTiles, MenuBanner64X32TileLen * 10);

	// An object attribute array to hold the intro banner
	s_ObjectAttribute*		o_BannerObjects[9];
	// An array of vectors to hold the intro banner object positions
	s_Vector2				o_BannerObjectsPositions[9];

	// An origin point for all the intro banner objects
	fixed f_x = 0;
	fixed f_y = 0;

	// Load banner objects into OAM
	for (u32 u32_i = 0; u32_i < MenuBannerTilesSegmentLen; u32_i++)
	{
		if (u32_i % MenuBannerTileWidth == 0)
		{
			f_y = FixAdd(f_y, Int2Fix(1));
			f_x = 0;
		}

		s_ObjectAttribute* o_MenuBanner;
		// Put the sprites at the very end of OAM so they are not overwritten by the sprite manager
		o_MenuBanner = &MEM_OAM[u32_i + 118];

		s_Vector2 o_TempVec;
		o_TempVec.f_x = FixMul(f_x, Int2Fix(64));
		o_TempVec.f_y = FixMul(f_y, Int2Fix(32));

		o_MenuBanner->u16_attr0 = SetAttrib0(Fix2Int(o_TempVec.f_y), 0, 0, 0, A0_4BPP, A0_WIDE);
		o_MenuBanner->u16_attr1 = SetAttrib1(Fix2Int(o_TempVec.f_x), 0, A1_SIZE_3);
		o_MenuBanner->u16_attr2 = SetAttrib2((u32_i * 32) + 512, 0, 1);
		f_x = FixAdd(f_x, Int2Fix(1));

		o_BannerObjects[u32_i] = o_MenuBanner;
		o_BannerObjectsPositions[u32_i] = o_TempVec;
	}

	// Telling the sprite manager how many attributes we have used
	// as we did not use the CreateSprite function
	o_SpriteManager.u8_ActiveSpriteAttributes = 9;

	// Load background
	// Load palette
	dma_word_cpy(pal_bg_mem, GameTileSetPal, GameTileSetPalLen);

	// Load tile memory location
	// & with 0x3 so that we cannot access outside of layers 0-3
	u16* u16_p_TileMemLoc = Tile_Block_Address(0);

	// Copy the background data into our tile memory location
	dma_word_cpy(u16_p_TileMemLoc, GameTileSetTiles, GameTileSetTilesLen);

	// Get the tilemap memory location and copy the map data into it
	// We only want to use tileblock locations 0, 8, 16 and 24
	// so we bitshift a_bgLayer up 3
	u16* u16_p_TileMapMemLoc = TileMap_Block_Address(16);
	dma_word_cpy(u16_p_TileMapMemLoc, Menu0, Menu0Len);

	u16_p_TileMapMemLoc = TileMap_Block_Address(17);
	dma_word_cpy(u16_p_TileMapMemLoc, Menu1, Menu1Len);

	u16_p_TileMapMemLoc = TileMap_Block_Address(18);
	dma_word_cpy(u16_p_TileMapMemLoc, Menu1, Menu1Len);

	u16_p_TileMapMemLoc = TileMap_Block_Address(19);
	dma_word_cpy(u16_p_TileMapMemLoc, Menu2, Menu2Len);

	// Set up the background control register
	REG_BGCNT[0] = SetBG_Control_Register(0, 0, 0, 0, 16, 0, BG_REG_SIZE_32x32);
	REG_BGCNT[1] = SetBG_Control_Register(1, 0, 0, 0, 17, 0, BG_REG_SIZE_32x32);
	REG_BGCNT[2] = SetBG_Control_Register(2, 0, 0, 0, 18, 0, BG_REG_SIZE_32x32);
	REG_BGCNT[3] = SetBG_Control_Register(3, 0, 0, 0, 19, 0, BG_REG_SIZE_32x32);

	// Another origin point for the banner objects
	// This point is above the screen and will allow the banner to come down from the top
	f_x = Int2Fix(32);
	f_y = Int2Fix(130);

	// Variables for the camera/viewport positions
	// These positions will be the starting point for the clouds in the background

	fixed f_xBack1Speed = FixDiv(Int2Fix(1), Int2Fix(5));
	fixed f_xBack2Speed = FixDiv(Int2Fix(1), Int2Fix(2));
	
	// Applying the background positions
	REG_BG_OFS[1].s16_x = Fix2Int(o_BG1Pos.f_x);
	REG_BG_OFS[1].s16_y = Fix2Int(o_BG1Pos.f_y);

	REG_BG_OFS[2].s16_x = Fix2Int(o_BG2Pos.f_x);
	REG_BG_OFS[1].s16_y = Fix2Int(o_BG2Pos.f_y);

	// Loading Mario for the intro screen
	c_Mario o_Player;
	o_Player.InitMario();
	o_Player.o_Pos.f_x = Int2Fix(16);
	o_Player.o_Pos.f_y = Int2Fix(120);

	SetObjectPosition(o_Player.o_Sprite.o_Attribute, Fix2Int(o_Player.o_Pos.f_x), Fix2Int(o_Player.o_Pos.f_y));

	// Intro animation
	for (u32 u32_i = 0; u32_i < 220; u32_i++)
	{
		// Get user input
		PollKeys();

		// Making sure our yBackIndex cant go outside the sin look up table
		u16_yLutIndex++;
		u16_yLutIndex = u16_yLutIndex % sin_lut_Size;

		// Assigning the background positions
		REG_BG_OFS[1].s16_y = Fix2Int(FixAdd(o_BG1Pos.f_y, sin_lut[u16_yLutIndex]));
		REG_BG_OFS[2].s16_y = Fix2Int(FixAdd(o_BG2Pos.f_y, FixMul(sin_lut[u16_yLutIndex], Int2Fix(2))));

		o_BG1Pos.f_x = FixAdd(o_BG1Pos.f_x, f_xBack1Speed);
		o_BG2Pos.f_x = FixAdd(o_BG2Pos.f_x, f_xBack2Speed);

		REG_BG_OFS[1].s16_x = Fix2Int(o_BG1Pos.f_x);
		REG_BG_OFS[2].s16_x = Fix2Int(o_BG1Pos.f_x);

		// Banner animation in the intro
		f_y = FixAdd(f_y, FixDiv(Int2Fix(1), Int2Fix(2)));

		for (u32 u32_i = 0; u32_i < 9; u32_i++)
		{
			SetObjectPosition(o_BannerObjects[u32_i], Fix2Int(FixAdd(f_x, o_BannerObjectsPositions[u32_i].f_x)), Fix2Int(FixAdd(f_y, o_BannerObjectsPositions[u32_i].f_y)));
		}

		// Check if the user pressed start to skip the animation
		if (KeyDown(START))
		{
			f_y = Int2Fix(240);

			for (u32 u32_i = 0; u32_i < 9; u32_i++)
			{
				SetObjectPosition(o_BannerObjects[u32_i], Fix2Int(FixAdd(f_x, o_BannerObjectsPositions[u32_i].f_x)), Fix2Int(FixAdd(f_y, o_BannerObjectsPositions[u32_i].f_y)));
			}

			break;
		}

		// Vsync if the option is set
		if (u8_VSYNC == 1)
		{
			Vsync();
		}
	}

	// Intro text objects
	c_Text o_Intro_startgame;
	o_Intro_startgame.u8_Length = 10;
	o_SpriteManager.DrawText((char*)"START GAME", o_Intro_startgame.u8_Index, 80, 105, o_Intro_startgame.u8_Length);

	c_Text o_Intro_options;
	o_Intro_options.u8_Length = 7;
	o_SpriteManager.DrawText((char*)"OPTIONS", o_Intro_options.u8_Index, 90, 115, o_Intro_options.u8_Length);

	c_Text o_Intro_credits;
	o_Intro_credits.u8_Length = 7;
	o_SpriteManager.DrawText((char*)"CREDITS", o_Intro_credits.u8_Index, 90, 125, o_Intro_credits.u8_Length);

	// Menu navigation variables
	u8 u8_CurrentMenuItem = 0;
	u8 u8_RedrawMenu = 1;
	u8 u8_MenuItems = 3; //0b0011

	// Main menu code
	while (this->eo_GameState == menu)
	{
		// Get keys
		PollKeys();

		// Move menu selection
		// Allows us to scroll through 3 options
		if (KeyHit(UP) || KeyHit(DOWN))
		{
			u8_CurrentMenuItem -= GetAxis(VERTICAL);
			u8_CurrentMenuItem = (u8_CurrentMenuItem & u8_MenuItems);
			if (u8_CurrentMenuItem == u8_MenuItems)
			{
				u8_CurrentMenuItem -= GetAxis(VERTICAL);
				u8_CurrentMenuItem = (u8_CurrentMenuItem & u8_MenuItems);
			}

			this->eo_MenuSelection = (e_MenuStates)u8_CurrentMenuItem;
			u8_RedrawMenu = 1;
		}

		// Menu interaction
		if (KeyHit(A) || KeyHit(START))
		{
			switch (this->eo_MenuSelection)
			{
			case startgame:
			{
				this->eo_GameState = game;
				break;
			}
			case options:
			{
				// Hiding the main menu Text and banner
				for (u32 u32_i = 0; u32_i < 9; u32_i++)
				{
					obj_hide(o_BannerObjects[u32_i]);
				}
				o_SpriteManager.DestroySprite(o_Intro_startgame.u8_Index, o_Intro_startgame.u8_Length);
				o_SpriteManager.DestroySprite(o_Intro_options.u8_Index, o_Intro_options.u8_Length);
				o_SpriteManager.DestroySprite(o_Intro_credits.u8_Index, o_Intro_credits.u8_Length);

				// A 'bool' for controlling the options menu
				u8 u8_InOptions = 1;

				// Options menu text
				c_Text o_Option_Title;
				o_Option_Title.u8_Length = 7;
				o_SpriteManager.DrawText((char*)"OPTIONS", o_Option_Title.u8_Index, 90, 40, o_Option_Title.u8_Length);

				// Change the palette for the title text
				for (u32 u32_i = 0; u32_i < o_Option_Title.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Option_Title.u8_Index[u32_i]], 14);

				// More options menu text
				c_Text o_Option_Vsync;
				o_Option_Vsync.u8_Length = 16;
				o_SpriteManager.DrawText((char*)"VSYNC        ON ", o_Option_Vsync.u8_Index, 60, 70, o_Option_Vsync.u8_Length);

				c_Text o_Options_Parallaxing;
				o_Options_Parallaxing.u8_Length = 16;
				o_SpriteManager.DrawText((char*)"PARALLAXING  ON ", o_Options_Parallaxing.u8_Index, 60, 80, o_Options_Parallaxing.u8_Length);

				c_Text o_Options_DisableTimer;
				o_Options_DisableTimer.u8_Length = 16;
				o_SpriteManager.DrawText((char*)"DISABLE TMR  OFF", o_Options_DisableTimer.u8_Index, 60, 90, o_Options_DisableTimer.u8_Length);

				c_Text o_Options_CollisionDebug;
				o_Options_CollisionDebug.u8_Length = 16;
				o_SpriteManager.DrawText((char*)"COL DEBUG    OFF", o_Options_CollisionDebug.u8_Index, 60, 100, o_Options_CollisionDebug.u8_Length);

				// Options menu selection variables
				u8 u8_CurrentOptionsMenuItem = 0;
				u8 u8_RedrawOptionsMenu = 1;
				u8 u8_OptionsMenuItems = 3; //0b0011

				// Options update loop
				while (u8_InOptions == 1)
				{
					// If b is pressed go back
					if (KeyHit(B) != 0)
					{
						u8_InOptions = 0;
					}

					// Get user input
					PollKeys();

					// Move options menu selection
					if (KeyHit(UP) || KeyHit(DOWN))
					{
						u8_CurrentOptionsMenuItem -= GetAxis(VERTICAL);
						u8_CurrentOptionsMenuItem = (u8_CurrentOptionsMenuItem & u8_OptionsMenuItems);

						this->eo_OptionsMenuSelection = (e_OptionsMenuStates)u8_CurrentOptionsMenuItem;
						u8_RedrawOptionsMenu = 1;
					}

					// Menu interaction
					if (KeyHit(A) || KeyHit(START))
					{
						// Turning menu options on and off and then redrawing the menu
						switch (this->eo_OptionsMenuSelection)
						{
						case Option_Vsync:
						{
							if (u8_VSYNC == 0)
							{
								u8_VSYNC = 1;
							}
							else
							{
								u8_VSYNC = 0;
							}

							u8_RedrawOptionsMenu = 1;
							break;
						}
						case Option_Parallaxing:
						{
							if (u8_Parallaxing == 0)
							{
								u8_Parallaxing = 1;
							}
							else
							{
								u8_Parallaxing = 0;
							}

							u8_RedrawOptionsMenu = 1;
							break;
						}
						case Option_DisablerTimer:
						{
							if (u8_DisableTimer == 0)
							{
								u8_DisableTimer = 1;
							}
							else
							{
								u8_DisableTimer = 0;
							}

							u8_RedrawOptionsMenu = 1;
							break;
						}
						case Option_ColDebug:
						{
							if (u8_CollisionDebug == 0)
							{
								u8_CollisionDebug = 1;
							}
							else
							{
								u8_CollisionDebug = 0;
							}

							u8_RedrawOptionsMenu = 1;
							break;
						}
						}
					}

					// If we need to change the options menu text palettes and text
					if (u8_RedrawOptionsMenu == 1)
					{
						// Update the options text
						if (u8_VSYNC == 1)
						{
							o_SpriteManager.UpdateText((char*)"VSYNC        ON ", o_Option_Vsync, o_Option_Vsync.u8_Length);
						}
						else
						{
							o_SpriteManager.UpdateText((char*)"VSYNC        OFF", o_Option_Vsync, o_Option_Vsync.u8_Length);
						}

						if (u8_Parallaxing == 1)
						{
							o_SpriteManager.UpdateText((char*)"PARALLAXING  ON ", o_Options_Parallaxing, o_Options_Parallaxing.u8_Length);
						}
						else
						{
							o_SpriteManager.UpdateText((char*)"PARALLAXING  OFF", o_Options_Parallaxing, o_Options_Parallaxing.u8_Length);
						}

						if (u8_DisableTimer == 0)
						{
							o_SpriteManager.UpdateText((char*)"DISABLE TMR  OFF", o_Options_DisableTimer, o_Options_DisableTimer.u8_Length);
						}
						else
						{
							o_SpriteManager.UpdateText((char*)"DISABLE TMR  ON ", o_Options_DisableTimer, o_Options_DisableTimer.u8_Length);
						}

						if (u8_CollisionDebug == 0)
						{
							o_SpriteManager.UpdateText((char*)"COL DEBUG    OFF", o_Options_CollisionDebug, o_Options_CollisionDebug.u8_Length);
						}
						else
						{
							o_SpriteManager.UpdateText((char*)"COL DEBUG    ON ", o_Options_CollisionDebug, o_Options_CollisionDebug.u8_Length);
						}

						// Update the palettes of the menu text depending on the current selection
						switch (this->eo_OptionsMenuSelection)
						{
						case Option_Vsync:
						{
							for (u32 u32_i = 0; u32_i < o_Option_Vsync.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Option_Vsync.u8_Index[u32_i]], 14);

							for (u32 u32_i = 0; u32_i < o_Options_Parallaxing.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_Parallaxing.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_DisableTimer.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_DisableTimer.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_CollisionDebug.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_CollisionDebug.u8_Index[u32_i]], 15);
							break;
						}
						case Option_Parallaxing:
						{
							for (u32 u32_i = 0; u32_i < o_Option_Vsync.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Option_Vsync.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_Parallaxing.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_Parallaxing.u8_Index[u32_i]], 14);

							for (u32 u32_i = 0; u32_i < o_Options_DisableTimer.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_DisableTimer.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_CollisionDebug.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_CollisionDebug.u8_Index[u32_i]], 15);
							break;
						}
						case Option_DisablerTimer:
						{
							for (u32 u32_i = 0; u32_i < o_Option_Vsync.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Option_Vsync.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_Parallaxing.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_Parallaxing.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_DisableTimer.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_DisableTimer.u8_Index[u32_i]], 14);

							for (u32 u32_i = 0; u32_i < o_Options_CollisionDebug.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_CollisionDebug.u8_Index[u32_i]], 15);
							break;
						}
						case Option_ColDebug:
						{
							for (u32 u32_i = 0; u32_i < o_Option_Vsync.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Option_Vsync.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_Parallaxing.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_Parallaxing.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_DisableTimer.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_DisableTimer.u8_Index[u32_i]], 15);

							for (u32 u32_i = 0; u32_i < o_Options_CollisionDebug.u8_Length; ++u32_i)
								SetObjectPaletteBank(&MEM_OAM[o_Options_CollisionDebug.u8_Index[u32_i]], 14);
							break;
						}
						}
						u8_RedrawOptionsMenu = 0;
					}


					// Moving clouds since they still need to move in the options menu
					// Making sure our yBackIndex cant go outside the sin look up table
					u16_yLutIndex++;
					if (u16_yLutIndex >= sin_lut_Size)
						u16_yLutIndex = 0;
					
					// Assigning the background positions
					REG_BG_OFS[1].s16_y = Fix2Int(FixAdd(o_BG1Pos.f_y, sin_lut[u16_yLutIndex]));
					REG_BG_OFS[2].s16_y = Fix2Int(FixAdd(o_BG2Pos.f_y, FixMul(sin_lut[u16_yLutIndex], Int2Fix(2))));

					if (u8_Parallaxing == 1)
					{
						o_BG1Pos.f_x = FixAdd(o_BG1Pos.f_x, f_xBack1Speed);
						o_BG2Pos.f_x = FixAdd(o_BG2Pos.f_x, f_xBack2Speed);

						REG_BG_OFS[1].s16_x = Fix2Int(o_BG1Pos.f_x);
						REG_BG_OFS[2].s16_x = Fix2Int(o_BG1Pos.f_x);
					}

					// Flip the page
					Page_Flip();

					// Do Vsync if the vsync options is set
					if (u8_VSYNC == 1)
					{
						Vsync();
					}
				}

				// Delete options menu text
				o_SpriteManager.DestroySprite(o_Option_Title.u8_Index, o_Option_Title.u8_Length);
				o_SpriteManager.DestroySprite(o_Option_Vsync.u8_Index, o_Option_Vsync.u8_Length);
				o_SpriteManager.DestroySprite(o_Options_Parallaxing.u8_Index, o_Options_Parallaxing.u8_Length);
				o_SpriteManager.DestroySprite(o_Options_DisableTimer.u8_Index, o_Options_DisableTimer.u8_Length);
				o_SpriteManager.DestroySprite(o_Options_CollisionDebug.u8_Index, o_Options_CollisionDebug.u8_Length);
				
				// Unhiding the main menu items
				o_SpriteManager.DrawText((char*)"START GAME", o_Intro_startgame.u8_Index, 80, 105, o_Intro_startgame.u8_Length);
				o_SpriteManager.DrawText((char*)"OPTIONS", o_Intro_options.u8_Index, 90, 115, o_Intro_options.u8_Length);
				o_SpriteManager.DrawText((char*)"CREDITS", o_Intro_credits.u8_Index, 90, 125, o_Intro_credits.u8_Length);
				for (u32 u32_i = 0; u32_i < 9; u32_i++)
				{
					obj_unhide(o_BannerObjects[u32_i], 0);
				}

				// Redraw the main menu
				u8_RedrawMenu = 1;
				break;
			}
			case credits:
			{
				// Hiding the main menu items
				for (u32 u32_i = 0; u32_i < 9; u32_i++)
				{
					obj_hide(o_BannerObjects[u32_i]);
				}
				o_SpriteManager.DestroySprite(o_Intro_startgame.u8_Index, o_Intro_startgame.u8_Length);
				o_SpriteManager.DestroySprite(o_Intro_options.u8_Index, o_Intro_options.u8_Length);
				o_SpriteManager.DestroySprite(o_Intro_credits.u8_Index, o_Intro_credits.u8_Length);

				// Credits text
				c_Text o_Credits_Text1;
				o_Credits_Text1.u8_Length = 7;
				o_SpriteManager.DrawText((char*)"GAME BY", o_Credits_Text1.u8_Index, 95, 45, o_Credits_Text1.u8_Length);

				c_Text o_Credits_Text2;
				o_Credits_Text2.u8_Length = 12;
				o_SpriteManager.DrawText((char*)"ZACK RAEBURN", o_Credits_Text2.u8_Index, 75, 55, o_Credits_Text2.u8_Length);

				c_Text o_Credits_Text3;
				o_Credits_Text3.u8_Length = 16;
				o_SpriteManager.DrawText((char*)"VISIT MY WEBSITE", o_Credits_Text3.u8_Index, 60, 75, o_Credits_Text3.u8_Length);

				c_Text o_Credits_Text4;
				o_Credits_Text4.u8_Length = 10;
				o_SpriteManager.DrawText((char*)"WWW.ZACKRA", o_Credits_Text4.u8_Index, 50, 85, o_Credits_Text4.u8_Length);

				c_Text o_Credits_Text5;
				o_Credits_Text5.u8_Length = 9;
				o_SpriteManager.DrawText((char*)"EBURN.COM", o_Credits_Text5.u8_Index, 130, 85, o_Credits_Text5.u8_Length);

				// Changing palettes for the website text
				for (u32 u32_i = 0; u32_i < o_Credits_Text4.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Credits_Text4.u8_Index[u32_i]], 14);
				for (u32 u32_i = 0; u32_i < o_Credits_Text5.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Credits_Text5.u8_Index[u32_i]], 14);

				// 'Bool' for the credits loop
				u8 u8_InCredits = 1;

				while (u8_InCredits == 1)
				{
					// Get user input
					PollKeys();

					if (KeyHit(START) || KeyHit(A) || KeyHit(B))
					{
						u8_InCredits = 0;
					}

					// Moving clouds since they still need to move in the options menu
					// Making sure our yBackIndex cant go outside the sin look up table
					u16_yLutIndex++;
					if (u16_yLutIndex >= sin_lut_Size)
						u16_yLutIndex = 0;

					// Assigning the background positions
					REG_BG_OFS[1].s16_y = Fix2Int(FixAdd(o_BG1Pos.f_y, sin_lut[u16_yLutIndex]));
					REG_BG_OFS[2].s16_y = Fix2Int(FixAdd(o_BG2Pos.f_y, FixMul(sin_lut[u16_yLutIndex], Int2Fix(2))));

					if (u8_Parallaxing == 1)
					{
						o_BG1Pos.f_x = FixAdd(o_BG1Pos.f_x, f_xBack1Speed);
						o_BG2Pos.f_x = FixAdd(o_BG2Pos.f_x, f_xBack2Speed);

						REG_BG_OFS[1].s16_x = Fix2Int(o_BG1Pos.f_x);
						REG_BG_OFS[2].s16_x = Fix2Int(o_BG1Pos.f_x);
					}

					Page_Flip();

					if (u8_VSYNC == 1)
					{
						Vsync();
					}
				}

				// Delete options menu text
				o_SpriteManager.DestroySprite(o_Credits_Text1.u8_Index, o_Credits_Text1.u8_Length);
				o_SpriteManager.DestroySprite(o_Credits_Text2.u8_Index, o_Credits_Text2.u8_Length);
				o_SpriteManager.DestroySprite(o_Credits_Text3.u8_Index, o_Credits_Text3.u8_Length);
				o_SpriteManager.DestroySprite(o_Credits_Text4.u8_Index, o_Credits_Text4.u8_Length);
				o_SpriteManager.DestroySprite(o_Credits_Text5.u8_Index, o_Credits_Text5.u8_Length);

				// Unhiding the main menu items
				o_SpriteManager.DrawText((char*)"START GAME", o_Intro_startgame.u8_Index, 80, 105, o_Intro_startgame.u8_Length);
				o_SpriteManager.DrawText((char*)"OPTIONS", o_Intro_options.u8_Index, 90, 115, o_Intro_options.u8_Length);
				o_SpriteManager.DrawText((char*)"CREDITS", o_Intro_credits.u8_Index, 90, 125, o_Intro_credits.u8_Length);
				for (u32 u32_i = 0; u32_i < 9; u32_i++)
				{
					obj_unhide(o_BannerObjects[u32_i], 0);
				}

				// Redraw the main menu
				u8_RedrawMenu = 1;
				break;
			}
			}
		}

		// If we need to change the menu text palettes
		if (u8_RedrawMenu == 1)
		{
			switch (this->eo_MenuSelection)
			{
			case startgame:
			{
				for (u32 u32_i = 0; u32_i < o_Intro_startgame.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_startgame.u8_Index[u32_i]], 14);

				for (u32 u32_i = 0; u32_i < o_Intro_options.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_options.u8_Index[u32_i]], 15);

				for (u32 u32_i = 0; u32_i < o_Intro_credits.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_credits.u8_Index[u32_i]], 15);
				break;
			}
			case options:
			{
				for (u32 u32_i = 0; u32_i < o_Intro_startgame.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_startgame.u8_Index[u32_i]], 15);

				for (u32 u32_i = 0; u32_i < o_Intro_options.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_options.u8_Index[u32_i]], 14);

				for (u32 u32_i = 0; u32_i < o_Intro_credits.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_credits.u8_Index[u32_i]], 15);
				break;
			}
			case credits:
			{
				for (u32 u32_i = 0; u32_i < o_Intro_startgame.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_startgame.u8_Index[u32_i]], 15);

				for (u32 u32_i = 0; u32_i < o_Intro_options.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_options.u8_Index[u32_i]], 15);

				for (u32 u32_i = 0; u32_i < o_Intro_credits.u8_Length; ++u32_i)
					SetObjectPaletteBank(&MEM_OAM[o_Intro_credits.u8_Index[u32_i]], 14);
				break;
			}
			}
			u8_RedrawMenu = 0;
		}


		// Making sure our yBackIndex cant go outside the sin look up table
		u16_yLutIndex++;
		if (u16_yLutIndex >= sin_lut_Size)
			u16_yLutIndex = 0;

		REG_BG_OFS[1].s16_y = Fix2Int(FixAdd(o_BG1Pos.f_y, LU_Sin(u16_yLutIndex)));
		REG_BG_OFS[2].s16_y = Fix2Int(FixAdd(o_BG2Pos.f_y, FixMul(LU_Cos(u16_yLutIndex), Int2Fix(2))));

		if (u8_Parallaxing == 1)
		{
			o_BG1Pos.f_x = FixAdd(o_BG1Pos.f_x, f_xBack1Speed);
			o_BG2Pos.f_x = FixAdd(o_BG2Pos.f_x, f_xBack2Speed);

			REG_BG_OFS[1].s16_x = Fix2Int(o_BG1Pos.f_x);
			REG_BG_OFS[2].s16_x = Fix2Int(o_BG2Pos.f_x);
		}

		// Flip the page
		Page_Flip();

		// Do Vsync if the Vsync option is set
		if (u8_VSYNC == 1)
		{
			Vsync();
		}
	}
}

//--------------------------------------------------
//	Levels
void c_GameManager::Level1()
{
	// Set the display modes
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | BGMODE_0 | BGMODE_1 | BGMODE_2 | BGMODE_3 | MAPPINGMODE_1D;
	// Load the background for this level
	LoadLevel1();

	// Initialise OAM (clears it)
	oam_init(MEM_OAM, 128);

	// Clear the sprite manager
	o_SpriteManager.InitSpriteManager();

	// Load the game text (points, coins, lives)
	LoadGameText();

	// Create a mario object
	c_Mario o_Player;
	o_Player.InitMario();

	// Telling the sprite manager that Mario exists
	o_SpriteManager.u8_EmptyOAMSlots[127] = 1;
	o_SpriteManager.u32_TileIndex += 4;
	o_SpriteManager.u8_PaletteIndex += 1;


	// Create text objects for collision debugging
	c_Text o_UpText;
	o_UpText.u8_Length = 1;

	c_Text o_DownText;
	o_DownText.u8_Length = 1;

	c_Text o_LleftText;
	o_LleftText.u8_Length = 1;

	c_Text o_RightText;
	o_RightText.u8_Length = 1;

	if (u8_CollisionDebug == 1)
	{
		o_SpriteManager.DrawText((char*)"T", o_UpText.u8_Index, 10, 30, 1);
		o_SpriteManager.DrawText((char*)"T", o_DownText.u8_Index, 10, 40, 1);
		o_SpriteManager.DrawText((char*)"T", o_LleftText.u8_Index, 0, 35, 1);
		o_SpriteManager.DrawText((char*)"T", o_RightText.u8_Index, 20, 35, 1);
	}

	// Make them equal to T
	char ch_UpChar = 'T';
	char ch_DownChar = 'T';
	char ch_LeftChar = 'T';
	char ch_RightChar = 'T';


	// Set the camera position so we can see mario and the world
	o_Player.o_WorldPos.f_y = Int2Fix(96);
	REG_BG_OFS[0].s16_y = Fix2Int(o_Player.o_WorldPos.f_y);

	// A time variable for the counter in the top right of the screen
	vu32 vu32_Time = 300;
	// Setting the timer
	Delay23(TM_SECOND);

	fixed f_bg1xMoveScale = FixDiv(Int2Fix(75), Int2Fix(100));
	fixed f_bg2xMoveScale = FixDiv(Int2Fix(5), Int2Fix(10));

	// 'bool' for main game loop
	u8 u8_InGame = 1;

	// Game loop
	while (u8_InGame == 1)
	{
		// Countdown the clock, 1 tick every second
		if (TM3D > 1 && u8_DisableTimer == 0)
		{
			// Using timers 1 and 2 as player animation uses timers 0 and 1
			StopDelay23();
			Delay23(TM_SECOND);

			// Decrementing time
			vu32_Time--;

			// Char array for the final number
			char ch_tm[3];
			// Hundreds
			ch_tm[0] = '0' + ((vu32_Time / 100) % 10);
			// Tens
			ch_tm[1] = '0' + ((vu32_Time / 10) % 10);
			// Digits
			ch_tm[2] = '0' + (vu32_Time % 10);

			// Update the timer text
			o_SpriteManager.UpdateText(ch_tm, o_Title_Time2, 3);

			// If the time runs out then execute order 66
			if (vu32_Time == 0)
			{
				o_Player.eo_State = deathAnim;
			}
		}

		// Update the player
		o_Player.Update();

		// Check if the player is dead so we can move to the menu
		if (o_Player.eo_State == dead)
		{
			u8_InGame = 0;
			this->eo_GameState = menu;
			break;
		}

		// Pause menu
		if (KeyHit(START))
		{
			// Options menu text
			c_Text o_Pause_Title;
			o_Pause_Title.u8_Length = 6;
			o_SpriteManager.DrawText((char*)"PAUSED", o_Pause_Title.u8_Index, 100, 70, o_Pause_Title.u8_Length);
			
			// Change the palette for the title text
			for (u32 u32_i = 0; u32_i < o_Pause_Title.u8_Length; ++u32_i)
				SetObjectPaletteBank(&MEM_OAM[o_Pause_Title.u8_Index[u32_i]], 14);

			c_Text o_Pause_Continue;
			o_Pause_Continue.u8_Length = 8;
			o_SpriteManager.DrawText((char*)"CONTINUE", o_Pause_Continue.u8_Index, 90, 100, o_Pause_Continue.u8_Length);
			
			// Change the palette for the Continue text
			for (u32 u32_i = 0; u32_i < o_Pause_Continue.u8_Length; ++u32_i)
				SetObjectPaletteBank(&MEM_OAM[o_Pause_Continue.u8_Index[u32_i]], 14);

			c_Text o_Pause_Exit;
			o_Pause_Exit.u8_Length = 4;
			o_SpriteManager.DrawText((char*)"EXIT", o_Pause_Exit.u8_Index, 105, 110, o_Pause_Exit.u8_Length);

			// 'Bool' for menu loop control
			u8 u8_InPause = 1;

			// Using another 'bool' for pause menu selection since we only have 2 options
			u8 u8_MenuSelection = 0;

			while (u8_InPause == 1)
			{
				// Get user input
				PollKeys();
				
				if (KeyHit(B))
				{
					u8_InPause = 0;
				}

				if (KeyHit(START) || KeyHit(A))
				{
					u8_InPause = 0;
					if (u8_MenuSelection == 1)
					{
						this->eo_GameState = menu;
						u8_InGame = 0;
					}
				}

				if (KeyHit(UP) || KeyHit(DOWN))
				{
					if (u8_MenuSelection == 1)
					{
						// Change the palette for the title text
						for (u32 u32_i = 0; u32_i < o_Pause_Continue.u8_Length; ++u32_i)
							SetObjectPaletteBank(&MEM_OAM[o_Pause_Continue.u8_Index[u32_i]], 14);
						for (u32 u32_i = 0; u32_i < o_Pause_Exit.u8_Length; ++u32_i)
							SetObjectPaletteBank(&MEM_OAM[o_Pause_Exit.u8_Index[u32_i]], 15);

						u8_MenuSelection = 0;
					}
					else if (u8_MenuSelection == 0)
					{
						// Change the palette for the title text
						for (u32 u32_i = 0; u32_i < o_Pause_Continue.u8_Length; ++u32_i)
							SetObjectPaletteBank(&MEM_OAM[o_Pause_Continue.u8_Index[u32_i]], 15);
						for (u32 u32_i = 0; u32_i < o_Pause_Exit.u8_Length; ++u32_i)
							SetObjectPaletteBank(&MEM_OAM[o_Pause_Exit.u8_Index[u32_i]], 14);

						u8_MenuSelection = 1;
					}
				}

				// Page flip and vsync
				Page_Flip();

				if (u8_VSYNC)
				{
					Vsync();
				}
			}

			// Delete the pause menu text
			o_SpriteManager.DestroySprite(o_Pause_Title.u8_Index, o_Pause_Title.u8_Length);
			o_SpriteManager.DestroySprite(o_Pause_Continue.u8_Index, o_Pause_Continue.u8_Length);
			o_SpriteManager.DestroySprite(o_Pause_Exit.u8_Index, o_Pause_Exit.u8_Length);
		}

		// Background paralaxing and movement
		// Making sure our yBackIndex cant go outside the sin look up table
		u16_yLutIndex++;
		u16_yLutIndex = u16_yLutIndex % sin_lut_Size;

		REG_BG_OFS[1].s16_y = Fix2Int(FixAdd(o_BG1Pos.f_y, LU_Sin(u16_yLutIndex)));
		REG_BG_OFS[2].s16_y = Fix2Int(FixAdd(o_BG2Pos.f_y, FixMul(LU_Cos(u16_yLutIndex), Int2Fix(2))));

		if (u8_Parallaxing == 1)
		{
			REG_BG_OFS[1].s16_x = Fix2Int(FixAdd(o_BG1Pos.f_x, FixMul(o_Player.o_WorldPos.f_x, f_bg1xMoveScale)));
			REG_BG_OFS[2].s16_x = Fix2Int(FixAdd(o_BG2Pos.f_x, FixMul(o_Player.o_WorldPos.f_x, f_bg2xMoveScale)));
		}

		// Stream in the map based on player movement
		UpdateMap(o_Player.o_WorldPos, o_Player.o_Vel);

		// Check the collision debug option
		if (u8_CollisionDebug == 1)
		{
			if (o_Player.u8_MoveUp == 1)
				ch_UpChar = 'F';
			else
				ch_UpChar = 'T';

			if (o_Player.u8_MoveDown == 1)
				ch_DownChar = 'F';
			else
				ch_DownChar = 'T';

			if (o_Player.u8_MoveLeft == 1)
				ch_LeftChar = 'F';
			else
				ch_LeftChar = 'T';

			if (o_Player.u8_MoveRight == 1)
				ch_RightChar = 'F';
			else
				ch_RightChar = 'T';

			o_SpriteManager.UpdateText(&ch_UpChar, o_UpText, 1);
			o_SpriteManager.UpdateText(&ch_DownChar, o_DownText, 1);
			o_SpriteManager.UpdateText(&ch_LeftChar, o_LleftText, 1);
			o_SpriteManager.UpdateText(&ch_RightChar, o_RightText, 1);
		}

		// Page flip and vsync
		Page_Flip();

		if (u8_VSYNC)
		{
			Vsync();
		}
	}
}

// Loading the game text that you see at the top of the game,
// this is required as a function as OAM init is called upon
// every transition of gamestates so we need to reload it at
// every stage
#include "textObject.h"
#include "SmallCoin_Sheet.h"
void c_GameManager::LoadGameText()
{
	// Loading of text and setting up of the sprite manager
	o_SpriteManager.u8_ActiveSpriteAttributes = 0;
	o_SpriteManager.u8_PaletteIndex = 1;

	o_Title_Mario1.u8_Length = 5;
	o_SpriteManager.DrawText((char*)"MARIO", o_Title_Mario1.u8_Index, 10, 0, o_Title_Mario1.u8_Length);;
	o_Title_Mario1.u8_Length = 6;
	o_SpriteManager.DrawText((char*)"000000", o_Title_Mario2.u8_Index, 8, 8, o_Title_Mario1.u8_Length);

	o_Title_Coins.u8_Length = 3;
	o_SpriteManager.DrawText((char*)"x00", o_Title_Coins.u8_Index, 100, 8, o_Title_Coins.u8_Length);

	o_SpriteManager.CreateSprite(SmallCoinPal, SmallCoinPalLen, SmallCoinTiles, SmallCoinTilesLen, 92, 8, A0_4BPP, A0_SQUARE, A1_SIZE_0);

	o_Title_World1.u8_Length = 5;
	o_SpriteManager.DrawText((char*)"WORLD", o_Title_World1.u8_Index, 140, 0, o_Title_World1.u8_Length);
	o_Title_World2.u8_Length = 3;
	o_SpriteManager.DrawText((char*)"1-1", o_Title_World2.u8_Index, 145, 8, o_Title_World2.u8_Length);

	o_Title_Time1.u8_Length = 4;
	o_SpriteManager.DrawText((char*)"TIME", o_Title_Time1.u8_Index, 200, 0, o_Title_Time1.u8_Length);
	o_Title_Time2.u8_Length = 3;
	o_SpriteManager.DrawText((char*)"300", o_Title_Time2.u8_Index, 200, 8, o_Title_Time2.u8_Length);
}