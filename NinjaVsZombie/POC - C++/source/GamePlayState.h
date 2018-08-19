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
#include "TimeStamp.h"

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

class GamePlayState :
	public IGameState
{
public:
	static GamePlayState* GetInstance(void);


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Input(void)				override;	// handle user input
	virtual void	Update(float elapsedTime)	override;	// update entites
	virtual void	Render(void)				override;	// render entities / menu


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
	void				CreateHook(Entity* owner);
	void				CreateSword(Entity* owner);
	void				CreatePizza(Entity* owner);
	void				CreateSmokeBomb(Entity* owner);

	void				PauseMenu();
	void				PauseInput();
	void				SaveVolume();

	/**********************************************************/
	// Game Entities
	EntityManager*			m_pEntities		= nullptr;
	Player*					m_Player		= nullptr;
	TimeStamp				testing;
	TimeStamp				testing2;
	TimeStamp				retesting;


	//AnimationManager*		m_Amanger;
	
	LevelManager*			m_LevelManager;

	ParticleManager*		particleTest;

	//*******************************************************************
	// Message Handler / Callback Function:
	static void MessageProc(const SGD::Message* pMsg);

	/**********************************************************/
	// Cursor Index
	int						m_nCursor				= 0;
	float					m_cursorMoveTimer		= 0.0f;
	bool					m_IsPaused				= false;
	bool					m_english;

	SGD::HAudio				m_cursorMove			= SGD::INVALID_HANDLE;
	SGD::HAudio				m_hMusic				= SGD::INVALID_HANDLE;
	SGD::HAudio				m_DropPizza				= SGD::INVALID_HANDLE;
	SGD::HTexture			m_hBackground			= SGD::INVALID_HANDLE;
};

