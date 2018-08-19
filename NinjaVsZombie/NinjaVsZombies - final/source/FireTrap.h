#pragma once
#include "Trap.h"
#include "Emiter.h"
class FireTrap :
	public Trap
{
	int m_nCurrTick = 0;
	//Emiter* m_pParticle;
	float damageTimer = 0.0f;

public:
	FireTrap();
	~FireTrap();

	int GetCurrTick()const { return m_nCurrTick; }
	//Emiter* GetParticle() { return m_pParticle; }

	void SetCurrTick(int tick) { m_nCurrTick = tick; }
	//Emiter* GetParticle() { return m_pParticle; }
	virtual void SetActive(bool active) override;

	virtual int		GetType(void)	const			override	{ return ENT_FIRETRAP; }

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;
	virtual void	HandleCollision(const IEntity* pOther)	override;

};

