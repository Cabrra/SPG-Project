/***************************************************************
|	File:		OptionState.cpp
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	OptionState class handles the options menu
***************************************************************/

#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"

#include "../SGD Wrappers/SGD_Color.h"


class OptionState :
	public IGameState
{
public:
public:
	static OptionState* GetInstance(void);


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
	OptionState(void) = default;
	virtual ~OptionState(void) = default;

	OptionState(const OptionState&) = delete;
	OptionState& operator= (const OptionState&) = delete;

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;
	SGD::HAudio		m_cursorMove = SGD::INVALID_HANDLE;

	SGD::Color SFXVol, MUSICVol, SFXMute, MUSICMute, FullScreen, OnFS, OffFS, ExitOptions, EnglishLang, SpanLang, LanguageOption;

	bool m_IsFullScreen = true;

	float m_cursorMoveTimer = 0.0f;
	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;
	bool	m_english = true;
};


