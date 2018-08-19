#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "GamePlayState.h"
#include "Entity.h"

class Trap : public Entity
{
public:
	enum TrapType {
		TRAP_FIRE = 5,
		TRAP_LIGHTING,
		TRAP_SPIKE,
		TRAP_FAKE,
	};

private:
	int m_nDamage;
	bool m_bActive;
	TrapType m_eTraptype;
	bool m_bTickNow = false;

public:
	Trap();
	virtual ~Trap();

	int GetDamage() { return m_nDamage; }
	bool GetActive() { return m_bActive; }
	TrapType GetTrapType() { return m_eTraptype; }
	bool GetTickNow() { return m_bTickNow; }

	void SetDamage(int dmg) { m_nDamage = dmg; }
	virtual void SetActive(bool active) { m_bActive = active; }
	void SetTrapType(TrapType type) { m_eTraptype = type; }
	void SetTickNow(bool now) { m_bTickNow = now; }

	virtual int		GetType(void)	const			override	{ return ENT_TRAP; }

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;
	virtual SGD::Rectangle GetRect(void)	const			override;
	virtual void	HandleCollision(const IEntity* pOther)	override;
};

