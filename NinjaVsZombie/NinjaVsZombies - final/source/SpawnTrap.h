#pragma once
#include "Trap.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "SpawnPoint.h"

class SpawnTrap :
	public Trap, public SGD::Listener
{
private:
	SpawnPoint*					m_Spawner = nullptr;
	SpawnPoint::m_eObjectType	m_SpawnType = SpawnPoint::NONE;

	float			m_SpawnTimer = 0.0f;
	SGD::Point		m_Position = { 0, 0 };
	unsigned int	m_MaxEnemies = 0;
	unsigned int	m_SpawnedUnits = 0;
	bool			m_Active = false;

public:
	SpawnTrap(SpawnPoint::m_eObjectType	spawnType, SGD::Point pos, float timer, unsigned int max);
	~SpawnTrap();


	virtual int			GetType(void)	const			override	{ return ENT_SPAWNTRAP; }
	virtual void		Update(float elapsedTime)		override;
	virtual void		Render(void)					override;
	virtual void		HandleEvent(const SGD::Event* pEvent) override;
};

