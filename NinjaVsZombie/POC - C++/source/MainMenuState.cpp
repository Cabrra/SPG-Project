/***************************************************************
|	File:		MainMenuState.cpp
|	Author:		Jagoba Macos
|	Course:		
|	Purpose:	MainMenuState class handles the main menu
***************************************************************/

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Color.h"

#include "TestTileEngineState.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "OptionState.h"
#include "HowToPlayState.h"
#include "CreditsState.h"

#include "AnimationManager.h"

#include <iostream>
#include <fstream>

#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>

#include "../TinyXML/tinyxml.h"


/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ MainMenuState* MainMenuState::GetInstance(void)
{
	static MainMenuState s_Instance;

	return &s_Instance;
}

/*virtual*/ void MainMenuState::Enter(void)
{
	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");
	m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Crest_the_Himalayas.xwm");

	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/testBackground.png");

	TiXmlDocument doc;

	if (doc.LoadFile("resource/XML/testvolume.xml") == false)
		return;
	else
	{
		TiXmlElement* root = doc.RootElement();

		if (root == nullptr)
			return;

		//load map/level info
		TiXmlElement* pMusic = root->FirstChildElement("Musice_Volume");

		int musicVolume, SoundEfVolume;

		pMusic->Attribute("Volume_level", &musicVolume);

		if (musicVolume < 0)
			return;
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, musicVolume);

		TiXmlElement* pSFX = root->FirstChildElement("SoundEf_Volume");


		pSFX->Attribute("Volume_level", &SoundEfVolume);

		if (SoundEfVolume < 0)
			return;
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, SoundEfVolume);
	}

	SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void MainMenuState::Exit(void)
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool MainMenuState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	
	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0,1))
		m_nCursor = 5;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0,0))
	{
		if (m_nCursor==0)
			Game::GetInstance()->ChangeState(GamePlayState::GetInstance());
		else if (m_nCursor == 1)
			Game::GetInstance()->ChangeState(GamePlayState::GetInstance());
		else if(m_nCursor == 2)
			Game::GetInstance()->ChangeState(HowToPlayState::GetInstance());
		else if (m_nCursor == 3)
			Game::GetInstance()->ChangeState(OptionState::GetInstance());
		else if (m_nCursor == 4)
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());
		else if (m_nCursor == 5)
		{
			AnimationManager::GetInstance()->Terminate();
			return false;
		}
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && time>.3)
	{
		if (m_nCursor == 0)
			m_nCursor = 5;
		else
			m_nCursor--;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && time>.3)
	{
		if (m_nCursor == 5)
			m_nCursor = 0;
		else
			m_nCursor++;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}

#if 0
	if (pInput->IsKeyPressed(SGD::Key::T))
	{
		Game::GetInstance()->ChangeState(TestTileEngineState::GetInstance());
	}
#endif

	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void MainMenuState::Update(float elapsedTime)
{
	time += elapsedTime;
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void MainMenuState::Render(void)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { 1.0f, 1.0f });
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];
	LoadString(HInstance, IDS_NINJA, buf, 255);

	pFont->Draw(buf, { (width - (40 * 12.0f)) / 2, 50 }, 1.0f, {});
	LoadString(HInstance, IDS_VS, buf, 255);
	pFont->Draw(buf, { (width - (20 * 12.0f)) / 2, 125 }, 0.7f, {});
	LoadString(HInstance, IDS_ZOMBIES, buf, 255);
	pFont->Draw(buf, { (width - (50 * 12.0f)) / 2, 150 }, 1.0f, {});

	SGD::Color		newGame, loadGame, howToPlay, options, credits, exit;
	newGame = loadGame = howToPlay = options = credits = exit = SGD::Color{};
	switch (m_nCursor)
	{
	case 0:
		newGame = { 0, 255, 0 };
		break;
	case 1:
		loadGame = { 0, 255, 0 };
		break;
	case 2:
		howToPlay = { 0, 255, 0 };
		break;
	case 3:
		options = { 0, 255, 0 };
		break;
	case 4:
		credits = { 0, 255, 0 };
		break;
	case 5:
		exit = { 0, 255, 0 }; 
		break;
	default:
		break;
	}
	
	LoadString(HInstance, IDS_NEWGAME, buf, 255);
	pFont->Draw(buf, { 50, 250 }, 0.7f, newGame);
	LoadString(HInstance, IDS_LOADGAME, buf, 255);
	pFont->Draw(buf, { 50, 300 }, 0.7f, loadGame);
	LoadString(HInstance, IDS_HOWTO, buf, 255);
	pFont->Draw(buf, { 50, 350 }, 0.7f, howToPlay);
	LoadString(HInstance, IDS_OPTIONS, buf, 255);
	pFont->Draw(buf, { 50, 400 }, 0.7f, options);
	LoadString(HInstance, IDS_CREDITS, buf, 255);
	pFont->Draw(buf, { 50, 450 }, 0.7f, credits);
	LoadString(HInstance, IDS_EXIT, buf, 255);
	pFont->Draw(buf, { 50, 500 }, 0.7f, exit);
}