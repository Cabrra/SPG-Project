#include "GamePlayState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_MessageManager.h"

#include "Puzzle.h"
#include "NeutralEnemy.h"
#include "AnchorPointAnimation.h"
#include "AnimationManager.h"
#include "LevelManager.h"
#include "Entity.h"
#include "Zombie.h"
#include "AmbushNinja.h"
#include "ThrowingNinja.h"
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
#include "Lever.h"
#include "PressurePoint.h"
#include "Door.h"
#include "BitmapFont.h"
#include "PowerUp.h"
#include "FireTrap.h"
#include "LightingTrap.h"
#include "SpikeTrap.h"
#include "FakeTrap.h"
#include "GluttonyGreed.h"
#include "Pride.h"
#include "LustWrath.h"
#include "BossKey.h"
#include "FakeFloor.h"
#include "FakeWall.h"
#include "SpawnTrap.h"
#include "SpawnPoint.h"
#include "Challenge.h"
#include "HeartBullet.h"

#include "../TinyXML/tinyxml.h"

#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>

#include "MainMenuState.h"
#include "LoadGameState.h"
#include "ShopState.h"

#include "MessageID.h"
#include "CreatePowerUpMessage.h"
#include "CreateShurikenMessage.h"
#include "CreateHeartMessage.h"
#include "CreatePizzaMessage.h"
#include "CreateEmiterMessage.h"
#include "DestroyEntityMessage.h"
#include "DeleteEmitterMessage.h"
#include "GameOverMessage.h"

#define FONTSCALE .78f
#define GAMEOVERSCREEN 3.5f
#define THROWLENGTH 64

#define WIN32_LEAN_AND_MEAN
using namespace std;

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
	if (SGD::AudioManager::GetInstance()->IsAudioPlaying(MainMenuState::GetInstance()->GetMusic()))
		SGD::AudioManager::GetInstance()->StopAudio(MainMenuState::GetInstance()->GetMusic());

	m_LoadScreen = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/swordloadscreen.jpg");
	m_hLoadMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Pivot_Point.xwm");
	SGD::AudioManager::GetInstance()->PlayAudio(m_hLoadMusic,true);
	m_loading = true;
	m_doneLoad = false;
	m_Hint = (rand() % 2) + 1;
	LoadScreen();
	SGD::GraphicsManager::GetInstance()->Update();


	tipisup = false;
	finalhint = false;

	// game over
	m_win = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/YouWon.wav");
	m_lose = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Gameover.wav");
	m_hlose = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/gameover.png");
	m_MAster = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/master.png");
	m_hwin = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/winscreen.jpg");
	m_Heart = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/heart.png");
	playonetime = false;

	
	m_cursorMove = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Sword.wav");
	m_DropPizza = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Pizza Drop.wav");
	m_HookSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/hooksound.wav");

	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/NVZ_menu.png");
	m_hPauseMenu = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/littleninja_alt.png");
	m_hSaveMenu = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Ninja-Sword-Background.png");

	m_Bullet = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/default.png");
	m_hNinjaIc = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/NinjaIcon.png");

	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);


	LANGID id = GetThreadUILanguage();
	if (id != LANG_SPANISH)
		m_english = true;
	else
		m_english = false;

	m_pEntities = new EntityManager;

	//ANTICRASH STUFF
	Enemy* enem = new Enemy();
	m_pEntities->AddEntity(enem, BUCKET_ENEMY);
	enem->Release();

	SpawnPoint* newSp = new SpawnPoint();
	newSp->SetPosition({ -200, -200 });
	CreatePride(newSp);
	delete newSp;

	if (LoadGameState::GetInstance()->IsGameLoaded())
		CreatePlayer(newSp);

	AnimationManager::GetInstance()->LoadXML("resource/XML/PlayerAnimations.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/pressurePointAnims.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/ZombieAnimations.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/GreedAnimations.xml");


	AnimationManager::GetInstance()->LoadXML("resource/XML/LadyLustAnimations.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/hdoorcloseanim.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/hdooropenanim.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/hdoorunlockanim.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/vdoorcloseanim.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/vdooropenanim.xml");
	AnimationManager::GetInstance()->LoadXML("resource/XML/vdoorunlockanim.xml");


	if (m_Player == nullptr)
		m_Level = 1;
	else
		m_Level = m_Player->GetGameLevel();

	m_LevelManager = new LevelManager;

	switch (m_Level)
	{
	case 1:
	{
		m_LevelManager->LoadFile("resource/XML/howtoplaylevel.xml");
		m_hMap = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Maps/HowtoMap.png");
		m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Espionage_and_Deception.xwm");
	}
		break;
	case 2:
	{
		m_LevelManager->LoadFile("resource/XML/level1.xml");
		m_hMap = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Maps/Map1.png");
		m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Flight_to_Freedom.xwm");
	}
		break;
	case 3:
	{
		m_LevelManager->LoadFile("resource/XML/level2.xml");
		m_hMap = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Maps/Map2.png");
		m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Follow_Me_Down.xwm");
	}
		break;
	case 4:
	{
		m_LevelManager->LoadFile("resource/XML/level3.xml");
		m_hMap = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Maps/Map3.png");
		m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Ancestors_Path.xwm");
	}
		break;
	case 5:
	{
		m_LevelManager->LoadFile("resource/XML/level4.xml");
		m_hMap = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Maps/Map4.png");
		m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/Music/Break_Away_from_the_Pack.xwm");
	}
		break;
	/*case 6:
	{
			  Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	}
		break;*/
	default:

		break;
	}

	m_IsPaused = false;
	m_nCursor = 0;
	m_lrCursor = 0;

	Slot1 = new Player();
	Slot2 = new Player();
	Slot3 = new Player();

	LoadGameState::GetInstance()->LoadSlot(Slot1, 1);
	LoadGameState::GetInstance()->LoadSlot(Slot2, 2);
	LoadGameState::GetInstance()->LoadSlot(Slot3, 3);

	for (unsigned int st = 0; st < m_LevelManager->GetRooms().size(); st++)
	{
		int count = 0;
		for (unsigned int rm = 0; rm < m_LevelManager->GetRooms()[st]->GetSpawnpoints().size(); rm++)
		{
			if (m_LevelManager->GetRooms()[st]->GetSpawnpoints()[rm]->GetType() == SpawnPoint::m_eObjectType::PLAYER)
			{
				if (m_Player == nullptr)
				{
					CreatePlayer(m_LevelManager->GetRooms()[st]->GetSpawnpoints()[rm]);
					//playerStart = false;
				}
				else if (LoadGameState::GetInstance()->IsGameLoaded())
				{
					m_Player->SetSpawnPoint(m_LevelManager->GetRooms()[st]->GetSpawnpoints()[rm]);
					m_Player->SetPosition(m_LevelManager->GetRooms()[st]->GetSpawnpoints()[rm]->GetPosition());
				}
				else
				{
					m_Player->SetSpawnPoint(m_LevelManager->GetRooms()[st]->GetSpawnpoints()[rm]);
					m_Player->SetCurrHP(m_Player->GetMaxHP());
					m_Player->SetPosition(m_LevelManager->GetRooms()[st]->GetSpawnpoints()[rm]->GetPosition());
					m_pEntities->AddEntity(m_Player, BUCKET_PLAYER);
				}
				count++;
				break;
			}
		}
		if (count >= 1)
			break;
	}


	switch (m_Level)
	{
	case 1:
	{
			  m_Player->SetTimer(240);

	}
		break;
	case 2:
	{
			  m_Player->SetTimer(240);
	}
		break;
	case 3:
	{
			  m_Player->SetTimer(400);

	}
		break;
	case 4:
	{
			  m_Player->SetTimer(450);

	}
		break;
	case 5:
	{
			  m_Player->SetTimer(300);

	}
		break;
		//default:
		//	break;
	}

	//CreatePressurePoint({ 300, 200 }, m_Player);


	m_Lose = m_Win = false;
	m_myAmbush = false;

	m_doneLoad = true;
}

/**************************************************************/
// Exit
//	- deallocate / unload resources
/*virtual*/ void GamePlayState::Exit(void)
{
	//delete ambushPoint;
	if (!m_Win && m_Player != nullptr)
	{
		m_Player->Release();
		m_Player = nullptr;
	}

	AnimationManager::GetInstance()->Terminate();

	Slot1->Release();
	Slot2->Release();
	Slot3->Release();

	m_pEntities->RemoveAll();
	delete m_pEntities;
	m_pEntities = nullptr;

	ParticleManager::GetParticleInstance()->RemoveAll(1);

	delete m_LevelManager;

	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_cursorMove);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_DropPizza);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_HookSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hLoadMusic);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_win);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_lose);

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPauseMenu);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSaveMenu);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Bullet);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hMap);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hNinjaIc);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_MAster);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Heart);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_LoadScreen);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hwin);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hlose);

	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	m_Win = false;
	m_Lose = false;
	m_myAmbush = false;

	SaveVolume();
}

