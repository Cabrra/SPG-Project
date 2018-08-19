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
	
	float time = 0.0f;
	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;
};

