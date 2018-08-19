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

	SGD::HTexture	m_hBackground = SGD::INVALID_HANDLE;

	/**********************************************************/
	// Cursor Index
	int		m_nCursor = 0;
};

