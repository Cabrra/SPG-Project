#include "LoadGameState.h"

#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "MainMenuState.h"
#include "GamePlayState.h"

#include "Game.h"
#include "Player.h"
#include "BitmapFont.h"

#include "../resource2.h"

LoadGameState* LoadGameState::GetInstance(void)
{
	static LoadGameState s_Instance;

	return &s_Instance;
}

void	LoadGameState::Enter(void)
{
	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/NVZ_menu.png");
	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");

	Slot1 = new Player();
	Slot2 = new Player();
	Slot3 = new Player();

	LoadSlot(Slot1, 1);
	LoadSlot(Slot2, 2);
	LoadSlot(Slot3, 3);

	m_Slot = 0;
}

void	LoadGameState::Exit(void)
{
	Slot1->Release();
	Slot2->Release();
	Slot3->Release();

	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
}

bool	LoadGameState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		m_nCursor = 3;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		m_IsGameLoaded = true;
		if (m_nCursor == 0 && Slot1->GetGameLevel() != 0)
		{
			m_Slot = 1;	
			Game::GetInstance()->ChangeState(GamePlayState::GetInstance());
		}
		else if (m_nCursor == 1 && Slot2->GetGameLevel() != 0)
		{
			m_Slot = 2;
			Game::GetInstance()->ChangeState(GamePlayState::GetInstance());
		}
		else if (m_nCursor == 2 && Slot3->GetGameLevel() != 0)
		{
			m_Slot = 3;
			Game::GetInstance()->ChangeState(GamePlayState::GetInstance());
		}
		else if (m_nCursor == 3)
		{
			m_IsGameLoaded = false;
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		}
		else
			m_IsGameLoaded = false;
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

	return true;
}

void	LoadGameState::Update(float elapsedTime)
{
	time += elapsedTime;
}

