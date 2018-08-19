#pragma once

#include "IGameState.h"

#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"

class LoadGameState: public IGameState
{
public:
	static LoadGameState* GetInstance(void);


	/**********************************************************/
	// IGameState Interface:
	virtual void	Enter(void)				override;		// load resources
	virtual void	Exit(void)				override;		// unload resources

	virtual bool	Input(void)				override;		// handle user input
	virtual void	Update(float elapsedTime)	override;	// update entites
	virtual void	Render(void)				override;	// render entities / menu


private:
	/**********************************************************/
	// SINGLETON!
	LoadGameState(void) = default;
	virtual ~LoadGameState(void) = default;

	LoadGameState(const LoadGameState&) = delete;
	LoadGameState& operator= (const LoadGameState&) = delete;

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;
	SGD::HAudio		m_cursorMove = SGD::INVALID_HANDLE;

	SGD::Rectangle	m_saveSlot1, m_saveSlot2, m_saveSlot3;

	struct PlayerInfo
	{
		unsigned int hooklevel, swordlevel, Gamelevel;
		unsigned int MaxHealth, CurrHealth;
		bool HaveSword, HaveHook, HavePizza;
		SGD::Point Position;
	};

	float time = 0.0f;
	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;
};