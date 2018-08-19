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
#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>


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
	m_hHook = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/hook.png");
	m_hPizza = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/pizzainstruct.png");
	m_hSword = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Katanainstru.png");
	m_hSmoke = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/SmokeBomb.png");


	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/NVZ_menu.png");

	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");

	m_nCursor = 2;
	m_pageCursor = 0;
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void HowToPlayState::Exit(void)
{

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hKeyboard);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hXbox);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hArcade);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHook);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPizza);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSword);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSmoke);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);

	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool HowToPlayState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	if (showArcade == false && showKeyboard == false && showXbox == false)
	{
		if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
			m_nCursor = 0; //Exit

		if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && m_cursorMoveTimer>0.3f)
		{
			switch (m_pageCursor)
			{
			case(0) :
			{
						if (m_nCursor == 0 || m_nCursor == 1)
							m_nCursor = 2;
						else if (m_nCursor == 2 || m_nCursor == 3 || m_nCursor == 4)
							m_nCursor = 0;
						break;
			}
			case(1) :
			{
						if (m_nCursor == 1)
							m_nCursor = 2;
						else if (m_nCursor == 2)
							m_nCursor = 1;
						break;
			}
			}
			m_cursorMoveTimer = 0;
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}
		if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && m_cursorMoveTimer > 0.3f)
		{
			switch (m_pageCursor)
			{
			case(0) :
			{
						if (m_nCursor == 0 || m_nCursor == 1)
							m_nCursor = 2;
						else if (m_nCursor == 2 || m_nCursor == 3 || m_nCursor == 4)
							m_nCursor = 0;

						break;
			}
			case(1) :
			{
						if (m_nCursor == 1)
							m_nCursor = 2;
						else if (m_nCursor == 2)
							m_nCursor = 1;
						break;
			}
			}
			m_cursorMoveTimer = 0;
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}

		if ((pInput->IsKeyDown(SGD::Key::Left) || pInput->IsDPadDown(0, SGD::DPad::Left) || pInput->GetLeftJoystick(0).x < 0) && m_cursorMoveTimer > 0.3f)
		{
			switch (m_pageCursor)
			{
			case(0) :
			{
						if (m_nCursor == 0)
							m_nCursor = 1;
						else if (m_nCursor == 1)
							m_nCursor = 0;
						if (m_nCursor == 2)
							m_nCursor = 4;
						else if (m_nCursor == 3)
							m_nCursor = 2;
						else if (m_nCursor == 4)
							m_nCursor = 3;
						break;
			}
			case(1) :
			{
						if (m_nCursor == 0)
							m_nCursor = 1;
						else if (m_nCursor == 1 || m_nCursor == 2)
							m_nCursor = 0;
						break;
			}
			case(2) :
			{
						if (m_nCursor == 0)
							m_nCursor = 1;
						else if (m_nCursor == 1)
							m_nCursor = 0;
						break;
			}
			}
			m_cursorMoveTimer = 0;
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}
		if ((pInput->IsKeyDown(SGD::Key::Right) || pInput->IsDPadDown(0, SGD::DPad::Right) || pInput->GetLeftJoystick(0).x > 0) && m_cursorMoveTimer > 0.3f)
		{
			switch (m_pageCursor)
			{
			case(0) :
			{
						if (m_nCursor == 0)
							m_nCursor = 1;
						else if (m_nCursor == 1)
							m_nCursor = 0;
						if (m_nCursor == 2)
							m_nCursor = 3;
						else if (m_nCursor == 3)
							m_nCursor = 4;
						else if (m_nCursor == 4)
							m_nCursor = 2;
						break;
			}
			case(1) :
			{
						if (m_nCursor == 0)
							m_nCursor = 1;
						else if (m_nCursor == 1 || m_nCursor == 2)
							m_nCursor = 0;
						break;
			}
			case(2) :
			{
						if (m_nCursor == 0)
							m_nCursor = 1;
						else if (m_nCursor == 1)
							m_nCursor = 0;
						break;
			}
			}
			m_cursorMoveTimer = 0;
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}
	}

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		switch (m_pageCursor)
		{
		case(0) :
		{
					if (m_nCursor == 0)
						Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
					if (m_nCursor == 1)
						m_pageCursor++;
					if (m_nCursor == 2)
						showKeyboard = !showKeyboard;
					if (m_nCursor == 3)
						showArcade = !showArcade;
					if (m_nCursor == 4)
						showXbox = !showXbox;

					break;
		}
		case(1) :
		{
					if (m_nCursor == 0)
						Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
					if (m_nCursor == 1)
						m_pageCursor++;
					if (m_nCursor == 2)
						m_pageCursor--;

					break;
		}
		case(2) :
		{
					if (m_nCursor == 0)
						Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
					if (m_nCursor == 1)
						m_pageCursor--;
					break;
		}
		}
	}

	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void HowToPlayState::Update(float elapsedTime)
{
	m_cursorMoveTimer += elapsedTime;
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void HowToPlayState::Render(void)
{
	SGD::GraphicsManager* pGraphic = SGD::GraphicsManager::GetInstance();
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { .8f, .6f });


	float width = Game::GetInstance()->GetScreenWidth();

	ExitCol = NextCol = BackCol = KeyCol = ArcaCol = XBOCol = { 178, 34, 34 };

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();
	char buf[255];
	LoadString(HInstance, IDS_HOWTO, buf, 255);
	pFont->Draw(buf, { 132, 77 }, 1.5f, { 255, 255, 255 });
	LoadString(HInstance, IDS_HOWTO, buf, 255);
	pFont->Draw(buf, { 130, 75 }, 1.5f, { 255, 0, 255 });

	switch (m_pageCursor)
	{
	case(0) :
	{
				LoadString(HInstance, IDS_LAYOUT, buf, 255);
				pFont->Draw(buf, { 175, 125 }, 1.0f, { 120, 225, 50 });

				if (m_nCursor == 0)
					ExitCol = { 255, 255, 255 };
				if (m_nCursor == 1)
					NextCol = { 255, 255, 255 };
				if (m_nCursor == 2)
					KeyCol = { 255, 255, 255 };
				if (m_nCursor == 3)
					ArcaCol = { 255, 255, 255 };
				if (m_nCursor == 4)
					XBOCol = { 255, 255, 255 };

				if (showArcade == false && showXbox == false)
				{
					LoadString(HInstance, IDS_KEYBOARD, buf, 255);
					pFont->Draw(buf, { 72, 192 }, 0.8f, { 255, 255, 255 });
					pFont->Draw(buf, { 70, 195 }, 0.8f, KeyCol);
					pGraphic->DrawRectangle({ 63, 243, 283, 358 }, KeyCol);
					if (showKeyboard == true)
						pGraphic->DrawTexture(m_hKeyboard, { 50, 135 }, 0.0f, {}, {}, { 0.7f, 0.75f });
					else
						pGraphic->DrawTexture(m_hKeyboard, { 70, 250 }, 0.0f, {}, {}, { 0.2f, 0.2f });
				}

				if (showKeyboard == false && showXbox == false)
				{
					LoadString(HInstance, IDS_ARCADE, buf, 255);
					pFont->Draw(buf, { 325, 197 }, 0.8f, { 255, 255, 255 });
					pFont->Draw(buf, { 323, 195 }, 0.8f, ArcaCol);
					pGraphic->DrawRectangle({ 318, 243, 487, 410 }, ArcaCol);
					if (showArcade == true)
						pGraphic->DrawTexture(m_hArcade, { 208, 135 }, 0.0f, {}, {}, { 0.9f, 0.8f });
					else
						pGraphic->DrawTexture(m_hArcade, { 325, 250 }, 0.0f, {}, {}, { 0.3f, 0.3f });
				}

				if (showArcade == false && showKeyboard == false)
				{
					LoadString(HInstance, IDS_XBOX, buf, 255);
					pFont->Draw(buf, { 552, 197 }, 0.8f, { 255, 255, 255 });
					pFont->Draw(buf, { 550, 195 }, 0.8f, XBOCol);
					pGraphic->DrawRectangle({ 543, 243, 710, 410 }, XBOCol);
					if (showXbox == true)
						pGraphic->DrawTexture(m_hXbox, { 158, 135 }, 0.0f, {}, {}, { 0.6f, 0.4f });
					else
						pGraphic->DrawTexture(m_hXbox, { 550, 250 }, 0.0f, {}, {}, { 0.15f, 0.15f });
				}

				LoadString(HInstance, IDS_EXIT, buf, 255);
				pFont->Draw(buf, { 52, 552 }, 0.7f, { 255, 255, 255 });
				pFont->Draw(buf, { 50, 550 }, 0.7f, ExitCol);
				LoadString(HInstance, IDS_NEXT, buf, 255);
				pFont->Draw(buf, { 552, 552 }, 0.7f, { 255, 255, 255 });
				pFont->Draw(buf, { 550, 550 }, 0.7f, NextCol);
				break;
	}
	case(1) : // items
	{
				  LoadString(HInstance, IDS_ITEMS, buf, 255);
				  pFont->Draw(buf, { 250, 125 }, 1.0f, { 120, 225, 50 });

				  if (m_nCursor == 0)
					  ExitCol = { 255, 255, 255 };
				  if (m_nCursor == 1)
					  NextCol = { 255, 255, 255 };
				  if (m_nCursor == 2)
					  BackCol = { 255, 255, 255 };

				  LoadString(HInstance, IDS_HOOK, buf, 255);
				  pFont->Draw(buf, { 42, 207 }, 0.8f, { 255, 255, 255 });
				  pFont->Draw(buf, { 40, 205 }, 0.8f, { 178, 34, 34 });
				  pGraphic->DrawTexture(m_hHook, { 50, 240 }, 0.0f, {}, {}, { 0.45f, 0.45f });

				  LoadString(HInstance, IDS_SWORD, buf, 255);
				  pFont->Draw(buf, { 202, 207 }, 0.8f, { 255, 255, 255 });
				  pFont->Draw(buf, { 200, 205 }, 0.8f, { 178, 34, 34 });
				  pGraphic->DrawTexture(m_hSword, { 210, 240 }, 0.0f, {}, {}, { 0.45f, 0.45f });

				  LoadString(HInstance, IDS_SMOKE, buf, 255);
				  pFont->Draw(buf, { 372, 167 }, 0.75f, { 255, 255, 255 });
				  pFont->Draw(buf, { 370, 165 }, 0.75f, { 178, 34, 34 });
				  pGraphic->DrawTexture(m_hSmoke, { 370, 240 }, 0.0f, {}, {}, { 0.125f, 0.25f });

				  LoadString(HInstance, IDS_PIZZA, buf, 255);
				  pFont->Draw(buf, { 567, 207 }, 0.8f, { 255, 255, 255 });
				  pFont->Draw(buf, { 565, 205 }, 0.8f, { 178, 34, 34 });
				  pGraphic->DrawTexture(m_hPizza, { 570, 240 }, 0.0f, {}, {}, { 0.45f, 0.45f });

				  LoadString(HInstance, IDS_EXIT, buf, 255);
				  pFont->Draw(buf, { 52, 552 }, 0.7f, { 255, 255, 255 });
				  pFont->Draw(buf, { 50, 550 }, 0.7f, ExitCol);
				  LoadString(HInstance, IDS_BACK, buf, 255);
				  pFont->Draw(buf, { 552, 502 }, 0.7f, { 255, 255, 255 });
				  pFont->Draw(buf, { 550, 500 }, 0.7f, BackCol);
				  LoadString(HInstance, IDS_NEXT, buf, 255);
				  pFont->Draw(buf, { 552, 552 }, 0.7f, { 255, 255, 255 });
				  pFont->Draw(buf, { 550, 550 }, 0.7f, NextCol);
				  break;
	}

	case(2) : // Instructions
	{
				  if (m_nCursor == 0)
					  ExitCol = { 255, 255, 255 };
				  if (m_nCursor == 1)
					  BackCol = { 255, 255, 255 };

				  LoadString(HInstance, IDS_INSTRUCTIONS, buf, 255);
				  pFont->Draw(buf, { 190, 125 }, 1.0f, { 120, 225, 50 });

				  LoadString(HInstance, IDS_INSTRUCLINE, buf, 255);
				  pFont->Draw(buf, { 47, 162 }, 0.8f, { 0, 0, 0 });
				  pFont->Draw(buf, { 45, 160 }, 0.8f, { 255, 255, 255 });

				  LoadString(HInstance, IDS_ITEMLINE, buf, 255);
				  pFont->Draw(buf, { 47, 317 }, 0.8f, { 0, 0, 0 });
				  pFont->Draw(buf, { 45, 315 }, 0.8f, { 255, 255, 255 });

				  LoadString(HInstance, IDS_EXIT, buf, 255);
				  pFont->Draw(buf, { 52, 552 }, 0.7f, { 255, 255, 255 });
				  pFont->Draw(buf, { 50, 550 }, 0.7f, ExitCol);

				  LoadString(HInstance, IDS_BACK, buf, 255);
				  pFont->Draw(buf, { 552, 552 }, 0.7f, { 255, 255, 255 });
				  pFont->Draw(buf, { 550, 550 }, 0.7f, BackCol);
				  break;
	}
	}
}