/***************************************************************
|	File:		HowToPlayState.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	HowToPlayState class handles the how to play menu
***************************************************************/

#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"

class HowToPlayState :
	public IGameState
{
public:
	static HowToPlayState* GetInstance(void);


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
	HowToPlayState(void) = default;
	virtual ~HowToPlayState(void) = default;

	HowToPlayState(const HowToPlayState&) = delete;
	HowToPlayState& operator= (const HowToPlayState&) = delete;

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;

	//items
	SGD::HTexture	m_hHook = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hPizza = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hSword = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hSmoke = SGD::INVALID_HANDLE;
	//key layout
	SGD::HTexture	m_hXbox = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hKeyboard = SGD::INVALID_HANDLE;
	SGD::HTexture	m_hArcade = SGD::INVALID_HANDLE;

	/**********************************************************/
	//float time = 0.0f;
	bool showXbox, showArcade, showKeyboard  = false;
};