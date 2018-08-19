/***************************************************************
|	File:		ShopState.cpp
|	Author:		Jagoba Macos
|	Course:
|	Purpose:	ShopState class handles the shop menu
***************************************************************/

#pragma once
#include "IGameState.h"
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Geometry.h"

class ShopState :
	public IGameState
{
public:
	static ShopState* GetInstance(void);


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
	ShopState(void) = default;
	virtual ~ShopState(void) = default;

	ShopState(const ShopState&) = delete;
	ShopState& operator= (const ShopState&) = delete;

	void MainInput();
	void StatsInput();
	void ItemsInput();

	void MainShopMenu();
	void StatsMenu();
	void ItemsMenu();

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;
	SGD::HTexture	m_Master = SGD::INVALID_HANDLE;
	SGD::HAudio		m_hMusic = SGD::INVALID_HANDLE;
	SGD::HAudio		m_cursorMove = SGD::INVALID_HANDLE;
	SGD::HAudio		m_Purchase = SGD::INVALID_HANDLE;
	SGD::HAudio		m_denied = SGD::INVALID_HANDLE;

	bool	m_Stats, m_Items;
	float time;

	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;
};

