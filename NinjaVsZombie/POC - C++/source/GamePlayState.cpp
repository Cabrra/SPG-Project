/***************************************************************
|	File:		GamePlayState.cpp
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	GamePlayState class handles the game play state
***************************************************************/

#include "GamePlayState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_EventManager.h"

#include "AnchorPointAnimation.h"
#include "AnimationManager.h"
#include "LevelManager.h"
#include "Entity.h"
#include "Zombie.h"
#include "EntityManager.h"
#include "ParticleManager.h"
#include "Emiter.h"
#include "ParticleFlyweight.h"
#include "Player.h"
#include "Hook.h"
#include "Sword.h"
#include "SmokeBomb.h"
#include "MovableBox.h"
#include "ImmovableBox.h"
#include "Pizza.h"
#include "BitmapFont.h"
#include "../TinyXML/tinyxml.h"

#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>

#include "Game.h"

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_MessageManager.h"
#include "MessageID.h"
#include "DestroyEntityMessage.h"

#define WIN32_LEAN_AND_MEAN
using namespace std;

enum Bucket{ BUCKET_PLAYER, BUCKET_ENEMY, BUCKET_PLAYER_ITEM, BUCKET_ENEMY_SHURIKEN, BUCKET_BOX_MOVABLE, BUCKET_BOX_IMMOVABLE, BUCKET_SMOKEBOMB };

/**************************************************************/
// GetInstance
//	- store the ONLY instance in global memory
//	- return the only instance by pointer
/*static*/ GamePlayState* GamePlayState::GetInstance(void)
{
	static GamePlayState s_Instance;

	return &s_Instance;
}

