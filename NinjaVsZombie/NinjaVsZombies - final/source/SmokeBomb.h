#pragma once


#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "PickUp.h"
class SmokeBomb :
	public PickUp, public SGD::Listener
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
	virtual void	HandleEvent(const SGD::Event* pEvent) override;
	virtual SGD::Rectangle GetRect(void)	const			override;

	Entity*			GetOwner()						{ return m_owner; }
	SGD::HAudio		GetSound()						{ return m_Sound; }

	void			SetOwner(Entity* owner)			{ m_owner = owner; }
};