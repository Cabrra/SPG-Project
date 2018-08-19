#pragma once
#include "PickUp.h"
class SmokeBomb :
	public PickUp
{
	Entity*			m_owner = nullptr;

	float			m_lifeSpan = 5;

	SGD::HAudio		m_Sound = SGD::INVALID_HANDLE;

public:
	SmokeBomb(Entity* owner);
	~SmokeBomb();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_SMOKEBOMB; }
	virtual void	HandleCollision(const IEntity* pOther)	override;

	Entity*			GetOwner()						{ return m_owner; }

	void			SetOwner(Entity* owner)			{ m_owner = owner; }
};