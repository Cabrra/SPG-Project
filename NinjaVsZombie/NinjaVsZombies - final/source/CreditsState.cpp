/***************************************************************
|	File:		CreditsState.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreditsState shows credits of everyone who helped
|				with the creation of the game
***************************************************************/

#include "CreditsState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "../SGD Wrappers/SGD_String.h"

#include <Windows.h>
#include "../resource2.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "OptionState.h"
#include "CreditsState.h"



#include <iostream>
#include <fstream>
#include <atlstr.h>

/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState s_Instance;

	return &s_Instance;
}

/*virtual*/ void CreditsState::Enter(void)
{
	m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Through_the_Ages.xwm");
	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/NVZ_menu.png");

	SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);
	time = 0;
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void CreditsState::Exit(void)
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool CreditsState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void CreditsState::Update(float elapsedTime)
{
	if (time<=2000.0f)
		time += 50 * elapsedTime;
	
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void CreditsState::Render(void)
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { .8f, .6f });


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();
	float height = Game::GetInstance()->GetScreenHeight();

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];
	LoadString(HInstance, IDS_CREDITS, buf, 255);
	pFont->Draw(buf, { 300, height - time }, 1.0f, { 255, 0, 0 });

	LoadString(HInstance, IDS_PRODUCERS, buf, 255);
	pFont->Draw(buf, { 25, (height - time) + 150 }, 0.8f, { 0, 0, 255 });

	LoadString(HInstance, IDS_EXECUTIVEPRODUCER, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 200 }, 0.5f, {});

	LoadString(HInstance, IDS_ASSOCIATEPREODUCER, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 250 }, 0.5f, {});

	LoadString(HInstance, IDS_ARTIST, buf, 255);
	pFont->Draw(buf, { 25, (height - time) + 350 }, 0.8f, { 0, 255, 0 });

	LoadString(HInstance, IDS_CARISFRAZIER, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 400 }, 0.5f, {});

	LoadString(HInstance, IDS_GREGORYREY, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 450 }, 0.5f, {});

	LoadString(HInstance, IDS_DEVELOPERS, buf, 255);
	pFont->Draw(buf, { 25, (height - time) + 550 }, 0.8f, {});
 
	LoadString(HInstance, IDS_COREY, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 600 }, 0.5f, {});

	LoadString(HInstance, IDS_JAGOBA, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 650 }, 0.5f, {});

	LoadString(HInstance, IDS_DESMON, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 700 }, 0.5f, {});

	LoadString(HInstance, IDS_WILLIAM, buf, 255);
	pFont->Draw(buf, { 50, (height - time) + 750 }, 0.5f, {});

	LoadString(HInstance, IDS_TESTERS, buf, 255);
	pFont->Draw(buf, { 25, (height - time) + 850 }, 0.8f, {});

	pFont->Draw("Nicholas Garcia", { 50, (height - time) + 900 }, 0.5f, {255,255,0});

	pFont->Draw("James Christner", { 50, (height - time) + 950 }, 0.5f, { 200, 139, 69, 19 });

	pFont->Draw("Grant Overwein", { 50, (height - time) + 1000 }, 0.5f, { 200, 107, 142, 35 });

	LoadString(HInstance, IDS_THANKS, buf, 255);
	pFont->Draw(buf, { 25, (height - time) + 1100 }, 0.8f, {220, 20, 60});
	
	pFont->Draw("Chuck Rainey", { 50, (height - time) + 1150 }, 0.5f, { 255, 255, 255 });

	pFont->Draw("Asier Barrio", { 50, (height - time) + 1200 }, 0.5f, { 255, 255, 255 });

	pFont->Draw("Jose Maeso", { 50, (height - time) + 1250 }, 0.5f, { 255, 255, 255 });


	LoadString(HInstance, IDS_PRESSSTART, buf, 255);
	pFont->Draw(buf, { 100, (height - time) + 1500 }, 0.5f, {});

}