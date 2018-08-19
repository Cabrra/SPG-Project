// written by william dimeo
// SGP 1408

#pragma once
// headers
#include <vector>
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "AnchorPointAnimation.h"

#include <vector>		// uses std::vector
#include <map>
using namespace std;
// forward declars
class TimeStamp;
class AnimationManager
{
	
	map<string, AnchorPointAnimation> LoadedAninmation;

	/**********************************************************/
	// SINGLETON!
	AnimationManager(void);
	virtual ~AnimationManager(void);

	AnimationManager(const AnimationManager&) = delete;
	AnimationManager& operator= (const AnimationManager&) = delete;

public:

	static AnimationManager* GetInstance(void);

	void Render(int x, int y, TimeStamp, bool flipped, SGD::Size scale = { 1, 1 });
	void Update(float dt, TimeStamp&);

	bool LoadXML(const char* file);
	void SaveXML();
	void Terminate(void);

	

};

//holds all the aninmations in vector<vector<anchor point>> 
// updates all the animations
// a funchion to find out what animation it should play
// a vearable to store that info
// call anchor's render
// take in all the tinyxml info and store it