/**************************************************************/
// Input
//	- handle user input
/*virtual*/ bool GamePlayState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (!m_loading)
	{

		if (m_IsPaused)
		{
			if (!m_SaveGame)
				PauseInput();
			else
				SaveInput();
		}
		else if (tipisup)
		{
			if (pInput->IsKeyPressed(SGD::Key::Space) || pInput->IsButtonPressed(0, 0))
			{
				tipisup = false;
				helpbox = 0;
			}
		}
		else if (m_Player != nullptr && m_Player->GetFighting())
		{

		}
		else if (m_Player->IsFalling() == false && !m_Lose && !m_Win)
		{
			if (!m_map && !m_Win && !m_Lose)
			{
				if (pInput->IsKeyPressed(SGD::Key::Space) || pInput->IsButtonPressed(0, 0))
				{
					if (m_Player->GetHavePizza())
						CreatePizza(m_Player);
				}
				if (pInput->IsKeyPressed(SGD::Key::L) || pInput->IsButtonPressed(0, 3))
					CreateSmokeBomb(m_Player);

				if (pInput->IsKeyPressed(SGD::Key::K) || pInput->IsButtonPressed(0, 1))
					CreateHook(m_Player);

				if (pInput->IsKeyPressed(SGD::Key::J) || pInput->IsButtonPressed(0, 2))
					CreateSword(m_Player);
			}

			if (pInput->IsKeyPressed(SGD::Key::M))
				m_map = !m_map;
		}

		if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 7))
		{
			if (!m_IsPaused)
			{
				m_IsPaused = true;
				SGD::Event* stopSound = new SGD::Event{ "Pause_Sound", nullptr, nullptr };
				stopSound->SendEventNow(nullptr);
				delete stopSound;
			}
		}
		//if (pInput->IsKeyPressed(SGD::Key::B))
		//{
			//m_Win = true;
		//}

	}
	else if (pInput->IsKeyPressed(SGD::Key::Space) || pInput->IsButtonPressed(0, 0))
	{
		SGD::AudioManager::GetInstance()->StopAudio(m_hLoadMusic);
		SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);
		m_loading = false;
	}
	return true;
}

/**************************************************************/
// Update
//	- update entities / animations
/*virtual*/ void GamePlayState::Update(float elapsedTime)
{
	if (GetFocus() == false && !m_loading)
		m_IsPaused = true;

	if (!m_loading)
	{
		if (helpbox > 25)
			GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_HintBox);


		if (m_Lose)
		{
			m_fgameover += elapsedTime;
			if (!playonetime)
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_lose);
				playonetime = true;
			}
			if (m_fgameover > GAMEOVERSCREEN)
			{
				Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
				m_fgameover = 0;
			}
		}
		else if (m_Win)
		{
			m_fgameover += elapsedTime;
			if (!playonetime)
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_win);
				playonetime = true;
			}
			if (m_fgameover > GAMEOVERSCREEN)
			{
				LoadGameState::GetInstance()->SetIsLoaded(false);
				m_fgameover = 0;
				if (m_Level < 5)
					Game::GetInstance()->ChangeState(ShopState::GetInstance());
				else
					Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

			}
		}
		else if (!m_IsPaused && !tipisup && !m_map)
		{
			if (!finalhint)
			{
				helpbox += elapsedTime;
			}

			m_pEntities->UpdateAll(elapsedTime);

			ParticleManager::GetParticleInstance()->UpdateAll(elapsedTime);

			m_LevelManager->Update(elapsedTime);
			for (size_t i = 0; i < m_LevelManager->GetRooms().size(); i++)
			{
				m_LevelManager->GetRooms()[i]->CheckCollision(m_Player);
				m_pEntities->CheckCollisionWithTiles(m_LevelManager->GetRooms()[i], BUCKET_ENEMY);
				m_pEntities->CheckCollisionWithTiles(m_LevelManager->GetRooms()[i], BUCKET_PICKUP);
			}


			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_ENEMY);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_PRIDE);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_PICKUP);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_ENEMY_SHURIKEN);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_ENEMY_HEART);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_TRAP);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_DOOR);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_PLAYER_ITEM);
			m_pEntities->CheckCollisions(BUCKET_PLAYER, BUCKET_LEVER);
			m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_ENEMY_SHURIKEN);
			m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_BOX_IMMOVABLE);
			m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_BOX_MOVABLE);
			m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_PICKUP);
			m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_ENEMY);
			m_pEntities->CheckCollisions(BUCKET_PLAYER_ITEM, BUCKET_LEVER);
			m_pEntities->CheckCollisions(BUCKET_ENEMY, BUCKET_SMOKE);
			m_pEntities->CheckCollisions(BUCKET_GLUTTONY, BUCKET_PLAYER_ITEM);
			m_pEntities->CheckCollisions(BUCKET_GLUTTONY, BUCKET_PICKUP);
			m_pEntities->CheckCollisions(BUCKET_GLUTTONY, BUCKET_PLAYER);
			m_pEntities->CheckCollisions(BUCKET_PRIDE, BUCKET_TRAP);
			m_pEntities->CheckCollisions(BUCKET_PRIDE, BUCKET_PLAYER_ITEM);
			m_pEntities->CheckCollisions(BUCKET_LUST, BUCKET_TRAP);
			m_pEntities->CheckCollisions(BUCKET_LUST, BUCKET_PLAYER_ITEM);
			m_pEntities->CheckCollisions(BUCKET_ENEMY, BUCKET_LEVER);
			m_pEntities->CheckCollisions(BUCKET_LEVER, BUCKET_BOX_MOVABLE);



			//AnimationManager::GetInstance()->Update(elapsedTime, retesting);


			CheckCamAndPlayer();

			if (!m_Player->GetHavePizza() && m_Pizza != nullptr && m_myAmbush == false)
			{
				SGD::Vector distance = m_Player->GetPosition() - m_Pizza->GetPosition();
				float length = distance.ComputeLength();

				if (length >= 300 && m_Pizza != nullptr)
				{
					SGD::Event* pEvent = new SGD::Event{ "PIZZA_FREE" };
					pEvent->QueueEvent();
					pEvent = nullptr;
				}
			}

			SGD::Rectangle rect = SGD::Rectangle{ m_LevelManager->GetEndPoint(), SGD::Point{ m_LevelManager->GetEndPoint().x + 32, m_LevelManager->GetEndPoint().y + 32 } };
			if ((m_Player->GetRect().IsIntersecting(rect) && m_Player->GetHavePizza()) || (!m_Player->GetHavePizza() && m_Pizza != nullptr && m_Pizza->GetRect().IsIntersecting(rect)))
			{
				m_Player->CheckPowerUp();
				if (m_Player->GetGameLevel() <= 5)
				{
					m_Player->SetGameLevel(m_Player->GetGameLevel() + 1);
					m_Player->SetDAQ( m_Player->GetDAQ() +(int)(m_Player->GetTimer()/2));
				}
				
				m_Win = true;
				SGD::AudioManager::GetInstance()->StopAudio(m_hMusic);
			}

			SGD::EventManager::GetInstance()->Update();
			SGD::MessageManager::GetInstance()->Update();
		}
		m_cursorMoveTimer += elapsedTime;
	}
}

/**************************************************************/
// Render
//	- render entities / menu options
/*virtual*/ void GamePlayState::Render(void)
{
	if (!m_loading)
	{
		if (!m_IsPaused && !m_map)
		{
			m_LevelManager->GetMap()->Render();
			m_LevelManager->RenderEndPoint();
			m_pEntities->RenderAll();
			ParticleManager::GetParticleInstance()->RenderAll();

			const BitmapFont* pFont = Game::GetInstance()->GetFont();

			if (tipisup)
			{
				SGD::GraphicsManager::GetInstance()->DrawRectangle(BoxRect, SGD::Color{ 0, 255, 255, 255 }, { 0, 255, 0, 255 }, 5);
				pFont->Draw(tiptext.c_str(), { BoxRect.left + 115, BoxRect.top + 65 }, FONTSCALE, { 255, 0, 0, 0 });
				pFont->Draw(tiptext.c_str(), { BoxRect.left + 112, BoxRect.top + 62 }, FONTSCALE, { 255, 255, 255, 255 });
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_MAster, { BoxRect.left - 10, BoxRect.top + 45 }, 0, {}, {}, { .2f, .2f });
			}
			else if (m_Lose)
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hlose, { 0, 0 }, 0, {}, {}, { .8f, 1 });
			}
			else if (m_Win)
			{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hwin, { 0, 0 }, 0, {}, {}, { .8f, 1 });
			}
		}
		else if (m_IsPaused == true)
		{
			PauseMenu();
		}
		else if (m_map == true)
		{
			ShowMap();
		}
	}
	else
		LoadScreen();
}

