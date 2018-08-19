#pragma once
#include "Enemy.h"

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"

#include <vector>

class NeutralEnemy : public Enemy, public SGD::Listener
{
private:
	AIState						m_myState = AI_PATROL;
	std::vector<SGD::Point>		m_Waypoints;
	SGD::Point					m_EscapePoint = { 0, 0 };
	unsigned int				m_currPoint;
	float						m_StateTimer = 2.5f;

	SGD::Point					m_currentPoint;
public:
	NeutralEnemy();
	~NeutralEnemy();

	void					CreateDrop();
	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	void					UpdateIdle(float elapsedTime);
	void					UpdateAttack(float elapsedTime);
	void					UpdateCaution(float elapsedTime); //run when health is low
	void					UpdatePatrol(float elapsedTime);

	void					SetStateTime(float newTime)		{ m_StateTimer = newTime; }
	void					SetState(AIState newState)		{ m_myState = newState; }

	virtual int				GetType(void)	const			override	{ return ENT_NEUTRAL_ENEMY; }
	virtual void			HandleCollision(const IEntity* pOther)	override;

	void					AddWayPoint(SGD::Point newPoint){ m_Waypoints.push_back(newPoint); }
	virtual void			HandleEvent(const SGD::Event* pEvent) override;
};

