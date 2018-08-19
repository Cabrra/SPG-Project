#pragma once

#include "Enemy.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Bullet.h"
#include <vector>

class ThrowingNinja : public Enemy, public SGD::Listener
{
private:
	bool testspin = false;
	float shooting = 0;
	SGD::Point m_LastKnownPoint;
	std::vector<SGD::Point>		m_Waypoints;
	unsigned int m_currentwaypoint = 0;

	SGD::HAudio					m_hit = SGD::INVALID_HANDLE;
	SGD::HAudio					m_death = SGD::INVALID_HANDLE;
	SGD::HAudio					m_attack = SGD::INVALID_HANDLE;

public:
	ThrowingNinja();
	~ThrowingNinja();
	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;
	//Entity*					GetTarget()		{ return m_Target; }
	virtual int				GetType(void)	const			override	{ return ENT_NINJA; }
	virtual void			HandleCollision(const IEntity* pOther)	override;
	virtual void			HandleEvent(const SGD::Event* pEvent) override;
	void					CreateDrop();
};