/**************************************************************/
// MessageProc
//	- gets the messages sent by objects and uses them
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
											  dynamic_cast<const DestroyEntityMessage*>(pMsg);

										  assert(pDestroyMsg != nullptr
											  && "GameplayState::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

										  Entity* ptr = pDestroyMsg->GetEntity();
										  // Use the singleton to access members!
										  GamePlayState::GetInstance()->m_pEntities->RemoveEntity(ptr);

										  if (ptr->GetType() == Entity::ENT_HOOK)
										  {
											  Hook* hook = (Hook*)ptr;
											  GamePlayState::GetInstance()->m_Player->SetHookActive(false);
											  GamePlayState::GetInstance()->m_Player->SetBeingPulled(false);
											  if (hook->HookedItem() != nullptr && hook->HookedItem()->GetType() == Entity::ENT_IMMOVABLE)
											  {
												  ImmovableBox* box = (ImmovableBox*)hook->HookedItem();
												  box->SetHook(nullptr);
											  }
										  }
										  else if (ptr->GetType() == Entity::ENT_PIZZA && GamePlayState::GetInstance()->m_Pizza != nullptr && GamePlayState::GetInstance()->m_Pizza->IsFalling())
										  {
											  vector<Room*> rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
											  for (size_t i = 0; i < rooms.size(); i++)
											  {
												  if (rooms[i]->IsPlayerHere())
													  GamePlayState::GetInstance()->CreatePizza(nullptr, rooms[i]->GetRespawnpoint());
											  }
										  }
										  else if (ptr->GetType() == Entity::ENT_PIZZA && GamePlayState::GetInstance()->m_Pizza != nullptr && GamePlayState::GetInstance()->m_Pizza->IsFalling() == false)
											  GamePlayState::GetInstance()->m_Pizza = nullptr;
										  else if (ptr->GetType() == Entity::ENT_BULLET && ((Bullet*)ptr)->GetOwner()->GetType() == Entity::ENT_LUST)
										  {
											  Entity* own = ((Bullet*)ptr)->GetOwner();

											  ((LustWrath*)own)->SetHeartActive(false);
										  }
	}
		break;
	case MessageID::MSG_CREATE_ARROW:
		break;
	case MessageID::MSG_CREATE_SHURIKEN:
	{
										   // Downcast to the actual message type
										   const CreateShurikenMessage* pBulletMsg =
											   dynamic_cast<const CreateShurikenMessage*>(pMsg);

										   assert(pBulletMsg != nullptr
											   && "Game::MessageProc - MSG_CREATE_BULLET is not actually a DestroyEntityMessage");

										   Entity* ptr = GamePlayState::GetInstance()->CreateBullet(pBulletMsg->GetOwner());
										   GamePlayState::GetInstance()->m_pEntities->AddEntity(ptr, BUCKET_ENEMY_SHURIKEN);
										   ptr->Release();

										   break;
	}
	case MessageID::MSG_CREATE_HEART:
	{
										// Downcast to the actual message type
										const CreateHeartMessage* pBulletMsg =
											dynamic_cast<const CreateHeartMessage*>(pMsg);

										assert(pBulletMsg != nullptr
											&& "Game::MessageProc - MSG_CREATE_HEART is not actually a CREATE HEART Message");

										Entity* ptr = GamePlayState::GetInstance()->CreateHeart(pBulletMsg->GetOwner());
										GamePlayState::GetInstance()->m_pEntities->AddEntity(ptr, BUCKET_ENEMY_HEART);
										ptr->Release();

										break;
	}
	case MessageID::MSG_CREATE_HOOK:
		break;
	case MessageID::MSG_CREATE_PIZZA:
	{
										const CreatePizzaMessage* pCreateMsg =
											dynamic_cast<const CreatePizzaMessage*>(pMsg);

										GamePlayState::GetInstance()->CreatePizza(nullptr, pCreateMsg->GetPoint());
										break;
	}
	case MessageID::MSG_CREATE_POWERUP:
	{
										  //CreatePowerUpMessage
										  const CreatePowerUpMessage* pCreateMsg =
											  dynamic_cast<const CreatePowerUpMessage*>(pMsg);

										  assert(pCreateMsg != nullptr
											  && "GameplayState::MessageProc - MSG_CREATE_POWERUP is not actually a CreatePowerUpMessage");

										  PowerUp* newpower = new PowerUp(pCreateMsg->GetPowerType(), GamePlayState::GetInstance()->m_Player->GetGameLevel(), pCreateMsg->GetPoint());

										  GamePlayState::GetInstance()->m_pEntities->AddEntity(newpower, BUCKET_PICKUP);

										  newpower->Release();
										  newpower = nullptr;


										  break;
	}
	case MessageID::MSG_CREATE_EMITTER:
	{
										  const CreateEmiterMessage* pCreateMsg =
											  dynamic_cast<const CreateEmiterMessage*>(pMsg);

										  assert(pCreateMsg != nullptr
											  && "GameplayState::MessageProc - MSG_CREATE_EMITTER is not actually a CreateEmiterMessage");
										  if (GamePlayState::GetInstance()->m_Player != nullptr)
										  {
											  if (pCreateMsg->GetEmiterType() == 0)//smoke bomb
												  ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::SMOKEBOMB, pCreateMsg->GetOwner());
											  if (pCreateMsg->GetEmiterType() == 1) //speed
											  {
												  ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::SPEED_POWER, pCreateMsg->GetOwner());

											  }
											  if (pCreateMsg->GetEmiterType() == 2)//strength
											  {

												  ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::STRENGTH_POWER, pCreateMsg->GetOwner());
											  }

											  if (pCreateMsg->GetEmiterType() == 3)//fire
											  {
												  ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::FIRE_TRAP, pCreateMsg->GetOwner());
											  }
											  if (pCreateMsg->GetEmiterType() == 4)//Electric
											  {
												  ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::ELECTRIC_TRAP, pCreateMsg->GetOwner());
											  }
											  if (pCreateMsg->GetEmiterType() == 5)//Fake wall / fake floor
											  {
												  ParticleManager::GetParticleInstance()->ActivateEmiter(ParticleOrder::LIL_BOMB, pCreateMsg->GetOwner());
											  }
										  }

										  break;
	}
	case MessageID::MSG_DELETE_EMITTER:
	{
										  const DeleteEmitterMessage* pDeleteMsg =
											  dynamic_cast<const DeleteEmitterMessage*>(pMsg);

										  assert(pDeleteMsg != nullptr
											  && "GameplayState::MessageProc - MSG_DELETE_EMITTER is not actually a DeleteEmitterMessage");

										  ParticleManager::GetParticleInstance()->RemoveEmiter(pDeleteMsg->GetMyOwner(), 1);

										  break;
	}
	case MessageID::MSG_GAME_OVER:
	{
									  const GameOverMessage* pGameOverMsg =
										  dynamic_cast<const GameOverMessage*>(pMsg);
									  Entity* sender = (Entity*)pGameOverMsg->GetOwner();
									 if (GamePlayState::GetInstance()->m_Player->dead)
										 GamePlayState::GetInstance()->m_Lose = true;
									 else if (pGameOverMsg->GetOwner()->GetHavePizza())
										 GamePlayState::GetInstance()->m_Lose = true;
									 else if (sender->GetType() == Entity::ENT_BOSS || sender->GetType() == Entity::ENT_AMBUSHNINJA
										 || sender->GetType() == Entity::ENT_ZOMBIE || sender->GetType() == Entity::ENT_ARMOREDZOMBIE)
										 GamePlayState::GetInstance()->m_Lose = true;
									 break;
	}
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
	if (!m_Player->IsHookActive() && !m_Player->GetHavePizza() && m_Player->HaveHook() && !m_Player->IsSwordActive())
	{
		Hook* playerHook = new Hook(m_Player->GetPosition(), m_Player->GetRotation(), m_Player);
		m_pEntities->AddEntity(playerHook, BUCKET_PLAYER_ITEM);
		playerHook->Release();
		m_Player->SetHookActive(true);
		SGD::AudioManager::GetInstance()->PlayAudio(m_HookSound);
	}
}

void	GamePlayState::CreateSword(Entity* owner)
{
	if (!m_Player->IsSwordActive() && !m_Player->GetHavePizza() && m_Player->HaveSword() && !m_Player->IsHookActive())
	{
		Sword* playerSword = new Sword(m_Player->GetPosition(), m_Player->GetRotation(), m_Player);
		playerSword->SetIsSwinging(true);
		m_pEntities->AddEntity(playerSword, BUCKET_PLAYER_ITEM);
		playerSword->Release();
		m_Player->SetSwordActive(true);
	}
}

