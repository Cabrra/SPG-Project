
#pragma once
#include "../SGD Wrappers/SGD_Geometry.h" 
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & 
#include "../SGD Wrappers/SGD_String.h"

#include <list>
#include "Particle.h"
#include "ParticleFlyweight.h"

using namespace std;

class Entity;

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
	bool						m_active;

	ShapeType					m_shape;		//shape type

	std::list<Particle*>		m_LiveList;
	std::list<Particle*>		m_DeadList;

	float						elapsedSpawn		= 0.0f;
	SGD::HTexture				m_hParticle			= SGD::INVALID_HANDLE;

	Entity*						m_owner = nullptr;

	/**********************************************************/
	// reference count
	unsigned int				m_unRefCount = 1;	// calling 'new' gives the prime reference

public:
	Emiter();
	Emiter(SGD::String imagePath, SGD::Point posi = {}, float widt = 0.0f, float heig = 0.0f, SGD::Size siz = {}, unsigned int maxPart = 0,
		unsigned int firstWav = 0, float spawn = 0.0f, SGD::Size Imagsiz = { 0, 0 },  unsigned int shape = 0, ParticleFlyweight* fly = nullptr, Entity* owner = nullptr, bool active = false);
	~Emiter();
	//trilogy of evil
	Emiter& operator=(const Emiter& _assign);
	Emiter(const Emiter& _cpy);

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

	void			SetPosition			(SGD::Point newPos) { m_Position = newPos; }
	void			SetWidth			(float newWidth) { m_width = newWidth; }
	void			SetHeight			(float newHeight) { m_height = newHeight; }
	void			SetSize				(SGD::Size newSiz) { m_Size = newSiz; }
	void			SetMaxParticles		(unsigned int newMax) { m_MaxParticles = newMax; }
	void			SetFirstWave		(unsigned int newFirst){ m_FirstWave = newFirst; }
	void			SetSpawnRate		(float newSpawnRate) { m_SpawnRate = newSpawnRate; }
	void			SetActive			(bool isActive)	{ m_active = isActive; }
	void			SetShape			(ShapeType newShape) { m_shape = newShape; }
	void			SetImageSize		(SGD::Size newSiz) { m_ImageSize = newSiz; }
	void			SetFilePath			(SGD::String path);
	void			SetOwner			(Entity* newOwner);
	void			SetFlyWeight		(ParticleFlyweight* fweight);

	//********************* | GETTERS | *********************************************

	SGD::Point				GetPosition			()const { return m_Position; }
	float					GetWidth			()const{ return m_width; }
	float					GetHeight			()const{ return m_height; }
	SGD::Size				GetSize				()const{ return m_Size; }
	unsigned int			GetMaxParticles		()const{ return m_MaxParticles; }
	unsigned int			GetFirstWave		()const{ return m_FirstWave; }
	float					GetSpawnRate		()const{ return m_SpawnRate; }
	Entity*					GetOwner			()const { return m_owner; }
	bool					GetActive			()const { return m_active; }
	SGD::Size				GetImageSize		()const { return m_ImageSize; }
	ParticleFlyweight*		GetFlyWeight		()const	{ return m_ParticleFlyweight; }
	ShapeType				GetShape			()const { return m_shape; }
	SGD::String				GetFilePath			()const { return m_filePath; }

};