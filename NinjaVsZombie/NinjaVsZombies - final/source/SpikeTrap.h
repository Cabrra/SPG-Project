#pragma once
#include "Trap.h"
#include "Animation.h"
#include "../SGD Wrappers/SGD_Handle.h"

class SpikeTrap :
	public Trap
{
	float m_nCurrTick = 0;
	Animation* m_pAnimation;
	float damageTimer = 0.0f;
	bool isUp = false;
	int hitCount = 0;

	SGD::HTexture m_imgup = SGD::INVALID_HANDLE;
	SGD::HTexture m_imgdown = SGD::INVALID_HANDLE;

public:
	SpikeTrap();
	~SpikeTrap();

	float GetCurrTick()const { return m_nCurrTick; }
	Animation* GetAnimation()const { return m_pAnimation; }

	void SetCurrTick(float tick) { m_nCurrTick = tick; }
	//Animation* GetAnimation() { return m_pAnimation; }

	virtual int		GetType(void)	const			override	{ return ENT_SPIKETRAP; }

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;
	virtual void	HandleCollision(const IEntity* pOther)	override;
};

