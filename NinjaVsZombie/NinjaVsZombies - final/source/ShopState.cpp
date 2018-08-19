#include "ShopState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include "BitmapFont.h"

#include "Game.h"
#include "Player.h"
#include "MainMenuState.h"
#include "GamePlayState.h"

#include <sstream>

#include "../resource2.h"

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
	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/newstore.png");
	m_Master = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/RoshiSitting.png");
	m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/music/Ninja_Master.xwm");
	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");
	m_Purchase = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/cash-register-purchase.wav");

	m_Stats = m_Items = false;
	m_nCursor = 0;
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void ShopState::Exit(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Master);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_Purchase);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool ShopState::Input(void)
{
	if (m_Stats)
		StatsInput();
	else if (m_Items)
		ItemsInput();
	else
		MainInput();

	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void ShopState::Update(float elapsedTime)
{
	time += elapsedTime;
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void ShopState::Render(void)
{
	if (m_Stats)
		StatsMenu();
	else if (m_Items)
		ItemsMenu();
	else
		MainShopMenu();
}

void ShopState::MainInput()
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		m_nCursor = 3;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		if (m_nCursor == 0)
			m_Stats = true;
		else if (m_nCursor == 1)
		{
			m_Items = true;
			m_nCursor = 0;
		}
		else if (m_nCursor == 2 && GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25 && GamePlayState::GetInstance()->GetPlayer()->GetNumSmokeBombs() < 10)
		{
			GamePlayState::GetInstance()->GetPlayer()->SetDAQ(GamePlayState::GetInstance()->GetPlayer()->GetDAQ() - 25);
			GamePlayState::GetInstance()->GetPlayer()->SetNumSmokeBombs(GamePlayState::GetInstance()->GetPlayer()->GetNumSmokeBombs() + 5);
			SGD::AudioManager::GetInstance()->PlayAudio(m_Purchase);
		}
		else if (m_nCursor == 3)
			Game::GetInstance()->ChangeState(GamePlayState::GetInstance());
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && time>.3)
	{
		if (m_nCursor == 0)
			m_nCursor = 3;
		else
			m_nCursor--;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && time>.3)
	{
		if (m_nCursor == 3)
			m_nCursor = 0;
		else
			m_nCursor++;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
}
	 
void ShopState::StatsInput()
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		m_nCursor = 3;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		if (m_nCursor == 0 && (int)GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25 * (GamePlayState::GetInstance()->GetPlayer()->GetStrength() / 5)
			&& GamePlayState::GetInstance()->GetPlayer()->GetStrength() < 30)
		{
			GamePlayState::GetInstance()->GetPlayer()->SetDAQ(GamePlayState::GetInstance()->GetPlayer()->GetDAQ() - 25 * GamePlayState::GetInstance()->GetPlayer()->GetStrength() / 5);
			GamePlayState::GetInstance()->GetPlayer()->SetStrength(GamePlayState::GetInstance()->GetPlayer()->GetStrength() + 5);
			SGD::AudioManager::GetInstance()->PlayAudio(m_Purchase);
		}
		else if (m_nCursor == 1 && (int)GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25 * GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() / 100
			&& GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() < 300)
		{
			GamePlayState::GetInstance()->GetPlayer()->SetDAQ(GamePlayState::GetInstance()->GetPlayer()->GetDAQ() - 25 * GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() / 100);
			GamePlayState::GetInstance()->GetPlayer()->SetMoveSpeed(GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() + 50);
			GamePlayState::GetInstance()->GetPlayer()->SetPrevSpeed(GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() + 50);
			SGD::AudioManager::GetInstance()->PlayAudio(m_Purchase);
		}
		else if (m_nCursor == 2 && (int)GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25 * GamePlayState::GetInstance()->GetPlayer()->GetMaxHP() / 100
			&& GamePlayState::GetInstance()->GetPlayer()->GetMaxHP() < 300)
		{
			GamePlayState::GetInstance()->GetPlayer()->SetDAQ(GamePlayState::GetInstance()->GetPlayer()->GetDAQ() - 25 * GamePlayState::GetInstance()->GetPlayer()->GetMaxHP() / 100);
			GamePlayState::GetInstance()->GetPlayer()->SetMaxHP(GamePlayState::GetInstance()->GetPlayer()->GetMaxHP() + 25);
			SGD::AudioManager::GetInstance()->PlayAudio(m_Purchase);
		}
		else if (m_nCursor == 3)
		{
			m_Stats = false;
			m_nCursor = 0;
		}
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && time>.3)
	{
		if (m_nCursor == 0)
			m_nCursor = 3;
		else
			m_nCursor--;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && time>.3)
	{
		if (m_nCursor == 3)
			m_nCursor = 0;
		else
			m_nCursor++;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
}