void	GamePlayState::CreatePizza(Entity* owner, SGD::Point point)
{
	Pizza* pizza = new Pizza();

	if (owner == nullptr)
		pizza->SetPosition(point);
	else if (owner == m_Player)
	{
		SGD::Rectangle pizzarect = { 0, 0, 0, 0 };
		SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
		if (m_Player->GetRotation() == SGD::PI)
		{
			pizzarect.left = m_Player->GetRect().ComputeCenter().x - pizza->GetSize().width / 2;
			pizzarect.top = (m_Player->GetRect().ComputeCenter().y - pizza->GetSize().height / 2) - THROWLENGTH;
		}
		else if (m_Player->GetRotation() == 0)
		{
			pizzarect.left = m_Player->GetRect().ComputeCenter().x - pizza->GetSize().width / 2;
			pizzarect.top = (m_Player->GetRect().ComputeCenter().y - pizza->GetSize().height / 2) + THROWLENGTH;
		}
		else if (m_Player->GetRotation() == (3 * SGD::PI) / 2)
		{
			pizzarect.left = (m_Player->GetRect().ComputeCenter().x - pizza->GetSize().width / 2) + THROWLENGTH;
			pizzarect.top = m_Player->GetRect().ComputeCenter().y - pizza->GetSize().height / 2;
		}
		else if (m_Player->GetRotation() == SGD::PI / 2)
		{
			pizzarect.left = (m_Player->GetRect().ComputeCenter().x - pizza->GetSize().width / 2) - THROWLENGTH;
			pizzarect.top = m_Player->GetRect().ComputeCenter().y - pizza->GetSize().height / 2;
		}

		pizzarect.right = pizzarect.left + pizza->GetSize().width;
		pizzarect.bottom = pizzarect.top + pizza->GetSize().height;

		if (!pizza->CheckCollision({ pizzarect.left, pizzarect.top }) && rect.IsPointInRectangle(pizzarect.ComputeCenter()))
		{
			pizza->SetPosition({ pizzarect.left, pizzarect.top });
			m_Player->SetHavePizza(false);
			SGD::AudioManager::GetInstance()->PlayAudio(m_DropPizza);
		}
		else
		{
			std::vector<Room*>	rooms = GetLevelManager()->GetRooms();
			int num = THROWLENGTH;
			do
			{
				num = num - 2;
				if (m_Player->GetRotation() == SGD::PI)
				{
					pizzarect.top = (m_Player->GetRect().ComputeCenter().y - pizza->GetSize().height / 2) - num;
				}
				else if (m_Player->GetRotation() == 0)
				{
					pizzarect.top = (m_Player->GetRect().ComputeCenter().y - pizza->GetSize().height / 2) + num;
				}
				else if (m_Player->GetRotation() == (3 * SGD::PI) / 2)
				{
					pizzarect.left = (m_Player->GetRect().ComputeCenter().x - pizza->GetSize().width / 2) + num;
				}
				else if (m_Player->GetRotation() == SGD::PI / 2)
				{
					pizzarect.left = (m_Player->GetRect().ComputeCenter().x - pizza->GetSize().width / 2) - num;
				}
			} while (pizza->CheckCollision({ pizzarect.left, pizzarect.top }) || !rect.IsPointInRectangle(pizzarect.ComputeCenter()));

			pizza->SetPosition({ pizzarect.left, pizzarect.top });
			m_Player->SetHavePizza(false);
			SGD::AudioManager::GetInstance()->PlayAudio(m_DropPizza);
		}
	}
	else if (owner != nullptr)
		pizza->SetPosition({ owner->GetPosition().x, owner->GetPosition().y });

	m_pEntities->AddEntity(pizza, BUCKET_PICKUP);
	m_Pizza = pizza;
	pizza->Release();

}

void	GamePlayState::CreateSmokeBomb(Entity* owner)
{
	if (owner->GetType() == Entity::ENT_PLAYER)
	{
		if (m_Player->GetNumSmokeBombs() > 0)
		{
			SmokeBomb* bomb = new SmokeBomb(m_Player);

			m_pEntities->AddEntity(bomb, BUCKET_SMOKE);
			CreateEmiterMessage* smokemsg = new CreateEmiterMessage(bomb, 0);

			smokemsg->QueueMessage();
			smokemsg = nullptr;

			bomb->Release();
			m_Player->SetNumSmokeBombs(m_Player->GetNumSmokeBombs() - 1);
		}
	}
	else
	{
		SmokeBomb* bomb = new SmokeBomb(owner);

		m_pEntities->AddEntity(bomb, BUCKET_PICKUP);
	}
}

void GamePlayState::SpawnHook(Entity* owner, SGD::Point point)
{
	Hook* playerHook = new Hook(point, 0, nullptr);
	SGD::HTexture image = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/hook.png");
	playerHook->SetImage(image);
	m_pEntities->AddEntity(playerHook, BUCKET_PICKUP);
	playerHook->Release();
}

void GamePlayState::SpawnSword(Entity* owner, SGD::Point point)
{
	Sword* playerSword = new Sword(point, 0, nullptr);
	m_pEntities->AddEntity(playerSword, BUCKET_PICKUP);
	playerSword->Release();
}

void GamePlayState::SpawnSmokeBomb(SGD::Point point)
{
	SmokeBomb* smokeBombs = new SmokeBomb(nullptr);
	smokeBombs->SetPosition(point);
	m_pEntities->AddEntity(smokeBombs, BUCKET_PICKUP);
	smokeBombs->Release();
}

void GamePlayState::CheckCamAndPlayer()
{
	if (!Game::GetInstance()->GetWorldCamera().IsPointInRectangle(m_Player->GetRect().ComputeCenter()))
	{
		helpbox -= 10;
		if (m_Player->GetPosition().x < Game::GetInstance()->GetWorldCamera().left)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x + Game::GetInstance()->GetScreenWidth(),
				Game::GetInstance()->GetWorldSpace().y });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left - Game::GetInstance()->GetScreenWidth(),
				Game::GetInstance()->GetWorldCamera().top }, SGD::Size{ Game::GetInstance()->GetScreenWidth(),
				Game::GetInstance()->GetScreenHeight() - HUDHEIGHT } });
			GamePlayState::GetInstance()->m_Player->SetSafeZone(false);
		}
		else if (m_Player->GetPosition().x > Game::GetInstance()->GetWorldCamera().right)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x - Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetWorldSpace().y });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left + Game::GetInstance()->GetScreenWidth(),
				Game::GetInstance()->GetWorldCamera().top }, SGD::Size{ Game::GetInstance()->GetScreenWidth(),
				Game::GetInstance()->GetScreenHeight() - HUDHEIGHT } });
			GamePlayState::GetInstance()->m_Player->SetSafeZone(false);
		}
		else if (m_Player->GetPosition().y < Game::GetInstance()->GetWorldCamera().top)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y + (Game::GetInstance()->GetScreenHeight() - HUDHEIGHT) });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left, Game::GetInstance()->GetWorldCamera().top - (Game::GetInstance()->GetScreenHeight() - HUDHEIGHT) },
				SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() - HUDHEIGHT } });
			GamePlayState::GetInstance()->m_Player->SetSafeZone(false);
		}
		else if (m_Player->GetPosition().y > Game::GetInstance()->GetWorldCamera().bottom)
		{
			Game::GetInstance()->SetWorldSpace({ Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y - (Game::GetInstance()->GetScreenHeight() - HUDHEIGHT) });
			Game::GetInstance()->SetWorldCamera({ { Game::GetInstance()->GetWorldCamera().left, Game::GetInstance()->GetWorldCamera().top + (Game::GetInstance()->GetScreenHeight() - HUDHEIGHT) },
				SGD::Size{ Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight() - HUDHEIGHT } });
			GamePlayState::GetInstance()->m_Player->SetSafeZone(false);
		}

		if (m_Pizza != nullptr && m_Player->GetHavePizza() == false && Lose() == false && Win() == false)
		{
			if (Game::GetInstance()->GetWorldCamera().IsPointInRectangle(m_Pizza->GetRect().ComputeCenter()) == false
				&& Game::GetInstance()->GetWorldCamera().IsPointInRectangle(m_Player->GetRect().ComputeCenter()) == true)
				m_Lose = true;
		}
	}
}

