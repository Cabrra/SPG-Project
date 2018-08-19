/***************************************************************
|	File:		HowToPlayState.cpp
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	HowToPlayState class handles the how to play menu
***************************************************************/

#include "HowToPlayState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "OptionState.h"
#include "CreditsState.h"

#include <iostream>
#include <fstream>


/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ HowToPlayState* HowToPlayState::GetInstance(void)
{
	static HowToPlayState s_Instance;

	return &s_Instance;
}

/*virtual*/ void HowToPlayState::Enter(void)
{
	//key layout
	m_hKeyboard = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/keyboard.jpg");
	m_hXbox = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/xbox.jpg");
	m_hArcade = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Arcade_controls.png");

	//items
	m_hHook = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/hookInstru.png");
	m_hPizza = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/pizzainstruct.png");
	m_hSword = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Katanainstru.png");
	m_hSmoke = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/SmokeBombInstruct.png");
	
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void HowToPlayState::Exit(void)
{

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hKeyboard);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hXbox);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hArcade);

		//items
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHook); 
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPizza);  
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSword);
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSmoke);
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool HowToPlayState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	
	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void HowToPlayState::Update(float elapsedTime)
{

}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void HowToPlayState::Render(void)
{
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	pFont->Draw("HOW TO PLAY", { 100, 50 }, 0.5f, { 255, 0, 255 });

	pFont->Draw("EXIT", { 50, 350 }, 0.3f, {255,255,0});
}