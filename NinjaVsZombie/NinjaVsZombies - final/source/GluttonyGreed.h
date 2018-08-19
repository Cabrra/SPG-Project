#pragma once

#include "Enemy.h"

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "TimeStamp.h"
#include <vector>
class Player;


class GluttonyGreed : public Enemy, SGD::Listener
{


private:
	AIState						m_myState			= AI_PATROL;
	float						m_StateTimer		= 0.0f;
	std::vector<SGD::Point>		m_Waypoints;
	unsigned int				m_currPoint			= 0;
	SGD::Point					m_myCurrentway;
	int							m_stolenDAQ			= 0;
	float						m_IdleTimer			= 3.0f;
	int							m_moneyCount		= 0;

	float						m_RushTimer			= 3.0f;
	bool						m_setPath			= true;
	
	Entity*						m_Pizza				= nullptr;
	int							pizzaCount			= 0;
	float						m_HitTimer			= 3.0f;
	int							speed;

	//TimeStamp m_Attacking;
	TimeStamp m_Tired;
	TimeStamp m_Run;
	bool left = true;
	bool attack = false;
	bool run = false;
	bool tired = true;

public:
	GluttonyGreed();
	~GluttonyGreed();

	virtual void						HandleCollision(const IEntity* pOther)	override;
	virtual void						Update(float elapsedTime)		override;
	virtual void						Render(void)					override;

	void								UpdatePatrol(float elapsedTime);
	void								UpdateAttack(float elapsedTime);
	//void								UpdateAlert(float elapsedTime);
	void								UpdateIdle(float elapsedTime);

	void								SetPizza(Entity* newPizza);
	void								SetMyState(AIState newState)  { m_myState = newState; }
	void								SetCurrentPoint(unsigned int newpoint) { m_currPoint = newpoint; }
	virtual int							GetType(void)	const			override	{ return ENT_BOSS; }
	AIState								GetMyState()const						{ return m_myState; }



	void SetRunAnin(TimeStamp run) { m_Run = run; }
	//void SetAttackAnin(TimeStamp att) { m_Attacking = att; }
	void SetTiredAnin(TimeStamp tired) { m_Tired = tired; }



	void								SetPath(bool isPath)				{ m_setPath = isPath; }
	//virtual SGD::Rectangle				GetRect(void)	const			override;

	void								AddWayPoint(SGD::Point newPoint){ m_Waypoints.push_back(newPoint); }
	virtual void						HandleEvent(const SGD::Event* pEvent) override;
};