void GamePlayState::PauseMenu()
{
	if (!m_SaveGame)
	{
		// Use the game's font
		const BitmapFont* pFont = Game::GetInstance()->GetFont();
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hPauseMenu, { 0, 0 }, 0.0f, {}, {}, { .4f, .3f });
		std::ostringstream out, musicvol;
		out << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
		musicvol << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);

		SGD::Color	SFXVol, MUSICVol, SFXMute, MUSICMute, MainMenu, FullScreen, OnFS, OffFS, Unpause, LanguageOption, EnglishLang, SpanLang, Save, Skip;
		SFXVol = MUSICVol = SFXMute = MUSICMute = MainMenu = FullScreen = OnFS = OffFS = Unpause = LanguageOption = EnglishLang = SpanLang = Save = Skip = { 255, 255, 255 };

		if (m_lrCursor == 1)
			Skip = { 0, 0, 255 };
		else if (m_nCursor == 0)
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
			Save = { 0, 0, 255 };
		else if (m_nCursor == 7)
			MainMenu = { 0, 0, 255 };
		else if (m_nCursor == 8)
			Unpause = { 0, 0, 255 };

		HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

		char buf[255];
		LoadString(HInstance, IDS_PAUSE, buf, 255);

		pFont->Draw(buf, { 100, 50 }, 1.0f, { 0, 0, 255 });

		LoadString(HInstance, IDS_SFXVOLUME, buf, 255);
		pFont->Draw(buf, { 50, 150 }, 0.5f, SFXVol);
		pFont->Draw(out.str().c_str(), { 550, 150 }, 0.5f, SFXVol);

		LoadString(HInstance, IDS_MUSICVOLUME, buf, 255);
		pFont->Draw(buf, { 50, 200 }, 0.5f, MUSICVol);
		pFont->Draw(musicvol.str().c_str(), { 550, 200 }, 0.5f, MUSICVol);

		LoadString(HInstance, IDS_SFXMUTEMAX, buf, 255);
		pFont->Draw(buf, { 50, 250 }, 0.5f, SFXMute);

		LoadString(HInstance, IDS_MUSICMUTEMAX, buf, 255);
		pFont->Draw(buf, { 50, 300 }, 0.5f, MUSICMute);

		LoadString(HInstance, IDS_FULLSCREEN, buf, 255);
		pFont->Draw(buf, { 50, 350 }, 0.5f, FullScreen);

		if (Game::GetInstance()->IsFullscreen())
			OffFS = { 0, 0, 255 };
		else
			OnFS = { 0, 0, 255 };

		LoadString(HInstance, IDS_ON, buf, 255);
		pFont->Draw(buf, { 500, 350 }, 0.4f, OnFS);

		LoadString(HInstance, IDS_OFF, buf, 255);
		pFont->Draw(buf, { 575, 350 }, 0.4f, OffFS);

		LoadString(HInstance, IDS_LANGUAGE, buf, 255);
		pFont->Draw(buf, { 50, 400 }, 0.5f, LanguageOption);

		if (m_english == true)
			EnglishLang = { 0, 0, 255 };
		else
			SpanLang = { 0, 0, 255 };

		LoadString(HInstance, IDS_ENGLISH, buf, 255);
		pFont->Draw(buf, { 325, 400 }, 0.5f, EnglishLang);

		LoadString(HInstance, IDS_SPANISH, buf, 255);
		pFont->Draw(buf, { 550, 400 }, 0.5f, SpanLang);

		LoadString(HInstance, IDS_SAVEGAME, buf, 255);
		pFont->Draw(buf, { 50, 450 }, 0.5f, Save);

		LoadString(HInstance, IDS_MAINMENU, buf, 255);
		pFont->Draw(buf, { 50, 500 }, 0.5f, MainMenu);

		LoadString(HInstance, IDS_RESUME, buf, 255);
		pFont->Draw(buf, { 50, 550 }, 0.5f, Unpause);

		if (m_Player->GetGameLevel() == 1)
		{
			LoadString(HInstance, IDS_SKIP, buf, 255);
			pFont->Draw(buf, { 250, 550 }, 0.5f, Skip);
		}
	}
	else
		SaveMenu();
}

void GamePlayState::PauseInput()
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		m_nCursor = 8;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		if (m_Player->GetGameLevel() == 1 && m_lrCursor == 1)
		{
			m_lrCursor = 0;
			m_Win = true;
			m_IsPaused = false;
			m_Player->SetGameLevel(2);
			m_Player->SetHaveHook(true);
			m_Player->SetHaveSword(true);
		}
		else if (m_nCursor == 7)
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
		else if (m_nCursor == 8)
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
		else if (m_nCursor == 6)
		{
			m_nCursor = 1;
			m_lrCursor = 1;
			m_SaveGame = true;
		}
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && m_cursorMoveTimer>0.3f)
	{
		if (m_nCursor == 0)
			m_nCursor = 8;
		else
			m_nCursor--;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && m_cursorMoveTimer > 0.3f)
	{
		if (m_nCursor == 8)
			m_nCursor = 0;
		else
			m_nCursor++;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Right) || pInput->IsDPadDown(0, SGD::DPad::Right) || pInput->GetLeftJoystick(0).x > 0) && m_cursorMoveTimer > 0.3f)
	{
			int SFX = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
			int Music = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
			if (m_nCursor == 0)
			{
				if (SFX < 100)
					SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, SFX + 5);
				SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
			}
			else if (m_nCursor == 1)
			{
				if (Music < 100)
					SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, Music + 5);
			}
			else if (m_Player->GetGameLevel() == 1)
			{
				if (m_lrCursor == 0)
					m_lrCursor = 1;
				else
					m_lrCursor = 0;
			}
		
			m_cursorMoveTimer = 0;
	}

	if ((pInput->IsKeyDown(SGD::Key::Left) || pInput->IsDPadDown(0, SGD::DPad::Left) || pInput->GetLeftJoystick(0).x < 0) && m_cursorMoveTimer > 0.3f)
	{
		int SFX = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
		int Music = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
		if (m_nCursor == 0 && SFX > 0)
		{
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, SFX - 5);
			SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
		}
		else if (m_nCursor == 1 && Music > 0)
		{
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, Music - 5);
		}
		else if (m_Player->GetGameLevel() == 1)
		{
			if (m_lrCursor == 0)
				m_lrCursor = 1;
			else
				m_lrCursor = 0;
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

	string filepath = Game::GetInstance()->GetFilePath() + "volume.xml";
	doc.SaveFile(filepath.c_str());
}

/////////////Factory Methods//////////////
//void GamePlayState::CreatePlayer(SpawnPoint* sp)
void GamePlayState::CreatePlayer(SpawnPoint* sp)
{
	Player* plyr = new Player();
	if (LoadGameState::GetInstance()->IsGameLoaded() && LoadGameState::GetInstance()->LoadSlot() != 0)
	{
		LoadGameState::GetInstance()->LoadSlot(plyr, LoadGameState::GetInstance()->LoadSlot());
	}
	else
	{
		plyr->SetGameLevel(1);
		plyr->SetHookLevel(1);
		plyr->SetSwordLevel(1);
	}
	Playerani.currAnimation = "Left";
	plyr->SetLeftAnimation(Playerani);
	Playerani.currAnimation = "UP";
	plyr->SetUpAnimation(Playerani);
	Playerani.currAnimation = "Down";
	plyr->SetDownAnimation(Playerani);
	plyr->SetSpawnPoint(sp);
	plyr->SetPosition(sp->GetPosition());

	m_pEntities->AddEntity(plyr, BUCKET_PLAYER);
	m_Player = plyr;
}

//Entity* GamePlayState::CreateZombie(SGD::Point pos)
void GamePlayState::CreateZombie(SpawnPoint* sp)
{
	Zombie* zom = new Zombie(false);
	zom->SetPosition(sp->GetPosition());
	zom->SetSpawnPoint(sp);

	Zombieani.currAnimation = "ZombieAttackDown";
	zom->SetAttackDown(Zombieani);
	Zombieani.currAnimation = "ZombieAttackUp";
	zom->SetAttackUp(Zombieani);
	Zombieani.currAnimation = "ZombieAttack";
	zom->SetAttackLeft(Zombieani);

	Zombieani.currAnimation = "ZombieWalkDown";
	zom->SetWalkDown(Zombieani);
	Zombieani.currAnimation = "ZombieWalkUp";
	zom->SetWalkUp(Zombieani);
	Zombieani.currAnimation = "ZombieWalk";
	zom->SetWalkLeft(Zombieani);

	Zombieani.currAnimation = "ZombieStealDown";
	zom->SetStealDown(Zombieani);
	Zombieani.currAnimation = "ZombieStealUp";
	zom->SetStealUp(Zombieani);
	Zombieani.currAnimation = "ZombieSteal";
	zom->SetStealLeft(Zombieani);

	Zombieani.currAnimation = "ZombieDeath";
	zom->SetDeath(Zombieani);
	//return zom;
	for (unsigned int x = 0; x < sp->GetWaypoints().size(); x++)
		zom->AddWayPoint(sp->GetWaypoints()[x]);

	m_pEntities->AddEntity(zom, BUCKET_ENEMY);
	zom->Release();
}

//Entity* GamePlayState::CreateAmbushNinja(SGD::Point pos)
void GamePlayState::CreateAmbushNinja(SpawnPoint* sp)
{
	AmbushNinja* amb = new AmbushNinja();
	amb->SetPosition(sp->GetPosition());
	amb->SetSpawnPoint(sp);

	//return amb;
	m_pEntities->AddEntity(amb, BUCKET_ENEMY);
	amb->Release();
}

//Entity* GamePlayState::CreateNinjaThrower(SGD::Point pos)
void GamePlayState::CreateNinjaThrower(SpawnPoint* sp)
{
	ThrowingNinja* thr = new ThrowingNinja();
	thr->SetPosition(sp->GetPosition());
	//return thr;
	m_pEntities->AddEntity(thr, BUCKET_ENEMY);
	thr->Release();
}

//Entity* GamePlayState::CreateArmoredZombie(SGD::Point pos)
void GamePlayState::CreateArmoredZombie(SpawnPoint* sp)
{
	Zombie* azom = new Zombie(true);
	azom->SetPosition(sp->GetPosition());
	azom->SetSpawnPoint(sp);

	Zombieani.currAnimation = "ZombieAttackDown";
	azom->SetAttackDown(Zombieani);
	Zombieani.currAnimation = "ZombieAttackUp";
	azom->SetAttackUp(Zombieani);
	Zombieani.currAnimation = "ZombieAttack";
	azom->SetAttackLeft(Zombieani);

	Zombieani.currAnimation = "ZombieWalkDown";
	azom->SetWalkDown(Zombieani);
	Zombieani.currAnimation = "ZombieWalkUp";
	azom->SetWalkUp(Zombieani);
	Zombieani.currAnimation = "ZombieWalk";
	azom->SetWalkLeft(Zombieani);

	Zombieani.currAnimation = "ZombieStealDown";
	azom->SetStealDown(Zombieani);
	Zombieani.currAnimation = "ZombieStealUp";
	azom->SetStealUp(Zombieani);
	Zombieani.currAnimation = "ZombieSteal";
	azom->SetStealLeft(Zombieani);

	Zombieani.currAnimation = "ZombieDeath";
	azom->SetDeath(Zombieani);
	//return zom;
	for (unsigned int x = 0; x < sp->GetWaypoints().size(); x++)
		azom->AddWayPoint(sp->GetWaypoints()[x]);
	//return azom;
	m_pEntities->AddEntity(azom, BUCKET_ENEMY);
	azom->Release();
}

