#pragma once

#include "Enemy.h"

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"

#include <vector>

class AmbushNinja :
	public Enemy, public SGD::Listener
{
private:
	AIState						m_myState = AI_ATTACK;
	SGD::Point					m_currPoint;
	float						m_StateTimer = 4.0f;


	SGD::HAudio					m_hit = SGD::INVALID_HANDLE;
	SGD::HAudio					m_death = SGD::INVALID_HANDLE;

public:
	AmbushNinja();
	~AmbushNinja();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	void					UpdateEscape(float elapsedTime);
	void					UpdateAttack(float elapsedTime);

	SGD::Point				GetCurrentPoint()const		{ return m_currPoint; }
	virtual int				GetType(void)	const			override	{ return ENT_AMBUSHNINJA; }

	void					SetCurrentPoint(SGD::Point newWay)		{ m_currPoint = newWay; }

	virtual void			HandleCollision(const IEntity* pOther)	override;
	virtual void			HandleEvent(const SGD::Event* pEvent) override;

	void					PlayStealSound();
};

