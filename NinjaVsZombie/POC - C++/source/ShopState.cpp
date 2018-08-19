/***************************************************************
|	File:		ShopState.cpp
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	ShopState class handles the shop menu
***************************************************************/

#include "ShopState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"

/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ ShopState* ShopState::GetInstance(void)
{
	static ShopState s_Instance;

	return &s_Instance;
}

/*virtual*/ void ShopState::Enter(void)
{

}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void ShopState::Exit(void)
{

}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool ShopState::Input(void)
{
	return false;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void ShopState::Update(float elapsedTime)
{

}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void ShopState::Render(void)
{

}