#pragma once

#include "Unbreakable.h"
class ImmovableBox:
	public Unbreakable
{
public:
	ImmovableBox();
	~ImmovableBox();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_IMMOVABLE; }
	virtual void	HandleCollision(const IEntity* pOther)	override;
};