//Entity* GamePlayState::CreateMovableBox(SGD::Point pos)
void GamePlayState::CreateMovableBox(SpawnPoint* sp)
{
	MovableBox* mbox = new MovableBox();
	mbox->SetPosition(sp->GetPosition());
	//return mbox;
	m_pEntities->AddEntity(mbox, BUCKET_BOX_MOVABLE);
	mbox->Release();
}

//Entity* GamePlayState::CreateImmovableBox(SGD::Point pos)
void GamePlayState::CreateImmovableBox(SpawnPoint* sp)
{
	ImmovableBox* ibox = new ImmovableBox();

	ibox->SetPosition(sp->GetPosition() - ibox->GetSize() / 4);
	//return ibox;
	m_pEntities->AddEntity(ibox, BUCKET_BOX_IMMOVABLE);
	ibox->Release();
}

void GamePlayState::CreateNeutralEnemy(SpawnPoint* sp)
{
	NeutralEnemy* niem = new NeutralEnemy();

	niem->SetPosition(sp->GetPosition());
	niem->SetSpawnPoint(sp);

	//return zom;
	for (unsigned int x = 0; x < sp->GetWaypoints().size(); x++)
		niem->AddWayPoint(sp->GetWaypoints()[x]);

	m_pEntities->AddEntity(niem, BUCKET_ENEMY);
	niem->Release();
}

Lever* GamePlayState::CreateLever(SGD::Point pt, Entity* ToActivate)
{
	Lever* lvr = new Lever();
	lvr->SetPosition(pt);
	lvr->SetSize({ 32, 32 });
	lvr->SetObjectToActivate(ToActivate);

	m_pEntities->AddEntity(lvr, BUCKET_LEVER);
	return lvr;
	//lvr->Release();
}

PressurePoint* GamePlayState::CreatePressurePoint(SGD::Point pt)
{
	PressurePoint* lvr = new PressurePoint(pt);
	PressurPoints.currAnimation = "Compress";
	lvr->SetCompressedAnim(PressurPoints);
	PressurPoints.currAnimation = "Decompress";
	lvr->SetDecompressedAnim(PressurPoints);

	m_pEntities->AddEntity(lvr, BUCKET_LEVER);
	return lvr;
	//lvr->Release();
}

void GamePlayState::SaveInput()
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) || pInput->IsButtonPressed(0, 1))
		m_nCursor = 4;

	if (pInput->IsKeyPressed(SGD::Key::Enter) || pInput->IsButtonPressed(0, 0))
	{
		if (m_nCursor<4 && m_nCursor>0)
		{
			if (m_lrCursor == 1)
				SaveGame(m_nCursor);
			else
				SaveGame(0);
			LoadGameState::GetInstance()->LoadSlot(Slot1, 1);
			LoadGameState::GetInstance()->LoadSlot(Slot2, 2);
			LoadGameState::GetInstance()->LoadSlot(Slot3, 3);
		}
		else if (m_nCursor == 4)
		{
			m_lrCursor = 0;
			m_nCursor = 6;
			m_SaveGame = false;
		}
	}

	if ((pInput->IsKeyDown(SGD::Key::Up) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && m_cursorMoveTimer>0.3f)
	{
		if (m_nCursor == 1)
			m_nCursor = 4;
		else
			m_nCursor--;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Down) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && m_cursorMoveTimer > 0.3f)
	{
		if (m_nCursor == 4)
			m_nCursor = 1;
		else
			m_nCursor++;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Left) || pInput->IsDPadDown(0, SGD::DPad::Left) || pInput->GetLeftJoystick(0).x < 0) && m_cursorMoveTimer>0.3f)
	{
		if (m_lrCursor == 1)
			m_lrCursor = 2;
		else
			m_lrCursor--;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
	if ((pInput->IsKeyDown(SGD::Key::Right) || pInput->IsDPadDown(0, SGD::DPad::Right) || pInput->GetLeftJoystick(0).x > 0) && m_cursorMoveTimer > 0.3f)
	{
		if (m_lrCursor == 2)
			m_lrCursor = 1;
		else
			m_lrCursor++;
		m_cursorMoveTimer = 0;
		SGD::AudioManager::GetInstance()->PlayAudio(m_cursorMove);
	}
}

void GamePlayState::SaveMenu()
{
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hSaveMenu, { 0, 0 }, 0.0f, {}, {}, { .4f, .8f });

	SGD::Color SaveSlot1, SaveSlot2, SaveSlot3, Back, Overwrite, Delete;
	SaveSlot1 = SaveSlot2 = SaveSlot3 = Back = Overwrite = Delete = { 255, 255, 255 };

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	SGD::Rectangle rect;



	SGD::OStringStream gamelevel, strength, movespeed, hooklevel, swordlevel, numsb, daq;

	char buf[255];

	switch (m_nCursor)
	{
	case 1:
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
				  pFont->Draw(daq.str().c_str(), { 700, 515 }, 0.4f, { 255, 0, 0 });
			  }
			  else
			  {
				  rect = { 400, 200, 500, 250 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_EMPTY, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
			  }
			  if (m_lrCursor == 1)
				  Overwrite = { 0, 0, 255 };
			  else
				  Delete = { 0, 0, 255 };
			  SaveSlot1 = { 0, 0, 255 };
			  break;
	}
	case 2:
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
				  pFont->Draw(daq.str().c_str(), { 700, 515 }, 0.4f, { 255, 0, 0 });
			  }
			  else
			  {
				  rect = { 400, 200, 500, 250 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_EMPTY, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
			  }
			  if (m_lrCursor == 1)
				  Overwrite = { 0, 0, 255 };
			  else
				  Delete = { 0, 0, 255 };
			  SaveSlot2 = { 0, 0, 255 };
			  break;
	}
	case 3:
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
				  pFont->Draw(daq.str().c_str(), { 700, 515 }, 0.4f, { 255, 0, 0 });
			  }
			  else
			  {
				  rect = { 400, 200, 500, 250 };
				  SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 1);

				  LoadString(HInstance, IDS_EMPTY, buf, 255);
				  pFont->Draw(buf, { 410, 220 }, 0.4f, { 255, 0, 0 });
			  }
			  if (m_lrCursor == 1)
				  Overwrite = { 0, 0, 255 };
			  else
				  Delete = { 0, 0, 255 };
			  SaveSlot3 = { 0, 0, 255 };
			  break;
	}
	case 4:
	{
			  Back = { 0, 0, 255 };
			  break;
	}
	default:
		break;
	}


	LoadString(HInstance, IDS_SAVEGAME, buf, 255);
	pFont->Draw(buf, { 50, 50 }, 0.7f, { 255, 0, 0 });

	LoadString(HInstance, IDS_SAVESLOT1, buf, 255);
	pFont->Draw(buf, { 50, 200 }, 0.5f, SaveSlot1);

	LoadString(HInstance, IDS_SAVESLOT2, buf, 255);
	pFont->Draw(buf, { 50, 250 }, 0.5f, SaveSlot2);

	LoadString(HInstance, IDS_SAVESLOT3, buf, 255);
	pFont->Draw(buf, { 50, 300 }, 0.5f, SaveSlot3);

	LoadString(HInstance, IDS_EXIT, buf, 255);
	pFont->Draw(buf, { 50, 350 }, 0.5f, Back);

	LoadString(HInstance, IDS_OVERWRITE, buf, 255);
	pFont->Draw(buf, { 50, 450 }, 0.4f, Overwrite);

	LoadString(HInstance, IDS_DELETE, buf, 255);
	pFont->Draw(buf, { 250, 450 }, 0.4f, Delete);
}