/*virtual*/ void GamePlayState::Enter(void)
{
	SGD::EventManager::GetInstance()->Initialize();
	m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Espionage_and_Deception.xwm");
	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");
	m_DropPizza = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Pizza Drop.wav");

	SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic);

	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);

	LANGID id = GetThreadUILanguage();
	if (id != LANG_SPANISH)
		m_english = true;
	else
		m_english = false;

	//****************************Particle

	particleTest = new ParticleManager;
	
	//ParticleFlyweight* flyweight = new ParticleFlyweight;
	//flyweight->SetColorStartA(250);
	//flyweight->SetColorStartR(255);
	//flyweight->SetColorStartG(255);
	//flyweight->SetColorStartB(255);
	//flyweight->SetColorEndA(125);
	//flyweight->SetColorEndR(240);
	//flyweight->SetColorEndG(128);
	//flyweight->SetColorEndB(128);
	//flyweight->SetMaxLife(9.0f);
	//flyweight->SetMinLife(5.5f);
	//flyweight->SetRotationStart(0);
	//flyweight->SetRotationEnd(SGD::PI * 2);
	//flyweight->SetScaleStart(0.01f);
	//flyweight->SetScaleEnd(0.5f);

	////velocity
	//flyweight->SetVelocityStartMinX(-37.05f);
	//flyweight->SetVelocityStartMaxX(-10.8f);
	//flyweight->SetVelocityEndMinX(15.0f);
	//flyweight->SetVelocityEndMaxX(30.0f);

	//flyweight->SetVelocityStartMinY(12.0f);
	//flyweight->SetVelocityStartMaxY(22.0f);
	//flyweight->SetVelocityEndMinY(-38.0f);
	//flyweight->SetVelocityEndMaxY(-13.0f);

	//SGD::String file = L"smoketex.png";
	//Emiter* emit = new Emiter(file, { 200, 450 }, 50.0f, 50.0f, { 50, 50 }, 50, 20, 0.05f, { 512, 512 }, 0, false, flyweight);

	//delete flyweight;
	//particleTest->AddEmiter(emit, 0);

	particleTest->LoadEmiter("resource/XML/Test.xml");

	//**************************** Particle end


	m_pEntities = new EntityManager;
	m_Player = new Player();
	m_Player->SetHookLevel(1);
	m_Player->SetSwordLevel(1);
	m_Player->SetHaveHook(false);
	m_pEntities->AddEntity(m_Player, BUCKET_PLAYER);

	MovableBox* box1 = new MovableBox();
	m_pEntities->AddEntity(box1, BUCKET_BOX_MOVABLE);
	box1->Release();

	Pizza* pizza = new Pizza();
	pizza->SetPosition({ 500, 100 });
	m_pEntities->AddEntity(pizza, BUCKET_BOX_MOVABLE);
	pizza->Release();

	ImmovableBox* box2 = new ImmovableBox();
	box2->SetPosition({ 500, 200 });
	m_pEntities->AddEntity(box2, BUCKET_BOX_IMMOVABLE);
	box2->Release();

	Hook* playerHook = new Hook({ 200, 100 }, 0, nullptr);
	SGD::HTexture image = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/hook.png");
	playerHook->SetImage(image);
	m_pEntities->AddEntity(playerHook, BUCKET_PLAYER_ITEM);
	playerHook->Release();

	Sword* playerSword = new Sword({ 400, 100 },0,nullptr);
	m_pEntities->AddEntity(playerSword, BUCKET_PLAYER_ITEM);
	playerSword->Release();

	SmokeBomb* smokeBombs = new SmokeBomb(nullptr);
	smokeBombs->SetPosition({ 200, 200 });
	m_pEntities->AddEntity(smokeBombs, BUCKET_SMOKEBOMB);
	smokeBombs->Release();

	Zombie* zombie = new Zombie();
	m_pEntities->AddEntity(zombie, BUCKET_ENEMY);
	zombie->Release();

	AnimationManager::GetInstance()->LoadXML("resource/XML/testfileAnimation.xml");
	testing.currAnimation = "test";
	testing2.currAnimation = "test2";

	//level ***********************************************
	m_LevelManager = new LevelManager;
	m_LevelManager->LoadFile("resource/XML/level1.xml");
	//*****************************************************

	retesting.currAnimation = "Test Animation";

	m_IsPaused = false;
	m_nCursor = 0;
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void GamePlayState::Exit(void)
{
	if (m_Player != nullptr)
	{
		m_Player->Release();
		m_Player = nullptr;
	}

	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;

	
	particleTest->RemoveAll();
	delete particleTest;


	//m_Amanger->RemoveAll();
	//delete m_Amanger;
	//m_Amanger = nullptr;

	delete m_LevelManager;
	m_LevelManager = nullptr; 

	
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_DropPizza);

	SaveVolume();
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool GamePlayState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (m_IsPaused)
	{
		PauseInput();
	}
	else
	{
		if (pInput->IsKeyPressed(SGD::Key::Space) || pInput->IsButtonPressed(0, 0))
			CreatePizza(m_Player);

		if (pInput->IsKeyPressed(SGD::Key::L) || pInput->IsButtonPressed(0, 1))
			CreateSmokeBomb(m_Player);

		if (pInput->IsKeyPressed(SGD::Key::K) || pInput->IsButtonPressed(0, 3))
			CreateHook(m_Player);

		if (pInput->IsKeyPressed(SGD::Key::J) || pInput->IsButtonPressed(0, 2))
			CreateSword(m_Player);
	}

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 7) || pInput->IsKeyPressed(SGD::Key::LButton))
	{
		if (!m_IsPaused)
			m_IsPaused = true;
	}
	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void GamePlayState::Update(float elapsedTime)
{
	if (!m_IsPaused)
	{
		m_pEntities->UpdateAll(elapsedTime);

		m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_BOX_MOVABLE);
		m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_BOX_IMMOVABLE);
		m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_PLAYER_ITEM);
		m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_SMOKEBOMB);

		m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_BOX_MOVABLE);
		m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_BOX_IMMOVABLE);
		m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_ENEMY);

		particleTest->UpdateAll(elapsedTime);

		m_LevelManager->GetMap()->CheckCollision(m_Player);

		AnimationManager::GetInstance()->Update(elapsedTime, testing);
		AnimationManager::GetInstance()->Update(elapsedTime, testing2);
		AnimationManager::GetInstance()->Update(elapsedTime, retesting);

		if (testing2.currFrame == 9)
		{
			SGD::Event* pEvent = new SGD::Event{ "Test" };
			pEvent->QueueEvent();
			pEvent = nullptr;
		}

		SGD::EventManager::GetInstance()->Update();
		SGD::MessageManager::GetInstance()->Update();

		CheckCamAndPlayer();
	}
	m_cursorMoveTimer += elapsedTime;
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void GamePlayState::Render(void)
{
	if (!m_IsPaused)
	{
		m_LevelManager->GetMap()->Render();

		m_pEntities->RenderAll();
		AnimationManager::GetInstance()->Render(600, 700, testing);
		AnimationManager::GetInstance()->Render(700, 500, retesting);
		AnimationManager::GetInstance()->Render(400, 700, testing2);


		//test->Render({ 500, 500 }, false, 1, { 255, 255, 255 });
		particleTest->RenderAll();
	}
	else
	{
		PauseMenu();
	}
}

