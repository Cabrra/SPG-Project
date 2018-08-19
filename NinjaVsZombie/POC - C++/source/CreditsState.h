/***************************************************************
|	File:		CreditsState.h
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	CreditsState shows credits of everyone who helped
|				with the creation of the game
***************************************************************/

#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"

class CreditsState :
	public IGameState
{ 
public:
	static CreditsState* GetInstance(void);


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
	CreditsState(void) = default;
	virtual ~CreditsState(void) = default;

	CreditsState(const CreditsState&) = delete;
	CreditsState& operator= (const CreditsState&) = delete;

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;

	float time = 0.0f;
};
