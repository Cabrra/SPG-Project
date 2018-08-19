/***************************************************************
|	File:		AnchorPointAnimation.h
|	Author:		Jagoba Marcos
|	Course:
|	Purpose:	AnchorPointAnimation class runs a variable-size
|				animation, storing the source rects for each frame
|				and an 'anchor-point' == relative position
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include <string>
#include <vector>
#include <cassert>

class AnimationManager;

/**************************************************************/
// AnchorPointAnimation class
//	- runs animation using an image of variable-size frames
//	- stores a relative offset from the top-left corner of the frame rect 
//	  to the render position
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)
class AnchorPointAnimation
{
public:
	/**********************************************************/
	// Default Constructor & Destructor
	AnchorPointAnimation() = default;
	~AnchorPointAnimation() = default;


	/**********************************************************/
	// Initialize & Terminate
	void	Initialize(void);	// should have a config file
	void	Terminate(void);


	/**********************************************************/
	// Animation Controls:
	void	Update(float elapsedTime);
	void	Render(SGD::Point position, bool flipped,
		SGD::Size scale, SGD::Color color) const;
	void	Restart(bool looping, float speed);
	void	Pause(bool pause)	{ m_bPlaying = !pause; }

	bool	IsPlaying(void) const	{ return m_bPlaying; }
	bool	IsFinished(void) const	{ return m_bFinished; }
	void AddRef();
	void Release();



	SGD::Rectangle GetRect(SGD::Point position, bool flipped,
		float scale) const;
	float GetScale() { return m_fScale; }
	SGD::Point GetPosition() { return position; }



	void SetImage(SGD::HTexture Image) { m_hImage = Image; }
	void SetPosition(SGD::Point newpos) { position = newpos; }
	void SetsetAnim(unsigned int newAnim) { setAnim = newAnim; }
	void SetScale(float _scale) { m_fScale = _scale; }
	void SetLooping(int _looping);
	void SetPlaying(int _Playing);
	void SetFinished(int _Finsihed);
	void SetTimeWaited(float _timetoWait) { m_fTimeWaited = _timetoWait; }

	// frame data
	struct Frame
	{
		SGD::Rectangle		rFrame;		// source rectangle
		SGD::Point			ptAnchor;	// relative position within source
		float				fDuration;	// time to wait on this frame
		std::string				eventname; // event name
	};


	//	void SetFrameRect(SGD::Rectangle sourceRect); 

private:
	/**********************************************************/
	// image
	SGD::HTexture			m_hImage = SGD::INVALID_HANDLE;

	unsigned int m_unRefCount = 1;
	SGD::Point position = { 500, 500 };
	unsigned int setAnim = 1;

	std::vector< Frame >	m_vFrames;

	// animation data
	int						m_nCurrFrame = 0;
	float					m_fScale = 1;
	float					m_fTimeWaited = 0.0f;
	float					m_fSpeed = 1.0f;		// multiplier: 2.0 - twice as fast

	bool					m_bPlaying = false;
	bool					m_bLooping = false;
	bool					m_bFinished = false;


	// load info
	int numFrames;

	friend class AnimationManager;
};
