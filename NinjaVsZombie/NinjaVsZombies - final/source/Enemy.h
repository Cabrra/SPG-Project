#pragma once

#include "Unit.h"

#include "../SGD Wrappers/SGD_Handle.h"
#include <vector>


class Enemy : public Unit
{
private:
	Entity*						m_Target			= nullptr;

	std::vector<SGD::Point>		m_Path;
	
public:
	

	enum AIState{
		AI_IDLE, AI_ATTACK,
		AI_ALERT, AI_CAUTION,
		AI_PATROL, AI_ESCAPE
	};

	Enemy();
	~Enemy();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_ENEMY; }
	virtual void			HandleCollision(const IEntity* pOther)	override;

	Entity*					GetTarget()const						{ return m_Target; }

	void					SetTarget(Entity* newtarget);

	std::vector<SGD::Point> GetPath()	{ return m_Path; }

	void					AddPointToPath(SGD::Point point)	{ m_Path.push_back(point); }
};