/*static*/ void GamePlayState::MessageProc(const SGD::Message* pMsg)
{
	/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch (pMsg->GetMessageID())
	{
	case MessageID::MSG_DESTROY_ENTITY:
	{
		  // Downcast to the actual message type
		  const DestroyEntityMessage* pDestroyMsg =
			  dynamic_cast< const DestroyEntityMessage* >(pMsg);

		  assert(pDestroyMsg != nullptr
			  && "GameplayState::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

		  Entity* ptr = pDestroyMsg->GetEntity();

		  // Use the singleton to access members!
		  GamePlayState::GetInstance()->m_pEntities->RemoveEntity(ptr);

		  if (ptr->GetType() == Entity::ENT_HOOK)
			  GamePlayState::GetInstance()->m_Player->SetHookActive(false);
	}
		break;
	case MessageID::MSG_CREATE_ARROW:
		break;
	case MessageID::MSG_CREATE_SHURIKEN:
		break;
	case MessageID::MSG_CREATE_HOOK:
		break;
	case MessageID::MSG_UNKNOWN:
	{
			   OutputDebugStringW(L"GamePlayState::MessageProc - unknown message id\n");
	}
		break;
	}


	/* Restore previous warning levels */
#pragma warning( pop )

}

void	GamePlayState::CreateHook(Entity* owner)
{
	if(!m_Player->IsHookActive() && !m_Player->HavePizza() && m_Player->HaveHook() && !m_Player->IsSwordActive())
	{
		Hook* playerHook = new Hook(m_Player->GetPosition(), m_Player->GetRotation(), m_Player);
		m_pEntities->AddEntity(playerHook, BUCKET_PLAYER_ITEM);
		playerHook->Release();
		m_Player->SetHookActive(true);
	}
}

void	GamePlayState::CreateSword(Entity* owner)
{
	if(!m_Player->IsSwordActive() && !m_Player->HavePizza() && m_Player->HaveSword() && !m_Player->IsHookActive())
	{
		Sword* playerSword = new Sword(m_Player->GetPosition(), m_Player->GetRotation(), m_Player);
		playerSword->SetIsSwinging(true);
		m_pEntities->AddEntity(playerSword, BUCKET_PLAYER_ITEM);
		playerSword->Release();
		m_Player->SetSwordActive(true);
	}
}

void	GamePlayState::CreatePizza(Entity* owner)
{
	if (m_Player->HavePizza())
	{
		Pizza* pizza = new Pizza();

		if (m_Player->GetRotation() == SGD::PI)
			pizza->SetPosition({ m_Player->GetPosition().x, m_Player->GetPosition().y - m_Player->GetSize().height - pizza->GetSize().height });
		else if (m_Player->GetRotation() == 0)
			pizza->SetPosition({ m_Player->GetPosition().x, m_Player->GetPosition().y + m_Player->GetSize().height + pizza->GetSize().height });
		else if (m_Player->GetRotation() == (3 * SGD::PI) / 2)
			pizza->SetPosition({ m_Player->GetPosition().x + m_Player->GetSize().width + pizza->GetSize().width, m_Player->GetPosition().y });
		else if (m_Player->GetRotation() == SGD::PI / 2)
			pizza->SetPosition({ m_Player->GetPosition().x - m_Player->GetSize().width - pizza->GetSize().width, m_Player->GetPosition().y });


		m_pEntities->AddEntity(pizza, BUCKET_BOX_MOVABLE);
		pizza->Release();
		SGD::AudioManager::GetInstance()->PlayAudio(m_DropPizza);
		m_Player->SetHavePizza(false);
	}
}

void	GamePlayState::CreateSmokeBomb(Entity* owner)
{
	if (m_Player->GetNumSmokeBombs() > 0)
	{
		SmokeBomb* bomb = new SmokeBomb(m_Player);

		m_pEntities->AddEntity(bomb, BUCKET_SMOKEBOMB);
		bomb->Release();
		m_Player->SetNumSmokeBombs(m_Player->GetNumSmokeBombs() - 1);
	}
}

void GamePlayState::CheckCamAndPlayer()
{
	if (!m_Player->GetRect().IsIntersecting(Game::GetInstance()->GetWorldCamera()))
	{
		if (m_Player->GetPosition().x < Game::GetInstance()->GetWorldCamera().left)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x + Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetWorldSpace().y });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left - Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetWorldCamera().top }, SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() } });
		}
		else if (m_Player->GetPosition().x > Game::GetInstance()->GetWorldCamera().right)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x - Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetWorldSpace().y });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left + Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetWorldCamera().top }, SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() } });
		}
		else if (m_Player->GetPosition().y < Game::GetInstance()->GetWorldCamera().top)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y + Game::GetInstance()->GetScreenHeight() });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left, Game::GetInstance()->GetWorldCamera().top - Game::GetInstance()->GetScreenHeight() }, SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() } });
		}
		else if (m_Player->GetPosition().y > Game::GetInstance()->GetWorldCamera().bottom)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y - Game::GetInstance()->GetScreenHeight() });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left, Game::GetInstance()->GetWorldCamera().top + Game::GetInstance()->GetScreenHeight() }, SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() } });
		}
	}
}

