#pragma once

#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"


class SurvivalPlayState :
	public IGameState
{
public:
	static SurvivalPlayState* GetInstance(void);


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
	SurvivalPlayState(void) = default;
	virtual ~SurvivalPlayState(void) = default;

	SurvivalPlayState(const MainMenuState&) = delete;
	SurvivalPlayState& operator= (const MainMenuState&) = delete;

	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;

};

