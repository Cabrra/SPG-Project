#pragma once

#include "Bullet.h"

class Hook :
	public Bullet
{
private:
	IEntity* m_Box = nullptr;
	float m_lifeTimer = 2.0f;
	Entity*					m_Lever = nullptr;

	SGD::HTexture m_Rope = SGD::INVALID_HANDLE;
public:
	Hook(SGD::Point position, float rotation, Entity* owner);
	~Hook();

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_HOOK; }
	
	virtual void	HandleCollision(const IEntity* pOther)	override;

	IEntity*			HookedItem()const { return m_Box; }

	bool			CheckCollision(SGD::Point point);
};