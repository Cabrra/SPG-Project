#pragma once
#include "Bullet.h"

class Entity;

class HeartBullet :
	public Bullet
{
private:
	Entity* m_Target;
	float m_Heartlife = 5.0f;

public:
	HeartBullet(SGD::Point position, float rotation, Entity* owner);
	virtual ~HeartBullet();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_HEART; }
	virtual void	HandleCollision(const IEntity* pOther)	override;

	Entity* GetTarget() { return m_Target; }
	void SetTarget(Entity* targ) { m_Target = targ; }
};