void GamePlayState::PauseMenu()
{
	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	std::ostringstream out, musicvol;
	out << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
	musicvol << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);

	SGD::Color	SFXVol, MUSICVol, SFXMute, MUSICMute, ExitOption, FullScreen, OnFS, OffFS, Unpause, LanguageOption, EnglishLang, SpanLang;
	SFXVol = MUSICVol = SFXMute = MUSICMute = ExitOption = FullScreen = OnFS = OffFS = Unpause = LanguageOption = EnglishLang = SpanLang ={ 255, 255, 255 };

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
		ExitOption = { 0, 0, 255 };
	else if (m_nCursor == 7)
		Unpause = { 0, 0, 255 };

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];
	LoadString(HInstance, IDS_PAUSE, buf, 255);

	pFont->Draw(buf, { (width - (15 * 40)) / 2, 50 }, 1.0f, { 0, 0, 255 });

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

	if (Game::GetInstance()->IsFullscreen())
		OffFS = { 0, 0, 255 };
	else
		OnFS = { 0, 0, 255 };

	LoadString(HInstance, IDS_ON, buf, 255);
	pFont->Draw(buf, { 500, 350 }, 0.2f, OnFS);
				
	LoadString(HInstance, IDS_OFF, buf, 255);
	pFont->Draw(buf, { 575, 350 }, 0.2f, OffFS);

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

	LoadString(HInstance, IDS_MAINMENU, buf, 255);
	pFont->Draw(buf, { 50, 450 }, 0.3f, ExitOption);

	LoadString(HInstance, IDS_RESUME, buf, 255);
	pFont->Draw(buf, { 50, 500 }, 0.3f, Unpause);
}

void GamePlayState::PauseInput()
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
	{
	/*	if (m_nCursor == 7)
		{
			m_nCursor = 0;
			m_IsPaused = false;
		}
		else*/
			m_nCursor = 7;
		
	}
	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		if (m_nCursor == 6)
			Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
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
			if (Game::GetInstance()->IsFullscreen())
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
		else if (m_nCursor == 7)
		{
			m_nCursor = 0;
			m_IsPaused = false;
		}
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
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && m_cursorMoveTimer>0.3f)
	{
		if (m_nCursor == 0)
			m_nCursor = 7;
		else
			m_nCursor--;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && m_cursorMoveTimer>0.3f)
	{
		if (m_nCursor == 7)
			m_nCursor = 0;
		else
			m_nCursor++;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Right) || pInput->IsDPadDown(0, SGD::DPad::Right) || pInput->GetLeftJoystick(0).x > 0) && m_cursorMoveTimer > 0.3f)
	{
		int SFX = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
		if (m_nCursor == 0)
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
}

void GamePlayState::SaveVolume()
{
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