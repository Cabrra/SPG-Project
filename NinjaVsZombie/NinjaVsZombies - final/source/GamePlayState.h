/***************************************************************
|	File:		GamePlayState.cpp
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	GamePlayState class handles the game play state
***************************************************************/
#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_String.h"
#include "TimeStamp.h"
#include "ThrowingNinja.h"
#include "Game.h"
/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Entity;
class EntityManager;
class Player;
class Enemy;
class BitmapFont;
class LevelManager;
class AnchorPointAnimation;
class AnimationManager;
class ParticleManager;
class Pizza;
class Sword;
class Hook;
class SpawnPoint;
class AmbushNinja;
class PowerUp;
class Tile;
class FireTrap;
class LustWrath;
class Lever;
class PressurePoint;

class GamePlayState :
	public IGameState
{
public:
	static GamePlayState* GetInstance(void);
	enum WhatBox
	{
		_ItemBox, _HookBox, _SwordBox, _TrapBox, _HintBox, _PizzaBox, _BossKey, _SmokeBomb
	};

	enum Bucket{
		BUCKET_TRAP, BUCKET_DOOR,
		BUCKET_ENEMY_SHURIKEN,
		BUCKET_LEVER,
		BUCKET_BOX_MOVABLE,
		BUCKET_BOX_IMMOVABLE,
		BUCKET_PICKUP,
		BUCKET_PLAYER_ITEM,
		BUCKET_PLAYER,
		BUCKET_ENEMY,
		BUCKET_PRIDE, 
		BUCKET_SMOKE,
		BUCKET_LUST,
		BUCKET_ENEMY_HEART,
		BUCKET_GLUTTONY,
		BUCKET_PUZZLES,
	};

	void				CreatMessage(WhatBox boxnames);


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Input(void)					override;	// handle user input
	virtual void	Update(float elapsedTime)	override;	// update entites
	virtual void	Render(void)				override;	// render entities / menu

	Player*		GetPlayer()	{ return m_Player; }
	bool		GetAmbush() { return m_myAmbush; }

	void		SetPlayer(Player* player);
	void		SetAmbush(bool ambush) { m_myAmbush = ambush; }
	void		SetPause(bool newPass) { m_IsPaused = newPass; }

	//create object methods
	void CreatePlayer(SpawnPoint* sp);
	void CreateZombie(SpawnPoint* sp);
	void CreateAmbushNinja(SpawnPoint* sp);
	void CreateNinjaThrower(SpawnPoint* sp);
	void CreateArmoredZombie(SpawnPoint* sp);
	void CreateMovableBox(SpawnPoint* sp);
	void CreateImmovableBox(SpawnPoint* sp);
	Lever* CreateLever(SGD::Point pt, Entity* ToActivate);
	PressurePoint* CreatePressurePoint(SGD::Point pt);
	Entity* CreateHDoor(SpawnPoint* sp);
	Entity* CreateVDoor(SpawnPoint* sp);
	void CreateNeutralEnemy(SpawnPoint* sp);
	void CreatePride(SpawnPoint* sp);
	void CreateLustWrath(SpawnPoint* sp);
	void CreateGluttony(SpawnPoint* sp);
	void CreateBossKey(SpawnPoint* sp);
	///////////////////////////////
	Entity* CreateFireTrap(Tile* tile);
	Entity* CreateElectricTrap(Tile* tile);
	Entity* CreateSpikeTrap(Tile* tile);
	Entity* CreateFakeTrap(Tile* tile);
	Entity* CreateSpawnTrap(Tile* tile);

	void CreateHook(Entity* owner);
	void CreateSword(Entity* owner);
	void CreatePizza(Entity* owner, SGD::Point point = {});
	void CreateSmokeBomb(Entity* owner);

	void SpawnHook(Entity* owner, SGD::Point point = {});
	void SpawnSword(Entity* owner, SGD::Point point = {});
	void SpawnSmokeBomb(SGD::Point point = {});

	Entity* CreateBullet(ThrowingNinja* pOwner) const;
	Entity* CreateHeart(LustWrath* pOwner) const;

	LevelManager*	GetLevelManager()		{ return m_LevelManager; }
	EntityManager*	GetEntityManager()		{ return m_pEntities; }

	struct PlayerInfo
	{
		int gamelevel, hooklevel, swordlevel, NumSB, DAQ;
		int MaxHealth, CurrHealth, Strength, MoveSpeed;
	};

	bool Lose(){ return m_Lose; }
	bool Win() { return m_Win; }

	void SetLose(bool lose)	{ m_Lose = lose; }
	void SetWin(bool win)	{ m_Win = win; }

	void LoadScreen();

private:
	/**********************************************************/
	// SINGLETON!
	GamePlayState(void) = default;
	virtual ~GamePlayState(void) = default;

	GamePlayState(const GamePlayState&) = delete;
	GamePlayState& operator= (const GamePlayState&) = delete;

	void			CheckCamAndPlayer();

	/**********************************************************/
	// Create functions

	void				PauseMenu();
	void				PauseInput();
	void				SaveMenu();
	void				SaveInput();
	void				SaveGame(int slot);
	void				SaveVolume();
	void				ShowMap();

	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities		= nullptr;
	Player*					m_Player		= nullptr;
	Pizza*					m_Pizza			= nullptr;
	bool					m_myAmbush		= false;

	
	TimeStamp				Playerani;
	TimeStamp				Pridesani;
	TimeStamp				Greedani;
	TimeStamp				Zombieani;
	TimeStamp				Lustani;
	TimeStamp				PressurPoints;
	TimeStamp				vdoors;
	TimeStamp				hdoors;



	//AnimationManager*		m_Amanger;
	
	LevelManager*			m_LevelManager;

	//*******************************************************************
	// Message Handler / Callback Function:
	static void MessageProc(const SGD::Message* pMsg);

	/**********************************************************/
	// Cursor Index
	int						m_nCursor				= 0;
	int						m_lrCursor				= 1;
	int						m_Hint					= 0;
	int						m_Level					= 1;
	float					m_cursorMoveTimer		= 0.0f;
	bool					m_IsPaused				= false;
	bool					m_SaveGame				= false;
	bool					m_Win					= false;
	bool					m_Lose					= false;
	bool					m_english;
	bool					m_map					= false;
	bool					m_loading				= true;
	bool					m_doneLoad				= false;

	// game over 
	SGD::HAudio				m_win = SGD::INVALID_HANDLE;
	SGD::HAudio				m_lose = SGD::INVALID_HANDLE;
	SGD::HTexture			m_hwin = SGD::INVALID_HANDLE;
	SGD::HTexture			m_hlose = SGD::INVALID_HANDLE;
	float					m_fgameover = 0;
	bool playonetime = false;

	SGD::HAudio				m_cursorMove			= SGD::INVALID_HANDLE;
	SGD::HAudio				m_hMusic				= SGD::INVALID_HANDLE;
	SGD::HAudio				m_hLoadMusic			= SGD::INVALID_HANDLE;
	SGD::HAudio				m_DropPizza				= SGD::INVALID_HANDLE;
	SGD::HAudio				m_HookSound				= SGD::INVALID_HANDLE;
	SGD::HTexture			m_hBackground			= SGD::INVALID_HANDLE;
	SGD::HTexture			m_hPauseMenu			= SGD::INVALID_HANDLE;
	SGD::HTexture			m_hSaveMenu				= SGD::INVALID_HANDLE;
	SGD::HTexture			m_hMap					= SGD::INVALID_HANDLE;
	SGD::HTexture			m_hNinjaIc				= SGD::INVALID_HANDLE;
	SGD::HTexture			m_Bullet				= SGD::INVALID_HANDLE;
	SGD::HTexture			m_MAster				= SGD::INVALID_HANDLE;
	SGD::HTexture			m_Heart					= SGD::INVALID_HANDLE;
	SGD::HTexture			m_LoadScreen			= SGD::INVALID_HANDLE;

	// tip info
	SGD::Rectangle BoxRect = SGD::Rectangle(20, Game::GetInstance()->GetScreenHeight() - 355, Game::GetInstance()->GetScreenWidth() - 20, Game::GetInstance()->GetScreenHeight() - 155);
	bool tipisup = false;
	bool poweruponce = false;
	bool pizzaonce = false;
	bool bosskeyonce = false;
	bool traponce = false;
	bool finalhint = false;
	bool smokeonce = false;
	string tiptext;
	float helpbox = 0;

	// Save Game
	Player* Slot1;
	Player* Slot2;
	Player* Slot3;

	//SpawnPoint* ambushPoint = nullptr;

	////FPS
	//unsigned int dwFrames = 0;
	//DWORD dwCurrentTime;
	//DWORD dwLastUpdateTime;
	//DWORD dwElapsedTime;
	//SGD::String szFPS;
};
