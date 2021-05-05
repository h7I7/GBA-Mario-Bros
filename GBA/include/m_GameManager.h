////////////////////////////////////////////////////////////
//	File Name:		m_GameManager.h
//	Description:	A header file for the enums, defines,
//					classes and functionality needed for
//					managing the game
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#ifndef _M_GAMEMANAGER_H_
#define _M_GAMEMANAGER_H_

#include "marioObject.h"
#include "m_SpriteManager.h"
#include "textObject.h"

// An enim for the game states
typedef enum e_RunStates
{
	intro,
	menu,
	game,
	gameover
}e_Runstates;

// an enum for the main menu states
typedef enum e_MenuStates
{
	startgame = 0,
	options,
	credits
}e_MenuStates;

// An enum for the options menu states
typedef enum e_OptionsMenuStates
{
	Option_Vsync = 0,
	Option_Parallaxing,
	Option_DisablerTimer,
	Option_ColDebug
}e_OptionsMenuStates;

// The game manager class, runs and manages the whole game
class c_GameManager
{
public:

	//-------------------------------------------------
	//	MEMBER VARIABLES

	//--------------------------------------------------
	// Mario
	c_Mario o_Player;

	//--------------------------------------------------
	// Sprite Manager
	SpriteManager o_SpriteManager;

	//--------------------------------------------------
	// GameState
	e_RunStates eo_GameState;

	//--------------------------------------------------
	// MenuState
	e_MenuStates eo_MenuSelection;

	//--------------------------------------------------
	// Options MenuState
	e_OptionsMenuStates eo_OptionsMenuSelection;

	//--------------------------------------------------
	// Background Positions
	//	0
	s_Vector2	o_BG0Pos;
	//	1
	s_Vector2	o_BG1Pos;
	//	2
	s_Vector2	o_BG2Pos;
	//	3
	s_Vector2	o_BG3Pos;
	// Look up table index for cloud bobbing
	u16			u16_yLutIndex;

	// Text objects
	c_Text		o_Title_Mario1;
	c_Text		o_Title_Mario2;

	c_Text		o_Title_Coins;

	c_Text		o_Title_World1;
	c_Text		o_Title_World2;

	c_Text		o_Title_Time1;
	c_Text		o_Title_Time2;

	//--------------------------------------------------
	// Game Options
	u8 u8_VSYNC;
	u8 u8_Parallaxing;
	u8 u8_DisableTimer;
	u8 u8_CollisionDebug;

	//--------------------------------------------------
	//	FUNCTIONS

	//--------------------------------------------------
	//	Initialisation
	void InitGameManager();

	// Run the main game loop
	void Run();

	//--------------------------------------------------
	//	Game intro
	void Intro();
	void Menu();

	//--------------------------------------------------
	//	Levels
	void Level1();

	//--------------------------------------------------
	//	Functions
	void LoadGameText();
};

#endif //_M_GAMEMANAGER_H_