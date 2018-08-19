#pragma once
#include "Trap.h"
#include "Emiter.h"
#include "Tile.h"
#include "../SGD Wrappers/SGD_Handle.h"

class FakeTrap :
	public Trap
{
	int m_nCurrTick = 0;
	Emiter* m_pParticle;
	float damageTimer = 0.0f;
	Tile* m_pTile;

public:
	FakeTrap();
	~FakeTrap();

	int GetCurrTick()const { return m_nCurrTick; }
	Emiter* GetParticle()const { return m_pParticle; }
	Tile* GetTile()const { return m_pTile; }

	void SetCurrTick(int tick) { m_nCurrTick = tick; }
	//Emiter* GetParticle() { return m_pParticle; }
	void SetTile(Tile* tile) { m_pTile = tile; }

	virtual int		GetType(void)	const			override	{ return ENT_FAKETRAP; }

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;
	virtual void	HandleCollision(const IEntity* pOther)	override;
};

