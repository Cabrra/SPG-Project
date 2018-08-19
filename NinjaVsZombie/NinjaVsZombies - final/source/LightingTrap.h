#pragma once
#include "Trap.h"
#include "Emiter.h"
class LightingTrap :
	public Trap
{
	int m_nCurrTick = 0;
	Emiter* m_pParticle;
	float damageTimer = 0.0f;
	bool m_hithim = false;

public:
	LightingTrap();
	~LightingTrap();

	int GetCurrTick()const { return m_nCurrTick; }
	Emiter* GetParticle()const { return m_pParticle; }

	void SetCurrTick(int tick) { m_nCurrTick = tick; }
	//Emiter* GetParticle() { return m_pParticle; }

	virtual int		GetType(void)	const			override	{ return ENT_ELECTRICTRAP; }
	virtual void	SetActive(bool active) override;

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;
	virtual void	HandleCollision(const IEntity* pOther)	override;
};

