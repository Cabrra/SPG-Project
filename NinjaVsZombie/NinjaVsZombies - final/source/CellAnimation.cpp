/***************************************************************
|	File:		CellAnimation.cpp
|	Author:		Jagoba Marcos
|	Course:		 
|	Purpose:	CellAnimation class runs a fixed-size animation
|				from frame 0 -> n-1
***************************************************************/

#include "CellAnimation.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>


/**************************************************************/
// Initialize
//	- hardcode the animation for "SGD_Anim_Explosion.png"
void CellAnimation::InitializePlayerWalk( void )
{
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
					L"resource/graphics/JMC_PLAYER_Walking.png" );

	m_nNumRows		= 1;
	m_nNumCols		= 6;
	m_nFrameWidth	= 31;
	m_nFrameHeight	= 32;
	
	m_nCurrFrame	= 0;
	m_nNumFrames	= 6;		// indices 0 -> 9
	
	m_fTimeWaited	= 0.0f;
	m_fDuration		= 0.1f;
	m_fSpeed		= 1.0f;
	
	m_bPlaying		= false;
	m_bLooping		= false;
	m_bFinished		= false;
}
void	CellAnimation::InitializePlayerDie(void){
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/JMC_PLAYER_Die.png");

	m_nNumRows = 1;
	m_nNumCols = 5;
	m_nFrameWidth = 31;
	m_nFrameHeight = 32;

	m_nCurrFrame = 0;
	m_nNumFrames = 5;		// indices 0 -> 9

	m_fTimeWaited = 0.0f;
	m_fDuration = 0.5F;
	m_fSpeed = 2.0f;

	m_bPlaying = false;
	m_bLooping = false;
	m_bFinished = false;
}
void	CellAnimation::InitializePoliceWalk(void){
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/JMC_Police_Walking.png");

	m_nNumRows = 1;
	m_nNumCols = 6;
	m_nFrameWidth = 31;
	m_nFrameHeight = 32;

	m_nCurrFrame = 0;
	m_nNumFrames = 6;		// indices 0 -> 9

	m_fTimeWaited = 0.0f;
	m_fDuration = 0.1f;
	m_fSpeed = 1.0f;

	m_bPlaying = false;
	m_bLooping = false;
	m_bFinished = false;
}
void	CellAnimation::InitializePoliceDie(void){
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/JMC_Police_Die.png");

	m_nNumRows = 1;
	m_nNumCols = 5;
	m_nFrameWidth = 31;
	m_nFrameHeight = 32;

	m_nCurrFrame = 0;
	m_nNumFrames = 5;		// indices 0 -> 9

	m_fTimeWaited = 0.0f;
	m_fDuration = 0.5F;
	m_fSpeed = 2.0F;

	m_bPlaying = false;
	m_bLooping = false;
	m_bFinished = false;
}
void	CellAnimation::InitializeCitizenWalk(void){
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/JMC_Citizen_Walking.png");

	m_nNumRows = 1;
	m_nNumCols = 6;
	m_nFrameWidth = 31;
	m_nFrameHeight = 32;

	m_nCurrFrame = 0;
	m_nNumFrames = 6;		// indices 0 -> 9

	m_fTimeWaited = 0.0f;
	m_fDuration = 0.1f;
	m_fSpeed = 2.0f;

	m_bPlaying = false;
	m_bLooping = false;
	m_bFinished = false;
}
void	CellAnimation::InitializeCitizenDie(void){
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/JMC_Citizen_Die.png");

	m_nNumRows = 1;
	m_nNumCols = 5;
	m_nFrameWidth = 31;
	m_nFrameHeight = 32;

	m_nCurrFrame = 0;
	m_nNumFrames = 5;		// indices 0 -> 9

	m_fTimeWaited = 0.0f;
	m_fDuration = 0.5F;
	m_fSpeed = 2.0F;

	m_bPlaying = false;
	m_bLooping = false;
	m_bFinished = false;
}

/**************************************************************/
// Terminate
//	- unload the resources
void CellAnimation::Terminate( void )
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture( m_hImage );
}


/**************************************************************/
// Update
//	- run the animation timer
void CellAnimation::Update( float elapsedTime )
{
	// Is the animation paused?
	if( m_bPlaying == false )
		return;


	// Increase the timer
	m_fTimeWaited += elapsedTime * m_fSpeed;

	// Is it time to move to the next frame?
	if( m_fTimeWaited >= m_fDuration )
	{
		m_fTimeWaited = 0.0f;
		++m_nCurrFrame;


		// Has it reached the end?
		if( m_nCurrFrame == m_nNumFrames )
		{
			// Should the animation loop from the beginning?
			if( m_bLooping == true )
				m_nCurrFrame = 0;
			else 
			{
				// Stop on the last valid frame
				--m_nCurrFrame;
				m_bPlaying = false;
				m_bFinished = true;
			}
		}
	}
}


/**************************************************************/
// Render
//	- draw the current frame at the given position
void CellAnimation::Render( SGD::Point position, float scale, SGD::Color color, float rotation, SGD::Vector offset ) const
{
	// Validate the image
	assert( m_hImage != SGD::INVALID_HANDLE 
			&& "CellAnimation::Render - image was not initialized!" );


	// Is the color invisible? Or is the scale invisible?
	if( color.alpha == 0 || scale == 0.0f )
		return;


	// Calculate the source rect for the current frame
	SGD::Rectangle frame;
	frame.left		= float( (m_nCurrFrame % m_nNumCols) * m_nFrameWidth  );
	frame.top		= float( (m_nCurrFrame / m_nNumCols) * m_nFrameHeight );
	frame.right		= frame.left + m_nFrameWidth;
	frame.bottom	= frame.top  + m_nFrameHeight;


	// Draw
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage, position, frame, 
		rotation, offset,
		color, {scale, scale});
}

//void CellAnimation::RenderDead(SGD::Point position, float scale, SGD::Color color, float rotation, SGD::Vector offset) const
//{
//	// Validate the image
//	assert(m_hImage != SGD::INVALID_HANDLE
//		&& "CellAnimation::Render - image was not initialized!");
//
//
//	// Is the color invisible? Or is the scale invisible?
//	if (color.alpha == 0 || scale == 0.0f)
//		return;
//
//	// Calculate the source rect for the current frame
//	SGD::Rectangle frame;
//	frame.left = float((m_nCurrFrame % m_nNumCols) * m_nFrameWidth);
//	frame.top = float((m_nCurrFrame / m_nNumCols) * m_nFrameHeight);
//	frame.right = frame.left + m_nFrameWidth;
//	frame.bottom = frame.top + m_nFrameHeight;
//
//
//	// Draw
//	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
//		m_hImage, position, frame,
//		rotation, offset,
//		color, { scale, scale });
//}


/**************************************************************/
// Restart
//	- start the animation over from frame 0
void CellAnimation::Restart( bool looping, float speed )
{
	// Store the parameters
	m_bLooping	= looping;
	m_fSpeed	= speed;

	// Reset animation
	m_nCurrFrame	= 0;
	m_fTimeWaited	= 0.0f;
	m_bPlaying		= true;
	m_bFinished		= false;
}
