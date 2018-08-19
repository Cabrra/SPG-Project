#pragma once

#include "Breakable.h"
#include "../SGD Wrappers/SGD_Handle.h"		// HTexture type
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Enemy.h"

class Player;
class FakeFloor :
	public Breakable, public SGD::Listener
{
public:
	FakeFloor();
	~FakeFloor();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_FAKE_FLOOR; }
	virtual void			HandleCollision(const IEntity* pOther)	override;
	void					SetTarget(Player* newTarget);

	virtual void			HandleEvent(const SGD::Event* pEvent) override;

private:

	Enemy::AIState						m_myState = Enemy::AI_PATROL;
	Player*								m_Target = nullptr;
	SGD::HTexture						m_hHide = SGD::INVALID_HANDLE;
	bool								m_ParticleActivated = false;
	float								m_particleTimer = 2.5f;

};