void GamePlayState::SaveGame(int slot)
{
	//create tinyXML Document
	TiXmlDocument doc;

	//create a tinyXML Declaration
	TiXmlDeclaration* pDecl = new TiXmlDeclaration{ "1.0", "utf-8", "" };

	//link the declaration to the doc
	doc.LinkEndChild(pDecl);

	//all the levels
	TiXmlElement* pRoot = new TiXmlElement{ "Save" };

	//link the root to the document
	doc.LinkEndChild(pRoot);

	TiXmlElement* GameLevel = new TiXmlElement{ "GameLevel" };
	pRoot->LinkEndChild(GameLevel);

	if (slot != 0)
		GameLevel->SetAttribute("GameLevel", m_Player->GetGameLevel());
	else
		GameLevel->SetAttribute("GameLevel", 0);

	TiXmlElement* Stats = new TiXmlElement{ "Stats" };
	pRoot->LinkEndChild(Stats);

	if (m_Player->GetStrength() <= 30)
		Stats->SetAttribute("Strength", m_Player->GetStrength());
	else
		Stats->SetAttribute("Strength", 30);

	if (m_Player->GetMoveSpeed() <= 300)
		Stats->SetAttribute("MoveSpeed", (int)m_Player->GetMoveSpeed());
	else
		Stats->SetAttribute("MoveSpeed", 300);

	if (m_Player->GetMaxHP() <= 300)
		Stats->SetAttribute("MaxHp", m_Player->GetMaxHP());
	else
		Stats->SetAttribute("MaxHp", 300);
	Stats->SetAttribute("CurrHp", m_Player->GetCurrHP());

	TiXmlElement* Items = new TiXmlElement{ "Items" };
	pRoot->LinkEndChild(Items);

	Items->SetAttribute("HookLevel", m_Player->GetHookLevel());
	Items->SetAttribute("SwordLevel", m_Player->GetSwordLevel());
	Items->SetAttribute("SmokeBombs", m_Player->GetNumSmokeBombs());
	Items->SetAttribute("DAQ", m_Player->GetDAQ());
	int num = m_Player->HaveHook();
	Items->SetAttribute("HaveHook", num);
	num = m_Player->HaveSword();
	Items->SetAttribute("HaveSword", num);
	num = m_Player->GetHavePizza();
	Items->SetAttribute("HavePizza", num);

	string filepath = Game::GetInstance()->GetFilePath();
	
	switch (slot)
	{
	case 0:
	{
			  if (m_nCursor == 1)
			  {
				  filepath += "SaveSlot1.xml";
				  doc.SaveFile(filepath.c_str());
			  }
			  else if (m_nCursor == 2)
			  {
				  filepath += "SaveSlot2.xml";
				  doc.SaveFile(filepath.c_str());
			  }
			  else if (m_nCursor == 3)
			  {
				  filepath += "SaveSlot3.xml";
				  doc.SaveFile(filepath.c_str());
			  }
			  break;
	}
	case 1:
	{
			  filepath += "SaveSlot1.xml";
			  doc.SaveFile(filepath.c_str());
			  break;
	}
	case 2:
	{
			  filepath += "SaveSlot2.xml";
			  doc.SaveFile(filepath.c_str());
			  break;
	}
	case 3:
	{
			  filepath += "SaveSlot3.xml";
			  doc.SaveFile(filepath.c_str());
			  break;
	}
	default:
		break;
	}

}

Entity* GamePlayState::CreateFireTrap(Tile* tile)
{
	FireTrap* trap = new FireTrap();
	trap->SetPosition(tile->GetPosition());
	trap->SetSize(tile->GetSize());
	trap->SetDamage(1);
	trap->SetTrapType(Trap::TrapType::TRAP_FIRE);
	trap->SetActive(true);

	m_pEntities->AddEntity(trap, BUCKET_TRAP);
	//trap->Release();
	return trap;
}

Entity* GamePlayState::CreateElectricTrap(Tile* tile)
{
	LightingTrap* trap = new LightingTrap();
	trap->SetPosition(tile->GetPosition());
	trap->SetSize(tile->GetSize());
	trap->SetDamage(3);
	trap->SetTrapType(Trap::TrapType::TRAP_LIGHTING);
	trap->SetActive(true);

	m_pEntities->AddEntity(trap, BUCKET_TRAP);
	//trap->Release();
	return trap;
}

Entity* GamePlayState::CreateSpikeTrap(Tile* tile)
{
	SpikeTrap* trap = new SpikeTrap();
	trap->SetPosition(tile->GetPosition());
	trap->SetSize(tile->GetSize());
	trap->SetDamage(25);
	trap->SetTrapType(Trap::TrapType::TRAP_SPIKE);
	trap->SetActive(true);

	m_pEntities->AddEntity(trap, BUCKET_TRAP);
	//trap->Release();
	return trap;
}

Entity* GamePlayState::CreateFakeTrap(Tile* tile)
{
	FakeTrap* trap = new FakeTrap();
	trap->SetPosition(tile->GetPosition());
	trap->SetSize(tile->GetSize());
	trap->SetDamage(0);
	trap->SetTrapType(Trap::TrapType::TRAP_FAKE);
	trap->SetActive(true);
	trap->SetTile(tile);

	m_pEntities->AddEntity(trap, BUCKET_TRAP);
	//trap->Release();
	return trap;
}

Entity* GamePlayState::CreateSpawnTrap(Tile* tile)
{
	SpawnTrap* mytrap = new SpawnTrap(SpawnPoint::ZOMBIE, tile->GetPosition(), 1.5f, 3);
	m_pEntities->AddEntity(mytrap, BUCKET_TRAP);
	//mytrap->Release();
	return mytrap;
}

Entity* GamePlayState::CreateHDoor(SpawnPoint* sp)
{
	Door* dr = new Door(false);
	dr->SetPosition(sp->GetPosition());
	dr->SetSize({ 64, 32 });
	dr->SetActive(true);
	dr->SetVertical(false);
	hdoors.currAnimation = "dooropenH";
	dr->ts1 = hdoors;
	hdoors.currAnimation = "doorcloseH";
	dr->ts2 = hdoors;
	hdoors.currAnimation = "doorunlockH";
	dr->ts3 = hdoors;
	m_pEntities->AddEntity(dr, BUCKET_DOOR);
	//dr->Release();
	return dr;
}

Entity* GamePlayState::CreateVDoor(SpawnPoint* sp)
{
	Door* dr = new Door(true);
	dr->SetPosition(sp->GetPosition());
	dr->SetSize({ 32, 64 });
	dr->SetActive(true);
	dr->SetVertical(true);
	vdoors.currAnimation = "dooropenV";
	dr->ts1 = vdoors;
	vdoors.currAnimation = "doorcloseV";
	dr->ts2 = vdoors;
	vdoors.currAnimation = "doorunlockV";
	dr->ts3 = vdoors;
	m_pEntities->AddEntity(dr, BUCKET_DOOR);
	//dr->Release();
	return dr;
}

Entity* GamePlayState::CreateBullet(ThrowingNinja* pOwner) const
{
	Bullet* newBullet = new Bullet(pOwner->GetPosition(), pOwner->GetRotation(), pOwner);
	newBullet->SetImage(m_Bullet);
	newBullet->SetSize({ 16, 16 });
	SGD::Vector bVec = { 0, -1 };
	bVec.Rotate(pOwner->GetRotation());
	/*if (this->GetSpeed() > 0)
	{
	bVec *= (200 + this->GetSpeed());
	}
	else
	{
	}*/
	bVec *= 200;

	newBullet->SetVelocity(bVec);

	return newBullet;
}

Entity* GamePlayState::CreateHeart(LustWrath* pOwner) const
{
	HeartBullet* newBullet = new HeartBullet(pOwner->GetPosition(), pOwner->GetRotation(), pOwner);
	newBullet->SetImage(m_Heart);
	newBullet->SetSize({ 16, 16 });
	SGD::Vector bVec = { 0, -1 };
	bVec.Rotate(pOwner->GetRotation());

	bVec *= 200;

	newBullet->SetVelocity(bVec);

	if (m_Player != nullptr)
		newBullet->SetTarget(m_Player);

	return newBullet;
}

