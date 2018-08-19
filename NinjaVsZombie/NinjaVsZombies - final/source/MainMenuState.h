/***************************************************************
|	File:		MainMenuState.h
|	Author:		Jagoba Macos
|	Course:		
|	Purpose:	MainMenuState class handles the main menu
***************************************************************/

#pragma once

#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"

//class ParticleManager;
class BitmapFont;
class MainMenuState :
	public IGameState
{
public:
	static MainMenuState* GetInstance(void);


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Input(void)				override;	// handle user input
	virtual void	Update(float elapsedTime)	override;	// update entites
	virtual void	Render(void)				override;	// render entities / menu

	SGD::HAudio		GetMusic()	{ return m_hMusic; }
private:
	/**********************************************************/
	// SINGLETON!
	MainMenuState(void) = default;
	virtual ~MainMenuState(void) = default;

	MainMenuState(const MainMenuState&) = delete;
	MainMenuState& operator= (const MainMenuState&) = delete;

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;
	SGD::HAudio		m_cursorMove = SGD::INVALID_HANDLE;	


	//ParticleManager*	myManager;
	
	float time = 0.0f;
	float StudioTimer = 0.0f;
	float LogoTimer = 0.0f;

	bool studio = true;
	bool logo = false;

	SGD::HTexture StudioPic = SGD::INVALID_HANDLE;
	SGD::HTexture LogoPic = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;
};

