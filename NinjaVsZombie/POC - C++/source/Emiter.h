
#pragma once
#include "../SGD Wrappers/SGD_Geometry.h" 
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & 
#include "../SGD Wrappers/SGD_String.h"

#include <list>
#include "Particle.h"
#include "ParticleFlyweight.h"

using namespace std;

enum ShapeType{
	SHAPE_POINT, SHAPE_LINE,
	SHAPE_RECTANGLE, SHAPE_CIRCLE,
	COUNTER // counter to know how many items are in the enumeration
};

class Emiter
{
private:
	SGD::String					m_filePath;
	SGD::Size					m_ImageSize; // Image size

	SGD::Point					m_Position;
	float						m_width;		// particle spawn spread
	float						m_height;
	SGD::Size					m_Size;			// emiter's size
	ParticleFlyweight*			m_ParticleFlyweight;
	unsigned int				m_MaxParticles;
	unsigned int				m_FirstWave;
	float						m_SpawnRate; 

	ShapeType					m_shape;		//shape type

	std::list<Particle*>		m_LiveList;
	std::list<Particle*>		m_DeadList;

	float						elapsedSpawn		= 0.0f;
	SGD::HTexture				m_hParticle			= SGD::INVALID_HANDLE;

	/**********************************************************/
	// reference count
	unsigned int				m_unRefCount = 0;	// calling 'new' gives the prime reference

public:
	Emiter(SGD::String imagePath, SGD::Point posi = {}, float widt = 0.0f, float heig = 0.0f, SGD::Size siz = {}, unsigned int maxPart = 0,
		unsigned int firstWav = 0, float spawn = 0.0f, SGD::Size Imagsiz = { 0, 0 },  unsigned int shape = 0, ParticleFlyweight* fly = nullptr);
	~Emiter();

	void			Update(float dt);
	void			Render();
	void			ChangeRatioCalculator(Particle* parti);
	void			InitializeEmiter();
	void			SpawnParticle();
	bool			IsOnLine(SGD::Point startPoint, SGD::Point endPoint, SGD::Point checkPoint);
	float			LerpFunction(float start, float end, float ratio);


	void	AddRef(void);
	void	Release(void);


	//********************* | SETTERS | *********************************************

	void			SetPosition			(SGD::Point newPos);
	void			SetWidth			(float newWidth);
	void			SetHeight			(float newHeight);
	void			SetSize				(SGD::Size newSiz);
	void			SetMaxParticles		(unsigned int newMax);
	void			SetFirstWave		(unsigned int newFirst);
	void			SetSpawnRate		(float newSpawnRate);

	void			SetFlyWeight(ParticleFlyweight* fweight);

	//********************* | GETTERS | *********************************************

	SGD::Point		GetPosition			()const;
	float			GetWidth			()const;
	float			GetHeight			()const;
	SGD::Size		GetSize				()const;
	unsigned int	GetMaxParticles		()const;
	unsigned int	GetFirstWave		()const;
	float			GetSpawnRate		()const;

};