/***************************************************************
|	File:		Game.h
|	Author:		Jagoba Macos
|	Course:		 
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/
#pragma once

#define CONVERTTORAD SGD::PI/180
#define CONVERTTODEG 180/SGD::PI
#define HUDHEIGHT 88
#define BASECOLOR	SGD::Color{100,100,100}

#include "../SGD Wrappers/SGD_Declarations.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include <Windows.h>	 
#include <string>

using namespace std;


enum ParticleOrder{ BLOSSOM_TOP, CLOUDS_RIGHT, CLOUDS_LEFT,
							SPEED_POWER, STRENGTH_POWER, SMOKEBOMB,
							FIRE_TRAP, ELECTRIC_TRAP , LIL_BOMB, BLOOD, };
/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class BitmapFont;
class IGameState;
class Entity;
class EntityManager;


/**************************************************************/
// Game class
//	- runs the game logic
//	- controls the SGD wrappers
//	- SINGLETON!
//		- there is only ONE instance
//		- global access method (GetInstance)
class Game
{
public:
	/**********************************************************/
	// Singleton Accessors:
	static Game*				GetInstance		( void );
	static void					DeleteInstance	( void );

	
	/**********************************************************/
	// Setup, Play, Cleanup:
	bool Initialize	( float width, float height );
	int	 Update		( void );
	void Terminate(void);
	
	
	/**********************************************************/
	// Screen Size Accessors:
	float				GetScreenWidth	( void ) const	{	return m_fScreenWidth;	}
	float				GetScreenHeight ( void ) const	{	return m_fScreenHeight;	} 

	//World Axcessors and Mutators
	SGD::Size		GetWorldSize()const		{ return m_fWorldSize; }
	SGD::Rectangle	GetWorldCamera()const	{ return m_fWorldCamera; }
	bool			IsFullscreen()const		{ return m_gFullScreen; }
	SGD::Point		GetWorldSpace()const	{ return m_WorldSpace; }
	HINSTANCE		GetHInstance()const		{ return m_hInstance; }
	WORD			GetLanguageID()const	{ return m_LanguageID; }
	SGD::HTexture	GetButtonBar()const		{ return m_hButtonbar; }
	SGD::HTexture	GetButtonBall()const	{ return m_hButtonball; }
	string			GetFilePath()const		{ return m_FilePath; }

	void	SetWorldSize(SGD::Size nSize)			{ m_fWorldSize = nSize; }
	void	SetWorldCamera(SGD::Rectangle nCam)		{ m_fWorldCamera = nCam; }
	void	SetFullscreen(bool full)				{ m_gFullScreen = full; }
	void	SetWorldSpace(SGD::Point point)			{ m_WorldSpace = point; }
	void	SetHInstance(HINSTANCE newHInstance)	{ m_hInstance = newHInstance; }
	void	SetLanguageID(WORD newID)				{ m_LanguageID = newID; }

	// Font Accessor: (#include "BitmapFont.h" to use!)
	const BitmapFont*	GetFont			( void ) const	{	return m_pFont;			}

	
	/**********************************************************/
	// Game State Machine:
	//	- can ONLY be called by the state's Input, Update, or Render methods!!!
	void ChangeState( IGameState* pNewState );


private:
	/**********************************************************/
	// Singleton Object:
	static Game*				s_pInstance;

	Game( void )	= default;		// default constructor
	~Game( void )	= default;		// destructor

	Game( const Game& )				= delete;	// copy constructor
	Game& operator= ( const Game& )	= delete;	// assignment operator


	/**********************************************************/
	// Screen Size
	float					m_fScreenWidth	= 1;
	float					m_fScreenHeight = 1;
	bool					m_gFullScreen	= true;

	string					m_FilePath		= "/0";

	//World Size and camera

	SGD::Size				m_fWorldSize	= { 1, 1 };
	SGD::Rectangle			m_fWorldCamera	= { 0, 0, 0, 0 };
	SGD::Point				m_WorldSpace	= { 0, 0 };

	/**********************************************************/
	// Game Font
	BitmapFont*				m_pFont			= nullptr;


	/**********************************************************/
	// Current Game State
	IGameState*				m_pCurrState	= nullptr;

	/**********************************************************/
	// Game Time
	unsigned long			m_ulGameTime	= 0;
	HINSTANCE				m_hInstance;
	WORD					m_LanguageID;
	

	SGD::HTexture	m_hButtonbar = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hButtonball = SGD::INVALID_HANDLE;
};
