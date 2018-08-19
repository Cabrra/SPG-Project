#pragma once

#include <vector>
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Unbreakable.h"
#include "SpawnPoint.h"
#include <vector>
using namespace std;

class Zombie;
class NeutralEnemy;

class Challenge: 
	public Unbreakable, public SGD::Listener
{

private:
	SpawnPoint*							m_ZombiePoint = nullptr;
	SpawnPoint*							m_NeutralPoint = nullptr;

	unsigned int						m_ZombieCount = 0;
	unsigned int						m_NeutralCount = 0;

	unsigned int						m_CurrentZombie = 0;

	bool								m_message = false;
	float								m_messageTime = 5.0f;
	
	Entity*								m_locked = nullptr; //door

	SGD::HAudio dooropensnd = SGD::INVALID_HANDLE;
	SGD::HAudio doorclosesnd = SGD::INVALID_HANDLE;

public:
	Challenge(Entity* locker = nullptr, unsigned int zombies = 0, unsigned int neutrals = 0, SGD::Point pos = {}, float spawnTime = 0.0f);
	~Challenge();

	virtual void			Update(float elapsedTime)		override;
	virtual int				GetType(void)	const			override	{ return ENT_CHALLENGE; }
	virtual void			Render()						override;
	virtual void			HandleEvent(const SGD::Event* pEvent) override;

	void					SetLocker(Entity* newLocker);
};