void ShopState::ItemsInput()
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		m_nCursor = 2;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		if (m_nCursor == 0 && (int)GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 50 * GamePlayState::GetInstance()->GetPlayer()->GetSwordLevel()
			&& GamePlayState::GetInstance()->GetPlayer()->GetSwordLevel() < 10)
		{
			GamePlayState::GetInstance()->GetPlayer()->SetDAQ(GamePlayState::GetInstance()->GetPlayer()->GetDAQ() - 50 * GamePlayState::GetInstance()->GetPlayer()->GetSwordLevel());
			GamePlayState::GetInstance()->GetPlayer()->SetSwordLevel(GamePlayState::GetInstance()->GetPlayer()->GetSwordLevel() + 1);
			SGD::AudioManager::GetInstance()->PlayAudio(m_Purchase);
		}
		else if (m_nCursor == 1 && (int)GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 50 * GamePlayState::GetInstance()->GetPlayer()->GetHookLevel()
			&& GamePlayState::GetInstance()->GetPlayer()->GetHookLevel() < 3)
		{
			GamePlayState::GetInstance()->GetPlayer()->SetDAQ(GamePlayState::GetInstance()->GetPlayer()->GetDAQ() - 50 * GamePlayState::GetInstance()->GetPlayer()->GetHookLevel());
			GamePlayState::GetInstance()->GetPlayer()->SetHookLevel(GamePlayState::GetInstance()->GetPlayer()->GetHookLevel() + 1);
			SGD::AudioManager::GetInstance()->PlayAudio(m_Purchase);
		}
		else if (m_nCursor == 2)
		{
			m_Items = false;
			m_nCursor = 0;
		}
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && time>.3)
	{
		if (m_nCursor == 0)
			m_nCursor = 2;
		else
			m_nCursor--;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && time>.3)
	{
		if (m_nCursor == 2)
			m_nCursor = 0;
		else
			m_nCursor++;
		time = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
}

void ShopState::MainShopMenu()
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, {}, 0.0f, {}, {}, { 1.0f, 1.0f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_Master, {200,350}, 0.0f, {}, {}, { 1.0f, 1.0f });

	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	std::ostringstream SmokeBombs, DAQ;
	if (GamePlayState::GetInstance()->GetPlayer()->GetNumSmokeBombs() < 10)
		SmokeBombs << "$" << 25;
	else
		SmokeBombs << "MAX";

	DAQ << "DAQ " << GamePlayState::GetInstance()->GetPlayer()->GetDAQ();

	char buf[255];
	LoadString(HInstance, IDS_STORE, buf, 255);
	pFont->Draw(buf, { 300, 50 }, 1.0f, {255,0,0});


	SGD::Color stats, items, smokebombs, SMOKEcost, exit;
	stats = items = smokebombs = SMOKEcost = exit = SGD::Color{};
	switch (m_nCursor)
	{
	case 0:
		stats = { 0, 255, 0 };
		break;
	case 1:
		items = { 0, 255, 0 };
		break;
	case 2:
	{
			  if (GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25)
				  SMOKEcost = { 0, 255, 0 };
			  else
				  SMOKEcost = { 255, 0, 0 };

			  smokebombs = { 0, 255, 0 };
	}
		break;
	case 3:
		exit = { 0, 255, 0 };
		break;
	default:
		break;
	}


	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 220 }, 0.0f, {}, BASECOLOR, { 0.3f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 220 }, 0.0f, {}, stats, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_UPGRADESTATS, buf, 255);
	pFont->Draw(buf, { 55, 250 }, 0.5f, stats);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 270 }, 0.0f, {}, BASECOLOR, { 0.3f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 270 }, 0.0f, {}, items, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_UPGRADEITEMS, buf, 255);
	pFont->Draw(buf, { 55, 300 }, 0.5f, items);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 320 }, 0.0f, {}, BASECOLOR, { 0.4f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 320 }, 0.0f, {}, smokebombs, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_BUYSMOKEBOMBS, buf, 255);
	pFont->Draw(buf, { 55, 350 }, 0.5f, smokebombs);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 545, 310 }, 0.0f, {}, BASECOLOR, { 0.12f, 0.12f });
	pFont->Draw(SmokeBombs.str().c_str(), { 550, 350 }, 0.5f, SMOKEcost);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 590, 440 }, 0.0f, {}, BASECOLOR, { 0.35f, 0.2f });
	pFont->Draw(DAQ.str().c_str(), { 600, 500 }, 0.5f, {});

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 460 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 460 }, 0.0f, {}, exit, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_EXIT, buf, 255);
	pFont->Draw(buf, { 55, 500 }, 0.5f, exit);
}

