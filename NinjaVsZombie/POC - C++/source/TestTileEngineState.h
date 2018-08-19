///Used to test the tile engine

#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"

class LevelManager;

class TestTileEngineState :
	public IGameState
{
public:
	static TestTileEngineState* GetInstance(void);


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
	TestTileEngineState(void) = default;
	virtual ~TestTileEngineState(void) = default;

	TestTileEngineState(const TestTileEngineState&) = delete;
	TestTileEngineState& operator= (const TestTileEngineState&) = delete;

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Cursor Index
	//int		m_nCursor = 0;

	LevelManager* manager;

};

