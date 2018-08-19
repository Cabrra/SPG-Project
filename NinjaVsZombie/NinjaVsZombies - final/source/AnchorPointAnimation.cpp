/***************************************************************
|	File:		AnchorPointAnimation.cpp
|	Author:		Jagoba Marcos
|	Course:
|	Purpose:	AnchorPointAnimation class runs a variable-size
|				animation, storing the source rects for each frame
|				and an 'anchor-point' == relative position
***************************************************************/

#include "AnchorPointAnimation.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "AnimationManager.h"
#include <cassert>


/**************************************************************/
// Initialize
//	- hardcode the animation
void AnchorPointAnimation::Initialize(void)
{

}

/**************************************************************/
// Terminate
//	- unload the resources
void AnchorPointAnimation::Terminate(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}


/**************************************************************/
// Update
//	- run the animation timer
void AnchorPointAnimation::Update(float elapsedTime)
{
	// Is the animation paused?
	if (m_bPlaying == false)
		return;


	// Increase the timer
	m_fTimeWaited += elapsedTime * m_fSpeed;

	// Is it time to move to the next frame?
	if (m_fTimeWaited >= m_vFrames[m_nCurrFrame].fDuration)
	{
		m_fTimeWaited = 0.0f;
		++m_nCurrFrame;
		if (m_vFrames[m_nCurrFrame - 1].eventname != "")
		{
			SGD::Event* myEvent = new SGD::Event{ "FrameTest" };
			myEvent->QueueEvent(nullptr);
			myEvent = nullptr;
		}

		// Has it reached the end?
		if (m_nCurrFrame == m_vFrames.size())
		{
			// Should the animation loop from the beginning?
			if (m_bLooping == true)
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
//	- draw the current frame offset from the given position
void AnchorPointAnimation::Render(SGD::Point position, bool flipped,
	SGD::Size scale, SGD::Color color) const
{
	// Flipped Image:
	//		(>'')>
	//		 |
	//	 <(''<)


	// Validate the image
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "AnchorPointAnimation::Render - image was not initialized!");


	// Is the color invisible? Or is the scale invisible?
	if (color.alpha == 0 || scale.width == 0.0f || scale.height == 0.0f)
		return;


	// Retrieve the source rect for the current frame
	SGD::Rectangle	frame = m_vFrames[m_nCurrFrame].rFrame;
	SGD::Point		anchor = m_vFrames[m_nCurrFrame].ptAnchor;
	anchor.x = (frame.right - frame.left) / 2;
	anchor.y = (frame.bottom - frame.top) / 2;


	// Store the X-scale for flipping
	float scaleX = scale.width;

	if (flipped == true)
		scaleX = -scaleX;


	// Offset the given position parameters by the 
	// frame's anchor-point to get to the top-left corner
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage,
		{ position.x - (anchor.x * scaleX),
		position.y - (anchor.y * scale.height) },
		frame,
		0.0f, {},
		color, { scaleX, scale.width });
}


/**************************************************************/
// GetRect
//	- return the frame rect at the given position
SGD::Rectangle	AnchorPointAnimation::GetRect(SGD::Point position, bool flipped,
	float scale) const
{
	// Retrieve the source rect for the current frame
	SGD::Rectangle	frame = m_vFrames[m_nCurrFrame].rFrame;
	SGD::Point		anchor = m_vFrames[m_nCurrFrame].ptAnchor;

	// Is it flipped?
	if (flipped == true)
	{
		SGD::Rectangle result = {};
		result.right = position.x + (anchor.x * scale);
		result.top = position.y - (anchor.y * scale);
		result.left = result.right - (frame.ComputeWidth()  * scale);
		result.bottom = result.top + (frame.ComputeHeight() * scale);
		return result;
	}
	else
	{
		SGD::Rectangle result = {};
		result.left = position.x - (anchor.x * scale);
		result.top = position.y - (anchor.y * scale);
		result.right = result.left + (frame.ComputeWidth()  * scale);
		result.bottom = result.top + (frame.ComputeHeight() * scale);
		return result;
	}
}


/**************************************************************/
// Restart
//	- start the animation over from frame 0
void AnchorPointAnimation::Restart(bool looping, float speed)
{
	// Store the parameters
	m_bLooping = looping;
	m_fSpeed = speed;

	// Reset animation
	m_nCurrFrame = 0;
	m_fTimeWaited = 0.0f;
	m_bPlaying = true;
	m_bFinished = false;
}

/**************************************************************/
// AddRef
//	- increase the reference count
/*virtual*/ void AnchorPointAnimation::AddRef(void)
{
	assert(m_unRefCount != 0xFFFFFFFF && "AnchorPointAnimation::AddRef - maximum reference count has been exceeded");

	++m_unRefCount;
}

/**************************************************************/
// Release
//	- decrease the reference count
//	- self-destruct when the count is 0
/*virtual*/ void AnchorPointAnimation::Release(void)
{
	--m_unRefCount;
	Terminate();

	if (m_unRefCount == 0)
	{
		delete this;
	}
}

void AnchorPointAnimation::SetLooping(int _looping)
{
	if (_looping == 1)
	{
		m_bLooping = false;
	}
	else
	{
		m_bLooping = true;
	}
}
void AnchorPointAnimation::SetPlaying(int _Playing)
{
	if (_Playing == 0)
	{
		m_bPlaying = false;
	}
	else
	{
		m_bPlaying = true;
	}
}
void AnchorPointAnimation::SetFinished(int _Finsihed)
{
	if (_Finsihed == 0)
	{
		m_bFinished = false;
	}
	else
	{
		m_bFinished = true;
	}
}