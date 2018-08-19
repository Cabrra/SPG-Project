/***************************************************************
|	File:		CellAnimation.h
|	Author:		Jagoba Marcos
|	Course:		 
|	Purpose:	CellAnimation class runs a fixed-size animation
|				from frame 0 -> n-1
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"


/**************************************************************/
// CellAnimation class
//	- runs animation using an image of fixed-size frames
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)
class CellAnimation
{
public:
	/**********************************************************/
	// Default Constructor & Destructor
	CellAnimation( void )	= default;
	~CellAnimation( void )	= default;

	/**********************************************************/
	// Initialize & Terminate
	void	InitializePlayerWalk	( void );	// should have parameters or a config file
	void	InitializePlayerDie(void);
	void	InitializePoliceWalk(void);
	void	InitializePoliceDie(void);
	void	InitializeCitizenWalk(void);
	void	InitializeCitizenDie(void);

	void	Terminate	( void );


	/**********************************************************/
	// Animation Controls:
	void	Update		( float elapsedTime );
	void	Render(SGD::Point position, float scale, SGD::Color color, float rotation = 0.0f, SGD::Vector offset = {})	const;
	void	RenderDead(SGD::Point position, float scale, SGD::Color color, float rotation, SGD::Vector offset = {}) const;

	void	Restart		( bool looping, float speed );
	void	Pause		( bool pause )	{	m_bPlaying = !pause;	}

	bool	IsPlaying	( void ) const	{	return m_bPlaying;		}
	bool	IsFinished	( void ) const	{	return m_bFinished;		}

private:
	/**********************************************************/
	// image
	SGD::HTexture	m_hImage		= SGD::INVALID_HANDLE;

	// cell data
	int				m_nNumRows		= 0;
	int				m_nNumCols		= 0;
	int				m_nFrameWidth	= 0;
	int				m_nFrameHeight	= 0;

	// animation data
	int				m_nCurrFrame	= 0;
	int				m_nNumFrames	= 0;

	float			m_fTimeWaited	= 0.0f;
	float			m_fDuration		= 0.0f;		// per frame
	float			m_fSpeed		= 1.0f;		// multiplier: 2.0 - twice as fast

	bool			m_bPlaying		= false;
	bool			m_bLooping		= false;
	bool			m_bFinished		= false;
};

