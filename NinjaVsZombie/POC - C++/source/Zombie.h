#pragma once
#include "Enemy.h"

class Zombie :
	public Enemy
{
	Entity*		m_attacker = nullptr;

public:
	Zombie();
	~Zombie();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_ZOMBIE; }
	virtual void			HandleCollision(const IEntity* pOther)	override;
};