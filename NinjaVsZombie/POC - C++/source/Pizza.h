#pragma once
#include "PickUp.h"
class Pizza :
	public PickUp
{
public:
	Pizza();
	~Pizza();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_PIZZA; }

	virtual void	HandleCollision(const IEntity* pOther)	override;
};