/***************************************************************
|	File:		OptionState.cpp
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	OptionState class handles the option menu
***************************************************************/

#include "OptionState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "HowToPlayState.h"
#include "CreditsState.h"

#include <sstream>
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
/*static*/ OptionState* OptionState::GetInstance(void)
{
	static OptionState s_Instance;

	return &s_Instance;
}

/*virtual*/ void OptionState::Enter(void)
{
	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");
	m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Dangerous_Steps.xwm");

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
		
		LANGID id = GetThreadUILanguage();
		if (id != LANG_SPANISH)
			m_english = true;
		else
			m_english = false;
	}
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void OptionState::Exit(void)
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);

	//create tinyXML Document
	TiXmlDocument doc;

	//create a tinyXML Declaration
	TiXmlDeclaration* pDecl = new TiXmlDeclaration{ "1.0", "utf-8", "" };

	//link the declaration to the doc
	doc.LinkEndChild(pDecl);

	//all the levels
	TiXmlElement* pRoot = new TiXmlElement{ "Options" };

	//link the root to the document
	doc.LinkEndChild(pRoot);

	TiXmlElement* pMusic = new TiXmlElement{ "Musice_Volume" };
	pRoot->LinkEndChild(pMusic);

	pMusic->SetAttribute("Volume_level", SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music));

	TiXmlElement* pSoundEf = new TiXmlElement{ "SoundEf_Volume" };
	pRoot->LinkEndChild(pSoundEf);

	pSoundEf->SetAttribute("Volume_level", SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects));

	doc.SaveFile("resource/XML/testvolume.xml");
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool OptionState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		m_nCursor = 6;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		if (m_nCursor == 6)
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		else if (m_nCursor == 5)
		{
			SetThreadPreferredUILanguages(MUI_RESET_FILTERS, nullptr, nullptr);
			if (m_english == true)
				SetThreadUILanguage(LANG_SPANISH);
			else
				SetThreadUILanguage(LANG_ENGLISH);

			LANGID id = GetThreadUILanguage();
			if (id == LANG_ENGLISH)
				m_english = true;
			else
				m_english = false;

		}
		else if (m_nCursor == 2)
		{
			int SFXlvl = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
			if (SFXlvl == 0)
			{
				SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, 100);
			}
			else if (SFXlvl > 0 && SFXlvl <= 100)
			{
				SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, 0);
			}
		}
		else if (m_nCursor == 3)
		{
			int Musiclvl = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
			if (Musiclvl == 0)
			{
				SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, 100);
				SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);
			}
			else if (Musiclvl > 0 && Musiclvl <= 100)
				SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, 0);
		}
		else if (m_nCursor == 4)
		{
			if ( m_IsFullScreen == true)
			{
				SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() }, false);
				Game::GetInstance()->SetFullscreen(false);
			}
			else
			{
				SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() }, true);
				Game::GetInstance()->SetFullscreen(true);
			}
		}
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && m_cursorMoveTimer>0.3f)
	{
		if (m_nCursor == 0)
			m_nCursor = 6;
		else
			m_nCursor--;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && m_cursorMoveTimer>0.3f)
	{
		if (m_nCursor == 6)
			m_nCursor = 0;
		else
			m_nCursor++;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Right) || pInput->IsDPadDown(0, SGD::DPad::Right) || pInput->GetLeftJoystick(0).x > 0) && m_cursorMoveTimer > 0.3f)
	{
		int SFX = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
		if (m_nCursor == 0 )
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hMusic);
			if (SFX < 100)
				SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, SFX + 5);
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}
		int Music = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
		if (m_nCursor == 1)
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hMusic);
			if (Music < 100)
				SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, Music + 5);
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);
		}
		m_cursorMoveTimer = 0;
	}

	if ((pInput->IsKeyDown(SGD::Key::Left) || pInput->IsDPadDown(0, SGD::DPad::Left) || pInput->GetLeftJoystick(0).x < 0) && m_cursorMoveTimer > 0.3f)
	{
		int SFX = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
		if (m_nCursor == 0 && SFX > 0)
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hMusic);
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, SFX - 5);
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}
		int Music = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
		if (m_nCursor == 1 && Music >0)
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hMusic);
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, Music - 5);
			SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);
		}
		m_cursorMoveTimer = 0;
	}

	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void OptionState::Update(float elapsedTime)
{
	m_cursorMoveTimer += elapsedTime;
	m_IsFullScreen = Game::GetInstance()->IsFullscreen();
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void OptionState::Render(void)
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	std::ostringstream out, musicvol;
	out << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
	musicvol << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);

	SFXVol = MUSICVol = SFXMute = MUSICMute = ExitOptions = LanguageOption = FullScreen = OnFS = OffFS = EnglishLang = SpanLang = { 255, 255, 255 };

	if (m_nCursor == 0)
		SFXVol = { 0, 0, 255 };
	else if (m_nCursor == 1)
		MUSICVol = { 0, 0, 255 };
	else if (m_nCursor == 2)
		SFXMute = { 0, 0, 255 };
	else if (m_nCursor == 3)
		MUSICMute = { 0, 0, 255 };
	else if (m_nCursor == 4)
		FullScreen = { 0, 0, 255 };
	else if (m_nCursor == 5)
		LanguageOption = { 0, 0, 255 };
	else if (m_nCursor == 6)
		ExitOptions = { 0, 0, 255 };


	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];
	LoadString(HInstance, IDS_OPTIONS, buf, 255);

	pFont->Draw(buf, { 100, 50 }, 1.0f, { 0, 0, 255 });

	LoadString(HInstance, IDS_SFXVOLUME, buf, 255);
	pFont->Draw(buf, { 50, 150 }, 0.3f, SFXVol);
	pFont->Draw(out.str().c_str(), { 550, 150 }, 0.2f, SFXVol);

	LoadString(HInstance, IDS_MUSICVOLUME, buf, 255);
	pFont->Draw(buf, { 50, 200 }, 0.3F, MUSICVol);
	pFont->Draw(musicvol.str().c_str(), { 550, 200 }, 0.2f, MUSICVol);

	LoadString(HInstance, IDS_SFXMUTEMAX, buf, 255);
	pFont->Draw(buf, { 50, 250 }, 0.3f, SFXMute);

	LoadString(HInstance, IDS_MUSICMUTEMAX, buf, 255);
	pFont->Draw(buf, { 50, 300 }, 0.3f, MUSICMute);

	LoadString(HInstance, IDS_FULLSCREEN, buf, 255);
	pFont->Draw(buf, { 50, 350 }, 0.3f, FullScreen);

	if (m_IsFullScreen)
		OffFS = { 0, 0, 255 };
	else
		OnFS = { 0, 0, 255 };

	LoadString(HInstance, IDS_ON, buf, 255);
	pFont->Draw(buf, { 500, 350 }, 0.3f, OnFS);

	LoadString(HInstance, IDS_OFF, buf, 255);
	pFont->Draw(buf, { 575, 350 }, 0.3f, OffFS);

	LoadString(HInstance, IDS_LANGUAGE, buf, 255);
	pFont->Draw(buf, { 50, 400 }, 0.3f, LanguageOption);

	if (m_english == true)
		EnglishLang = { 0, 0, 255 };
	else
		SpanLang = { 0, 0, 255 };

	LoadString(HInstance, IDS_ENGLISH, buf, 255);
	pFont->Draw(buf, { 325, 400 }, 0.3f, EnglishLang);

	LoadString(HInstance, IDS_SPANISH, buf, 255);
	pFont->Draw(buf, { 550, 400 }, 0.3f, SpanLang);

	LoadString(HInstance, IDS_EXIT, buf, 255);
	pFont->Draw(buf, { 50, 450 }, 0.3f, ExitOptions);
}