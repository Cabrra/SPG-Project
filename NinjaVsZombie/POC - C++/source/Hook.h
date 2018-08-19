#pragma once

#include "Bullet.h"

class Hook :
	public Bullet
{
	IEntity* m_Box = nullptr;

	SGD::HTexture m_Rope = SGD::INVALID_HANDLE;
	SGD::HAudio		m_HookSound = SGD::INVALID_HANDLE;
public:
	Hook(SGD::Point position, float rotation, Entity* owner);
	~Hook();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_HOOK; }
	
	virtual void	HandleCollision(const IEntity* pOther)	override;
};