void ShopState::StatsMenu()
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, {}, 0.0f, {}, {}, { 1.0f, 1.0f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_Master, { 200, 350 }, 0.0f, {}, {}, { 1.0f, 1.0f });

	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	std::ostringstream strength, move, health, DAQ;

	DAQ << "DAQ " << GamePlayState::GetInstance()->GetPlayer()->GetDAQ();

	if (GamePlayState::GetInstance()->GetPlayer()->GetStrength() < 30)
		strength << "$" << 25 * GamePlayState::GetInstance()->GetPlayer()->GetStrength() / 5;
	else
		strength << "MAX";

	if (GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() < 300)
		move << "$" << 25 * GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() / 100;
	else
		move << "MAX";

	if (GamePlayState::GetInstance()->GetPlayer()->GetMaxHP() < 300)
		health << "$" << 25 * GamePlayState::GetInstance()->GetPlayer()->GetMaxHP() / 100;
	else
		health << "MAX";

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];
	LoadString(HInstance, IDS_STORE, buf, 255);
	pFont->Draw(buf, { 300, 50 }, 1.0f, { 255, 0, 0 });


	SGD::Color		STR, MOVE, HEALTH, exit, STRcost, MOVEcost, HEALTHcost;
	STR = MOVE = HEALTH = exit = STRcost = MOVEcost = HEALTHcost = SGD::Color{};
	switch (m_nCursor)
	{
	case 0:
	{	
			  if (GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25 * GamePlayState::GetInstance()->GetPlayer()->GetStrength() / 5)
				  STRcost = { 0, 255, 0 };
			  else
				  STRcost = { 255, 0, 0 };

			  STR = { 255, 0, 0 };
	}
		break;
	case 1:
	{
			  if (GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25 * GamePlayState::GetInstance()->GetPlayer()->GetMoveSpeed() / 100)
				  MOVEcost = { 0, 255, 0 };
			  else
				  MOVEcost = { 255, 0, 0 };

			  MOVE = { 255, 0, 0 };
	}
		break;
	case 2:
	{
			  if (GamePlayState::GetInstance()->GetPlayer()->GetDAQ() >= 25 * GamePlayState::GetInstance()->GetPlayer()->GetMaxHP() / 100)
				  HEALTHcost = { 0, 255, 0 };
			  else
				  HEALTHcost = { 255, 0, 0 };

			  HEALTH = { 255, 0, 0 };
	}
		break;
	case 3:
		exit = { 255, 0, 0 };
		break;
	default:
		break;
	}


	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 220 }, 0.0f, {}, BASECOLOR, { 0.34f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 220 }, 0.0f, {}, STR, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_UPGRADESTRENGTH, buf, 255);
	pFont->Draw(buf, { 55, 250 }, 0.5f, STR);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 650, 210 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.13f });
	pFont->Draw(strength.str().c_str(), { 650, 250 }, 0.5f, STRcost);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 270 }, 0.0f, {}, BASECOLOR, { 0.4f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 270 }, 0.0f, {}, MOVE, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_UPGRADEMOVESPEED, buf, 255);
	pFont->Draw(buf, { 55, 300 }, 0.5f, MOVE);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 650, 260 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.13f });
	pFont->Draw(move.str().c_str(), { 650, 300 }, 0.5f, MOVEcost);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 320 }, 0.0f, {}, BASECOLOR, { 0.51f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 320 }, 0.0f, {}, HEALTH, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_UPGRADEMAXHP, buf, 255);
	pFont->Draw(buf, { 55, 350 }, 0.5f, HEALTH);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 650, 310 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.13f });
	pFont->Draw(health.str().c_str(), { 650, 350 }, 0.5f, HEALTHcost);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 590, 440 }, 0.0f, {}, BASECOLOR, { 0.35f, 0.2f });
	pFont->Draw(DAQ.str().c_str(), { 600, 500 }, 0.5f, {});

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 460 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 460 }, 0.0f, {}, exit, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_EXIT, buf, 255);
	pFont->Draw(buf, { 55, 500 }, 0.5f, exit);
}

