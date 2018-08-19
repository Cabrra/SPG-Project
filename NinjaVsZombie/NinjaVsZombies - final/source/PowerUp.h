#pragma once
#include "PickUp.h"
#include "../SGD Wrappers/SGD_Handle.h"

class PowerUp :
	public PickUp
{
private:
	int			m_type;
	float		m_timer;
	int			m_level;

	float		m_collisionTimer = 1.2f;
	
public:
	enum PowerUpType {
		PU_STRENGTH,
		PU_SPEED, PU_TIME,
		PU_MONEY, PU_HEALTH
	};

	PowerUp();
	PowerUp(int mytype, int level, SGD::Point myPos);
	~PowerUp();

	void SetPowerUpType(int newtype)   { m_type = newtype; }
	void SetPowerUpLevel(int newLevel)	{ m_level = newLevel; }

	int GetPowerUpType() const			{ return m_type; }
	int GetPowerUpLevel()const			{ return m_level; }

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_POWERUP; }
	virtual SGD::Rectangle	GetRect(void)	const			override;
	virtual void			HandleCollision(const IEntity* pOther)	override;
};

