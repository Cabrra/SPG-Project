#pragma once
#include "Item.h"
class Bullet :
	public Item
{
public:
	Bullet(SGD::Point position, float rotation, Entity* owner);
	~Bullet();

	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_BULLET; }
	virtual void	HandleCollision(const IEntity* pOther)	override;

	Entity* GetOwner()const { return m_eOwner; }

	void SetOwner(Entity* newOwner){ m_eOwner = newOwner; }

	float		GetSpeed(void) { return m_speed; }

	void				SetSpeed(float newSpeed){ m_speed = newSpeed; }

private:
	Entity* m_eOwner;
	float m_speed = 200;
};