void	LoadGameState::Render(void)
{
	SGD::Rectangle rect;

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { .8f, .6f });

	SGD::Color slot1, slot2, slot3, MainMenu;
	slot1 = slot2 = slot3 = MainMenu = { 255, 255, 255 };

	SGD::OStringStream gamelevel, strength, movespeed, hooklevel, swordlevel, numsb, daq;

	if (m_nCursor == 0)
		slot1 = { 0, 0, 255 };
	else if (m_nCursor == 1)
		slot2 = { 0, 0, 255 };
	else if (m_nCursor == 2)
		slot3 = { 0, 0, 255 };
	else if (m_nCursor == 3)
		MainMenu = { 0, 0, 255 };

	switch (m_nCursor)
	{
	case 0:
	{
			  if (Slot1->GetGameLevel() > 0)
			  {
				  rect = { 400, 200, 750, 550 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_GAMELEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
				  gamelevel << Slot1->GetGameLevel();
				  pFont->Draw(gamelevel.str().c_str(), { 700, 225 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_STRENGTH, buf, 255);
				  pFont->Draw(buf, { 410, 270 }, 0.4f, { 255, 0, 0 });
				  strength << Slot1->GetStrength();
				  pFont->Draw(strength.str().c_str(), { 700, 275 }, 0.4f, { 255, 0, 0 });


				  LoadString(HInstance, IDS_MOVESPEED, buf, 255);
				  pFont->Draw(buf, { 410, 320 }, 0.4f, { 255, 0, 0 });
				  movespeed << Slot1->GetMoveSpeed();
				  pFont->Draw(movespeed.str().c_str(), { 700, 325 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_HOOKLEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 370 }, 0.4f, { 255, 0, 0 });
				  hooklevel << Slot1->GetHookLevel();
				  pFont->Draw(hooklevel.str().c_str(), { 700, 375 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_SWORDLEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 420 }, 0.4f, { 255, 0, 0 });
				  swordlevel << Slot1->GetSwordLevel();
				  pFont->Draw(swordlevel.str().c_str(), { 700, 425 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_NUMSB, buf, 255);
				  pFont->Draw(buf, { 410, 470 }, 0.4f, { 255, 0, 0 });
				  numsb << Slot1->GetNumSmokeBombs();
				  pFont->Draw(numsb.str().c_str(), { 700, 475 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_DAQ, buf, 255);
				  pFont->Draw(buf, { 410, 520 }, 0.4f, { 255, 0, 0 });
				  daq << Slot1->GetDAQ();
				  pFont->Draw(daq.str().c_str(), { 700, 525 }, 0.4f, { 255, 0, 0 });
			  }
			  else
			  {
				  rect = { 400, 200, 500, 250 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_EMPTY, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
			  }
			  break;
	}
	case 1:
	{
			  if (Slot2->GetGameLevel() > 0)
			  {
				  rect = { 400, 200, 750, 550 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_GAMELEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
				  gamelevel << Slot2->GetGameLevel();
				  pFont->Draw(gamelevel.str().c_str(), { 700, 225 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_STRENGTH, buf, 255);
				  pFont->Draw(buf, { 410, 270 }, 0.4f, { 255, 0, 0 });
				  strength << Slot2->GetStrength();
				  pFont->Draw(strength.str().c_str(), { 700, 275 }, 0.4f, { 255, 0, 0 });


				  LoadString(HInstance, IDS_MOVESPEED, buf, 255);
				  pFont->Draw(buf, { 410, 320 }, 0.4f, { 255, 0, 0 });
				  movespeed << Slot2->GetMoveSpeed();
				  pFont->Draw(movespeed.str().c_str(), { 700, 325 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_HOOKLEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 370 }, 0.4f, { 255, 0, 0 });
				  hooklevel << Slot2->GetHookLevel();
				  pFont->Draw(hooklevel.str().c_str(), { 700, 375 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_SWORDLEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 420 }, 0.4f, { 255, 0, 0 });
				  swordlevel << Slot2->GetSwordLevel();
				  pFont->Draw(swordlevel.str().c_str(), { 700, 425 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_NUMSB, buf, 255);
				  pFont->Draw(buf, { 410, 470 }, 0.4f, { 255, 0, 0 });
				  numsb << Slot2->GetNumSmokeBombs();
				  pFont->Draw(numsb.str().c_str(), { 700, 475 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_DAQ, buf, 255);
				  pFont->Draw(buf, { 410, 520 }, 0.4f, { 255, 0, 0 });
				  daq << Slot2->GetDAQ();
				  pFont->Draw(daq.str().c_str(), { 700, 525 }, 0.4f, { 255, 0, 0 });
			  }
			  else
			  {
				  rect = { 400, 200, 500, 250 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_EMPTY, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
			  }
			  break;
	}
	case 2:
	{
			  if (Slot3->GetGameLevel() > 0)
			  {
				  rect = { 400, 200, 750, 550 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_GAMELEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
				  gamelevel << Slot3->GetGameLevel();
				  pFont->Draw(gamelevel.str().c_str(), { 700, 225 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_STRENGTH, buf, 255);
				  pFont->Draw(buf, { 410, 270 }, 0.4f, { 255, 0, 0 });
				  strength << Slot3->GetStrength();
				  pFont->Draw(strength.str().c_str(), { 700, 275 }, 0.4f, { 255, 0, 0 });


				  LoadString(HInstance, IDS_MOVESPEED, buf, 255);
				  pFont->Draw(buf, { 410, 320 }, 0.4f, { 255, 0, 0 });
				  movespeed << Slot3->GetMoveSpeed();
				  pFont->Draw(movespeed.str().c_str(), { 700, 325 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_HOOKLEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 370 }, 0.4f, { 255, 0, 0 });
				  hooklevel << Slot3->GetHookLevel();
				  pFont->Draw(hooklevel.str().c_str(), { 700, 375 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_SWORDLEVEL, buf, 255);
				  pFont->Draw(buf, { 410, 420 }, 0.4f, { 255, 0, 0 });
				  swordlevel << Slot3->GetSwordLevel();
				  pFont->Draw(swordlevel.str().c_str(), { 700, 425 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_NUMSB, buf, 255);
				  pFont->Draw(buf, { 410, 470 }, 0.4f, { 255, 0, 0 });
				  numsb << Slot3->GetNumSmokeBombs();
				  pFont->Draw(numsb.str().c_str(), { 700, 475 }, 0.4f, { 255, 0, 0 });

				  LoadString(HInstance, IDS_DAQ, buf, 255);
				  pFont->Draw(buf, { 410, 520 }, 0.4f, { 255, 0, 0 });
				  daq << Slot3->GetDAQ();
				  pFont->Draw(daq.str().c_str(), { 700, 525 }, 0.4f, { 255, 0, 0 });
			  }
			  else
			  {
				  rect = { 400, 200, 500, 250 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_EMPTY, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
			  }
			  break;
	}
	default:
		break;
	}


	LoadString(HInstance, IDS_LOADGAME, buf, 255);
	pFont->Draw(buf, { 27, 52 }, 1.0f, { 255, 255, 255 });
	LoadString(HInstance, IDS_LOADGAME, buf, 255);
	pFont->Draw(buf, { 25, 50 }, 1.0f, { 0, 0, 255 });

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 210 }, 0.0f, {}, BASECOLOR, { 0.3f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 210 }, 0.0f, {}, slot1, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_LOADSLOT1, buf, 255);
	pFont->Draw(buf, { 95, 250 }, 0.5f, slot1);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 260 }, 0.0f, {}, BASECOLOR, { 0.3f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 260 }, 0.0f, {}, slot2, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_LOADSLOT2, buf, 255);
	pFont->Draw(buf, { 95, 300 }, 0.5F, slot2);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 310 }, 0.0f, {}, BASECOLOR, { 0.3f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 310 }, 0.0f, {}, slot3, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_LOADSLOT3, buf, 255);
	pFont->Draw(buf, { 95, 350 }, 0.5f, slot3);

	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBar(), { 60, 360 }, 0.0f, {}, BASECOLOR, { 0.15f, 0.1f });
	SGD::GraphicsManager::GetInstance()->DrawTexture(Game::GetInstance()->GetButtonBall(), { 50, 360 }, 0.0f, {}, MainMenu, { 0.1f, 0.1f });
	LoadString(HInstance, IDS_EXIT, buf, 255);
	pFont->Draw(buf, { 95, 400 }, 0.5f, MainMenu);
}

void	LoadGameState::LoadSlot(Player* player, int slot)
{
	GamePlayState::PlayerInfo p;
	TiXmlDocument doc;
	string filepath = Game::GetInstance()->GetFilePath();
	
	switch (slot)
	{
	case 1:
	{
			  filepath += "SaveSlot1.xml";
			  if (doc.LoadFile(filepath.c_str()) == false)
				  return;
			  break;
	}
	case 2:
	{
			  filepath += "SaveSlot2.xml";
			  if (doc.LoadFile(filepath.c_str()) == false)
				  return;
			  break;
	}
	case 3:
	{
			  filepath += "SaveSlot3.xml";
			  if (doc.LoadFile(filepath.c_str()) == false)
				  return;
			  break;
	}
	default:
		break;
	}

	TiXmlElement* root = doc.RootElement();

	if (root == nullptr)
		return;

	TiXmlElement* GameLevel = root->FirstChildElement("GameLevel");
	GameLevel->Attribute("GameLevel", &p.gamelevel);
	player->SetGameLevel(p.gamelevel);

	TiXmlElement* Stats = root->FirstChildElement("Stats");

	Stats->Attribute("Strength", &p.Strength);
	player->SetStrength(p.Strength);
	Stats->Attribute("MoveSpeed", &p.MoveSpeed);
	player->SetMoveSpeed(p.MoveSpeed);
	Stats->Attribute("MaxHp", &p.MaxHealth);
	player->SetMaxHP(p.MaxHealth);
	Stats->Attribute("CurrHp", &p.CurrHealth);
	player->SetCurrHP(p.CurrHealth);

	player->SetPrevSpeed(p.MoveSpeed);

	TiXmlElement* Items = root->FirstChildElement("Items");

	Items->Attribute("HookLevel", &p.hooklevel);
	player->SetHookLevel(p.hooklevel);
	Items->Attribute("SwordLevel", &p.swordlevel);
	player->SetSwordLevel(p.swordlevel);
	Items->Attribute("SmokeBombs", &p.NumSB);
	player->SetNumSmokeBombs(p.NumSB);
	Items->Attribute("DAQ", &p.DAQ);
	player->SetDAQ(p.DAQ);

	int num = 0;
	Items->Attribute("HaveHook", &num);
	if (num == 0)
		player->SetHaveHook(false);
	else
		player->SetHaveHook(true);
	Items->Attribute("HaveSword", &num);
	if (num == 0)
		player->SetHaveSword(false);
	else
		player->SetHaveSword(true);
	Items->Attribute("HavePizza", &num);
	if (num == 0)
		player->SetHavePizza(false);
	else
		player->SetHavePizza(true);
}