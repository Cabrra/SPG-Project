#pragma once
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Entity.h"
class BossKey : public Entity
{

	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;
	SGD::HAudio  m_hSound = SGD::INVALID_HANDLE;

public:
	BossKey();
	~BossKey();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;
	virtual void			HandleCollision(const IEntity* pOther)	override;


};
