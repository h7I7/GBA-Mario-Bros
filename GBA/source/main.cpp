////////////////////////////////////////////////////////////
//	File Name:		main.cpp
//	Description:	The entry point for the application
//	Author:			Zack Raeburn
////////////////////////////////////////////////////////////

#include "Intellisense.h"
#include "m_GameManager.h"

int main()
{
	// Create a new game manager object
	c_GameManager manager;

	// Run the game
	manager.Run();

	return 0;
}