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
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Color.h"

#include "TestTileEngineState.h"

#include "Game.h"
#include "Player.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "OptionState.h"
#include "HowToPlayState.h"
#include "CreditsState.h"
#include "LoadGameState.h"
#include "ShopState.h"

#include "AnimationManager.h"
#include "ParticleManager.h"
#include "Particle.h"
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
	ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::BLOSSOM_TOP, nullptr);
	ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::CLOUDS_LEFT, nullptr);
	ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::CLOUDS_RIGHT, nullptr);

	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");
	if (m_hMusic == SGD::INVALID_HANDLE)
		m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Crest_the_Himalayas.xwm");

	
	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/NVZ_menu.png");
	StudioPic = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/logo.jpg");
	LogoPic = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/LogoScreen.png");

	TiXmlDocument doc;
	string filepath = Game::GetInstance()->GetFilePath() + "volume.xml";

	if (doc.LoadFile(filepath.c_str()) == false)
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
	if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hMusic))
		SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);



	SGD::Rectangle myrect = SGD::Rectangle(SGD::Point{ 0, 0 }, SGD::Size{ 800, 512 });
	Game::GetInstance()->SetWorldCamera(myrect);
	Game::GetInstance()->SetWorldSpace({ 0, 0 });

	StudioTimer = 3.0f;
	LogoTimer = 3.0f;
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void MainMenuState::Exit(void)
{
	ParticleManager::GetParticleInstance()->RemoveAll(1); 
	
	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(StudioPic);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(LogoPic);
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool MainMenuState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	
	if (!logo && !studio)
	{
		if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
			m_nCursor = 5;

		if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
		{
			if (m_nCursor == 0)
			{
				LoadGameState::GetInstance()->SetIsLoaded(false);

				if (GamePlayState::GetInstance()->GetPlayer() != nullptr)
					GamePlayState::GetInstance()->GetPlayer()->Release();

				GamePlayState::GetInstance()->SetPlayer(nullptr);

				Game::GetInstance()->ChangeState(GamePlayState::GetInstance());
			}
			else if (m_nCursor == 1)
				Game::GetInstance()->ChangeState(LoadGameState::GetInstance());
			else if (m_nCursor == 2)
				Game::GetInstance()->ChangeState(HowToPlayState::GetInstance());
			else if (m_nCursor == 3)
				Game::GetInstance()->ChangeState(OptionState::GetInstance());
			else if (m_nCursor == 4)
			{
				Game::GetInstance()->ChangeState(CreditsState::GetInstance());
				SGD::AudioManager::GetInstance()->StopAudio(m_hMusic);
			}
			else if (m_nCursor == 5)
			{
				//AnimationManager::GetInstance()->Terminate();
				if (GamePlayState::GetInstance()->GetPlayer() != nullptr)
					GamePlayState::GetInstance()->GetPlayer()->Release();

				SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
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
		if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && time > .3)
		{
			if (m_nCursor == 5)
				m_nCursor = 0;
			else
				m_nCursor++;
			time = 0;
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}
	}
	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void MainMenuState::Update(float elapsedTime)
{
	if (logo)
		LogoTimer -= elapsedTime;
	else if (studio)
		StudioTimer -= elapsedTime;
	else
	{
		time += elapsedTime;
		ParticleManager::GetParticleInstance()->UpdateAll(elapsedTime);
	}

	if (StudioTimer <= 0.0f)
	{
		studio = false;
		logo = true;
	}
	if (LogoTimer <= 0.0f)
		logo = false;
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void MainMenuState::Render(void)
{
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];
	if (studio)
		SGD::GraphicsManager::GetInstance()->DrawTexture(StudioPic, { 200, 200 }, 0.0f, {}, {}, { .8f, .6f });
	else if (logo)
		SGD::GraphicsManager::GetInstance()->DrawTexture(LogoPic, { -50, 200 }, 0.0f, {}, {}, { .79f, .6f });
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, { 0,0 }, 0.0f, {}, {}, { .8f, .6f });

		ParticleManager::GetParticleInstance()->RenderAll();

		LoadString(HInstance, IDS_NINJA, buf, 255);
		pFont->Draw(buf, { 350, 50 }, 1.0f, {});
		LoadString(HInstance, IDS_VS, buf, 255);
		pFont->Draw(buf, { 400, 100 }, 0.7f, {});
		LoadString(HInstance, IDS_ZOMBIES, buf, 255);
		pFont->Draw(buf, { 300, 150 }, 1.0f, {});

		LoadString(HInstance, IDS_NINJA, buf, 255);
		pFont->Draw(buf, { 352, 52 }, 1.0f, { 0, 0, 0 });
		LoadString(HInstance, IDS_VS, buf, 255);
		pFont->Draw(buf, { 402, 102 }, 0.7f, { 0, 0, 0 });
		LoadString(HInstance, IDS_ZOMBIES, buf, 255);
		pFont->Draw(buf, { 302, 152 }, 1.0f, { 0, 0, 0 });

		SGD::Color		newGame, loadGame, howToPlay, options, credits, exit;
		newGame = loadGame = howToPlay = options = credits = exit = SGD::Color{};
		switch (m_nCursor)
		{
		case 0:
			newGame = { 255, 0, 0 };
			break;
		case 1:
			loadGame = { 255, 0, 0 };
			break;
		case 2:
			howToPlay = { 255, 0, 0 };
			break;
		case 3:
			options = { 255, 0, 0 };
			break;
		case 4:
			credits = { 255, 0, 0 };
			break;
		case 5:
			exit = { 255, 0, 0 };
			break;
		default:
			break;
		}

		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 210 }, 0.0f, {}, BASECOLOR, { 0.33f, 0.1f });
		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 210 }, 0.0f, {}, newGame, { 0.1f, 0.1f });
		LoadString(HInstance, IDS_NEWGAME, buf, 255);
		pFont->Draw(buf, { 95, 250 }, 0.7f, newGame);

		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 260 }, 0.0f, {}, BASECOLOR, { 0.35f, 0.1f });
		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 260 }, 0.0f, {}, loadGame, { 0.1f, 0.1f });
		LoadString(HInstance, IDS_LOADGAME, buf, 255);
		pFont->Draw(buf, { 95, 300 }, 0.7f, loadGame);

		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 310 }, 0.0f, {}, BASECOLOR, { 0.4f, 0.1f });
		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 310 }, 0.0f, {}, howToPlay, { 0.1f, 0.1f });
		LoadString(HInstance, IDS_HOWTO, buf, 255);
		pFont->Draw(buf, { 95, 350 }, 0.7f, howToPlay);

		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 360 }, 0.0f, {}, BASECOLOR, { 0.25f, 0.1f });
		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 360 }, 0.0f, {}, options, { 0.1f, 0.1f });
		LoadString(HInstance, IDS_OPTIONS, buf, 255);
		pFont->Draw(buf, { 95, 400 }, 0.7f, options);

		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 410 }, 0.0f, {}, BASECOLOR, { 0.25f, 0.1f });
		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 410 }, 0.0f, {}, credits, { 0.1f, 0.1f });
		LoadString(HInstance, IDS_CREDITS, buf, 255);
		pFont->Draw(buf, { 95, 450 }, 0.7f, credits);

		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 460 }, 0.0f, {}, BASECOLOR, { 0.18f, 0.1f });
		SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 460 }, 0.0f, {}, exit, { 0.1f, 0.1f });
		LoadString(HInstance, IDS_EXIT, buf, 255);
		pFont->Draw(buf, { 95, 500 }, 0.7f, exit);
	}
}