void ShopState::ItemsMenu()
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, {}, 0.0f, {}, {}, { 1.0f, 1.0f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_Master, { 200, 350 }, 0.0f, {}, {}, { 1.0f, 1.0f });

	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	float width = Game::GetInstance()->GetScreenWidth();

	std::ostringstream  sword, hook, DAQ;
	if (GamePlayState::GetInstance()->GetPlayer()->GetSwordLevel() < 10)
		sword << "$" << 50 * GamePlayState::GetInstance()->GetPlayer()->GetSwordLevel();
	else
		sword << "MAX";
	
	if (GamePlayState::GetInstance()->GetPlayer()->GetHookLevel() < 3)
		hook << "$" << 50 * GamePlayState::GetInstance()->GetPlayer()->GetHookLevel();
	else
		hook << "MAX";
	
	DAQ << "DAQ " << GamePlayState::GetInstance()->GetPlayer()->GetDAQ();

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];
	LoadString(HInstance, IDS_STORE, buf, 255);
	pFont->Draw(buf, { 300, 50 }, 1.0f, { 255, 0, 0 });


	SGD::Color		SWORD, Swordcost, HOOK, Hookcost, exit;
	SWORD = HOOK = exit = SGD::Color{};
	switch (m_nCursor)
	{
	case 0:
	{
			  if (50 * GamePlayState::GetInstance()->GetPlayer()->GetSwordLevel() <= GamePlayState::GetInstance()->GetPlayer()->GetDAQ())
				  Swordcost = { 0, 255, 0 };
			  else
				  Swordcost = { 255, 0, 0 };

			  SWORD = { 0, 255, 0 };
	}
		break;
	case 1:
	{
			  if (50 * GamePlayState::GetInstance()->GetPlayer()->GetHookLevel() <= GamePlayState::GetInstance()->GetPlayer()->GetDAQ())
				  Hookcost = { 0, 255, 0 };
			  else
				  Hookcost = { 255, 0, 0 };

			  HOOK = { 0, 255, 0 };
	}
		break;
	case 2:
		exit = { 0, 255, 0 };
		break;
	default:
		break;
	}


	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 170 }, 0.0f, {}, BASECOLOR, { 0.3f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 170 }, 0.0f, {}, SWORD, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_UPGRADESWORD, buf, 255);
	pFont->Draw(buf, { 55, 200 }, 0.5f, SWORD);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 650, 160 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.13f });
	pFont->Draw(sword.str().c_str(), { 650, 200 }, 0.5f, Swordcost);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 270 }, 0.0f, {}, BASECOLOR, { 0.3f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 270 }, 0.0f, {}, HOOK, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_UPGRADEHOOK, buf, 255);
	pFont->Draw(buf, { 55, 300 }, 0.5f, HOOK);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 650, 260 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.13f });
	pFont->Draw(hook.str().c_str(), { 650, 300 }, 0.5f, Hookcost);


	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 590, 440 }, 0.0f, {}, BASECOLOR, { 0.35f, 0.2f });
	pFont->Draw(DAQ.str().c_str(), { 600, 500 }, 0.5f, {});

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 20, 460 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 10, 460 }, 0.0f, {}, exit, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_EXIT, buf, 255);
	pFont->Draw(buf, { 55, 500 }, 0.5f, exit);
}