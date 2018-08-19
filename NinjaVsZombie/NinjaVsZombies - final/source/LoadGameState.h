#pragma once

#include "IGameState.h"

#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


class Player;
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

	bool			IsGameLoaded()	{ return m_IsGameLoaded; }
	void			SetIsLoaded(bool load){ m_IsGameLoaded = load; }

	int				LoadSlot()		{ return m_Slot; }
	void			LoadSlot(Player* player, int slot);

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

	Player* Slot1;
	Player* Slot2;
	Player* Slot3;

	bool m_IsGameLoaded = false;
	int m_Slot = 0;
	float time = 0.0f;
	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;
};