void GamePlayState::CreatMessage(WhatBox boxnames)
{
	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

	char buf[255];


	switch (boxnames)
	{
	case _ItemBox:
	{
					 if (!poweruponce)
					 {
						 tipisup = true;
						 LoadString(HInstance, IDS_ITEMHELP, buf, 255);
						 tiptext = buf; // "This is a power up!  There \nare manymore that you will find \nthrought the game.  Kill enemies \nor find them in boxes to get more! \n\t\t(space to exit)";
						 poweruponce = true;
					 }
					 break;
	}
	case _HookBox:
	{
					 tipisup = true;
					 LoadString(HInstance, IDS_HOOKHELP, buf, 255);
					 tiptext = buf; // "This is your hook!  You will need \nthis for just about everything. You \ncan grab onto all boxes, try it out!\nPress (K) to use\n\t\t(space to exit)";
					 break;
	}
	case _SwordBox:
	{
					  tipisup = true;
					  LoadString(HInstance, IDS_SWORDHELP, buf, 255);
					  tiptext = buf; //"This is your sword!  You will need \nthis for fighting if you want to.  \nLook! A Zombie KILL HIM!\nPress (J) to use\n\t\t(space to exit)";
					  break;
	}
	case _HintBox:
	{
					 if (m_Player->GetGameLevel() == 1)
					 {
						 if (!(m_Player->HaveHook() && m_Player->HaveSword()))
						 {
							 LoadString(HInstance, IDS_NINJATOOLHINT, buf, 255);
							 tiptext = buf; // "Find all your ninja tools and \nkeep moving forward!  \n\t\t(space to exit)";
							 tipisup = true;
						 }
						 else
						 {
							 finalhint = true;
						 }
					 }
					 //else if (m_Player->GetGameLevel() != 1)
					 //{
						// if ((m_Player->HaveHook() && m_Player->HaveSword()) && !m_Player->GetHasKey())
						// {
						//	 LoadString(HInstance, IDS_KEYHINT, buf, 255);
						//	 tiptext = buf; // "Find the boss key to open his door!  \n\t\t(space to exit)";
						//	 tipisup = true;
						// }
						// else if (m_Player->GetHasKey())
						// {
						//	 LoadString(HInstance, IDS_GETKEYHINT, buf, 255);
						//	 tiptext = buf; // "Your ready to fight the boss!  \nYou can do it!! \n\t\t(space to exit)";
						//	 tipisup = true;
						// }
						// else
						// {
						//	 finalhint = true;
						// }
					 //}
					 break;
	}
	case _TrapBox:
	{
					 if (!traponce && m_Player->GetGameLevel() == 1)
					 {
						 tipisup = true;
						 LoadString(HInstance, IDS_TRAPHELP, buf, 255);
						 tiptext = buf; //"This is A Trap!  You will need to \nbe careful for these. They are \neverywhere, all look diffrent too!\n\t\t(space to exit)";
						 traponce = true;
					 }
					 break;
	}
	case _BossKey:
	{
					 if (!bosskeyonce)
					 {
						 tipisup = true;
						 LoadString(HInstance, IDS_BOSSKEYHINT, buf, 255);
						 tiptext = buf; //"This is the boss room's key!  You \nneed this to enter his room.  There \nis only one on each floor!  \nGood Luck!\n\t\t(space to exit)";
						 bosskeyonce = true;
					 }
					 break;
	}
	case _PizzaBox:
	{
					  if (!pizzaonce)
					  {
						  tipisup = true;
						  LoadString(HInstance, IDS_PIZZAHELP, buf, 255);
						  tiptext = buf;//"This is your pizza! You need \nto protect it at all costs!  If \nit gets stolen and leaves the screen \nit game over!(You can hook it too) \n\t\t(space to exit)";
						  pizzaonce = true;
					  }
					  break;
	}
	case _SmokeBomb:
	{
					   if (!smokeonce)
					   {
						   tipisup = true;
						   LoadString(HInstance, IDS_SMOKEHELP, buf, 255);
						   tiptext = buf; // "This is a Smoke bomb!  These can be used to hide from the enemies  \nPress (L) to use\n\t\t(space to exit)";
						   smokeonce = true;
					   }
					   break;
	}
	}
}

void GamePlayState::ShowMap()
{
	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();
	char buf[255];

	const BitmapFont* pFont = Game::GetInstance()->GetFont();




	float x, y;
	x = Game::GetInstance()->GetWorldCamera().right / 800;
	y = Game::GetInstance()->GetWorldCamera().bottom / 512;

	switch (m_Level)
	{
	case(1) :
	{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hMap, { 0, 0 }, 0.0f, {}, {}, { 0.777f, 1.175f });
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hNinjaIc, { ((x - 1) * 134) + 34, 270 }, 0.0f, {}, {}, { 0.13f, 0.13f });

				break;
	}
	case(2) :
	{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hMap, { 0, 0 }, 0.0f, {}, {}, { 0.777f, 1.175f });
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hNinjaIc, { ((x - 1) * 134) + 34, ((y - 1) * 128) + 64 }, 0.0f, {}, {}, { 0.13f, 0.13f });

				break;
	}
	case(3) :
	{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hMap, { 0, 0 }, 0.0f, {}, {}, { 0.777f, 1.175f });
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hNinjaIc, { ((x - 1) * 114) + 26, ((y - 1) * 128) + 70 }, 0.0f, {}, {}, { 0.13f, 0.13f });
				break;
	}
	case(4) :
	{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hMap, { 0, 0 }, 0.0f, {}, {}, { 0.777f, 1.175f });
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hNinjaIc, { ((x - 1) * 114) + 226 + (x - 1) * 20, ((y - 1) * 128) + 200 }, 0.0f, {}, {}, { 0.13f, 0.13f });
				break;
	}
	case(5) :
	{
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hMap, { 0, 0 }, 0.0f, {}, {}, { 0.777f, 1.175f });
				SGD::GraphicsManager::GetInstance()->DrawTexture(m_hNinjaIc, { ((x - 1) * 114) + 250 + (x - 1) * 50, ((y - 1) * 128) + 150 }, 0.0f, {}, {}, { 0.13f, 0.13f });
				break;
	}
	}

	LoadString(HInstance, IDS_MAP, buf, 255);

	pFont->Draw(buf, { 90, 53 }, 0.8f, { 255, 255, 255 });
	pFont->Draw(buf, { 85, 53 }, 0.8f, { 0, 0, 255 });


	LoadString(HInstance, IDS_PRESSTOCO, buf, 255);

	pFont->Draw(buf, { 205, 550 }, 0.8f, { 255, 255, 255 });
	pFont->Draw(buf, { 200, 550 }, 0.8f, { 0, 0, 255 });

}

void GamePlayState::SetPlayer(Player* player)
{
	if (m_Player != nullptr)
		m_Player->Release();

	m_Player = player;

	if (m_Player != nullptr)
		m_Player->AddRef();
}

void GamePlayState::CreatePride(SpawnPoint* sp)
{
	Pride* pride = new Pride();
	pride->SetPosition(sp->GetPosition());
	pride->SetTarget(m_Player);
	Pridesani.currAnimation = "PrideDeath";
	pride->SetDyingAnimation(Pridesani);
	Pridesani.currAnimation = "PrideMove";
	pride->SetWalkingAnimation(Pridesani);
	Pridesani.currAnimation = "PrideTaunt";
	pride->SetTauntingAnimation(Pridesani);
	Pridesani.currAnimation = "PrideAttack";
	pride->SetAttackingAnimation(Pridesani);
	Pridesani.currAnimation = "PrideIdle";
	pride->SetIdleAnimation(Pridesani);
	Pridesani.currAnimation = "TakeAKnee";
	pride->SetKneeAnimation(Pridesani);
	m_pEntities->AddEntity(pride, BUCKET_PRIDE);
	pride->Release();
}

void GamePlayState::CreateLustWrath(SpawnPoint* sp)
{
	LustWrath* boss2 = new LustWrath();
	boss2->SetPosition(sp->GetPosition());
	boss2->SetSize({ 64, 64 });
	//Lustani.currAnimation = "LustDeath";
	//boss2->SetDyingAnimation(Lustani);
	Lustani.currAnimation = "Walk_Left";
	boss2->SetWalkingAnimation(Lustani);
	Lustani.currAnimation = "LadyKiss";
	boss2->SetKissingAnimation(Lustani);
	Lustani.currAnimation = "Whip";
	boss2->SetAttackingAnimation(Lustani);
	Lustani.currAnimation = "LadyIdle";
	boss2->SetIdleAnimation(Lustani);
	boss2->SetTarget(m_Player);
	m_pEntities->AddEntity(boss2, BUCKET_LUST);
	boss2->Release();
}

void GamePlayState::CreateGluttony(SpawnPoint* sp)
{
	GluttonyGreed* boss1 = new GluttonyGreed();
	boss1->SetPosition(sp->GetPosition());
	boss1->SetSize({ 64, 64 });
	boss1->SetTarget(m_Player);
	Greedani.currAnimation = "GreedRun";
	boss1->SetRunAnin(Greedani);
	Greedani.currAnimation = "GreedTired";
	boss1->SetTiredAnin(Greedani);
	m_pEntities->AddEntity(boss1, BUCKET_GLUTTONY);
	boss1->Release();
}

void GamePlayState::CreateBossKey(SpawnPoint* sp)
{
	BossKey* bKEy = new BossKey();
	bKEy->SetPosition(sp->GetPosition());
	m_pEntities->AddEntity(bKEy, BUCKET_PICKUP);
	bKEy->Release();
}

void GamePlayState::LoadScreen()
{
	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();
	char buf[255];

	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_LoadScreen, { 0, 0 }, 0.0f, {}, {}, { 0.4f, 0.5f });

	if (m_Hint == 1)
	{
		LoadString(HInstance, IDS_TIP1_1, buf, 255);
		pFont->Draw(buf, { 10, 100 }, 0.7f, {});
		LoadString(HInstance, IDS_TIP1_2, buf, 255);
		pFont->Draw(buf, { 100, 150 }, 0.7f, {});
	}
	else if (m_Hint == 2)
	{
		LoadString(HInstance, IDS_TIP2_1, buf, 255);
		pFont->Draw(buf, { 10, 100 }, 0.7f, {});
		LoadString(HInstance, IDS_TIP2_2, buf, 255);
		pFont->Draw(buf, { 375, 150 }, 0.7f, {});
	}

	if (!m_doneLoad)
	{
		LoadString(HInstance, IDS_LOADING, buf, 255);
		pFont->Draw(buf, { 350, 550 }, 0.7f, {});
	}
	else
	{

		LoadString(HInstance, IDS_LOADDONE, buf, 255);
		pFont->Draw(buf, { 150, 550 }, 0.7f, {});
	}
}