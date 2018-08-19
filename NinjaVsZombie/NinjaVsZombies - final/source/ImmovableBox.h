#pragma once

#include "Unbreakable.h"
class Hook;
class ImmovableBox:
	public Unbreakable
{
	Hook* m_hook = nullptr;
public:
	ImmovableBox();
	~ImmovableBox();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_IMMOVABLE; }
	virtual void	HandleCollision(const IEntity* pOther)	override;

	void		SetHook(Hook* hook)	{ m_hook = hook; }
};