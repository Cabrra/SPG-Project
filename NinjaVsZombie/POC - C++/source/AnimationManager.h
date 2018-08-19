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
	AnchorPointAnimation testanimation;
	AnchorPointAnimation testanimation2;


#if 0
	int animations;
	bool flipped = false;
	float scale = 1;
	SGD::Color color = SGD::Color{ 255, 255, 255 };

	/**********************************************************/
	// Typedefs will simplify the templates
	typedef std::vector< AnchorPointAnimation* >		EntityVector;
	typedef std::vector< EntityVector >	EntityTable;

	EntityTable	m_tEntities;			// vector-of-vector-of-IEntity* (2D table)

	/**********************************************************/
	// image
	SGD::HTexture			m_hImage = SGD::INVALID_HANDLE;

	bool		m_bIterating = false;	// read/write lock  
	vector<AnchorPointAnimation> vecAnimations;
#endif // 0
	/**********************************************************/
	// SINGLETON!
	AnimationManager(void);
	virtual ~AnimationManager(void);

	AnimationManager(const AnimationManager&) = delete;
	AnimationManager& operator= (const AnimationManager&) = delete;

public:
	
#if 0
	/**********************************************************/
	// Animation Storage:
	void	AddEntity(AnchorPointAnimation* pEntity, unsigned int bucket);
	void	RemoveEntity(AnchorPointAnimation* pEntity, unsigned int bucket);
	void	RemoveEntity(AnchorPointAnimation* pEntity);
	void	RemoveAll(unsigned int bucket);
	void	RemoveAll(void);
	void	CheckCollisions(unsigned int bucket1, unsigned int bucket2);


	/**********************************************************/
	// Animation Upkeep:
	void	UpdateAll(float elapsedTime);
	void	RenderAll(void);

#endif // 0
	static AnimationManager* GetInstance(void);


	void Render(int x, int y, TimeStamp);
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