#pragma once
#include "PickUp.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"

class Pizza :
	public PickUp, public SGD::Listener
{
	bool		m_IsAlive = true;
public:
	Pizza();
	~Pizza();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_PIZZA; }

	virtual void	HandleCollision(const IEntity* pOther)	override;
	virtual void HandleEvent(const SGD::Event* pEvent) override;

	bool CheckCollision(SGD::Point point);
};