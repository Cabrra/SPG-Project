#include "Challenge.h"

#include "Game.h"
#include "PressurePoint.h"
#include "Unbreakable.h"
#include "DestroyEntityMessage.h"
#include "Trap.h"
#include "Door.h"

#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>
#include "BitmapFont.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

Challenge::Challenge(Entity* locker, unsigned int zombies, unsigned int neutrals, SGD::Point pos, float spawnTime) : Listener(this)
{
	Listener::RegisterForEvent("EnemyDeleted");
	//Challenge Puzzles can't have any other spawn points close.
	this->SetPosition(pos);
	
	std::vector<SGD::Point> myPo;
	SGD::Point aux;
	aux = { pos.x - 200, pos.y + 100 };
	myPo.push_back(pos);
	myPo.push_back(aux);
	aux = { pos.x + 170, pos.y - 150 };
	myPo.push_back(aux);

	m_ZombiePoint = new SpawnPoint(spawnTime, { GetPosition().x + 200, GetPosition().y + 50 }, zombies, true, SpawnPoint::ZOMBIE, false, myPo);
	m_NeutralPoint = new SpawnPoint(spawnTime, { GetPosition().x - 200, GetPosition().y - 50 }, neutrals, true, SpawnPoint::NEUTRAL_ENEMY, false, myPo);
	myPo.clear();
	
	m_ZombieCount = zombies;
	SetLocker(locker);

	SetSize({ 5, 5 });

	dooropensnd = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Dooropen.wav");
	doorclosesnd = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Doorclose.wav");
}


Challenge::~Challenge()
{
	delete m_ZombiePoint;
	delete m_NeutralPoint;
	SetLocker(nullptr);

	SGD::AudioManager::GetInstance()->UnloadAudio(dooropensnd);
	SGD::AudioManager::GetInstance()->UnloadAudio(doorclosesnd);
}

void Challenge::SetLocker(Entity* newLocker)
{
	if (m_locked != nullptr)
		m_locked->Release();

	m_locked = newLocker;

	if (m_locked != nullptr)
		m_locked->AddRef();
}

void Challenge::Update(float elapsedTime)
{

	m_messageTime -= elapsedTime;
	if (m_messageTime <= 0)
		m_message = false;

		m_ZombiePoint->SetActive(true);
		m_NeutralPoint->SetActive(true);
		
		m_ZombiePoint->SetMyState(Enemy::AI_PATROL);
		m_NeutralPoint->SetMyState(Enemy::AI_PATROL);

		m_ZombiePoint->Update(elapsedTime);
		m_NeutralPoint->Update(elapsedTime);

		if (m_CurrentZombie >= m_ZombieCount)
		{
			if (m_locked->GetType() == ENT_TRAP || m_locked->GetType() == ENT_FIRETRAP || m_locked->GetType() == ENT_SPIKETRAP
				|| m_locked->GetType() == ENT_ELECTRICTRAP || m_locked->GetType() == ENT_SPAWNTRAP)
			{
				Trap* mytr = (Trap*)m_locked;
				bool act = mytr->GetActive();
				mytr->SetActive(!act);
			}
			else if (m_locked->GetType() == Entity::ENT_DOOR)
			{
				Door* mydur = (Door*)m_locked;
				bool act = mydur->GetUnlocked();
				mydur->SetUnlocked(!act);

				if (mydur->GetUnlocked() == true)
					SGD::AudioManager::GetInstance()->PlayAudio(dooropensnd, false);
				else
					SGD::AudioManager::GetInstance()->PlayAudio(doorclosesnd, false);
			}
			DestroyEntityMessage* mymess = new DestroyEntityMessage(this);
			mymess->QueueMessage();
			mymess = nullptr;
		}

}

void Challenge::Render()
{
	if (m_message == true)
	{
		const BitmapFont* pFont = Game::GetInstance()->GetFont();

		HINSTANCE HInstance = Game::GetInstance()->GetHInstance();
		char buf[255];
		LoadString(HInstance, IDS_CHALLENGEHINT, buf, 255);
		
		pFont->Draw(buf, { 150, 50 }, 0.7f, { 0, 0, 0 });
		pFont->Draw(buf, { 150, 53 }, 0.7f, { 255, 255, 255 });

	}
}

void Challenge::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "EnemyDeleted")
	{
		if (Game::GetInstance()->GetWorldCamera().IsIntersecting(GetRect()))
		{
			Entity* sender = (Entity*)pEvent->GetSender();
			if (sender->GetType() == ENT_NEUTRAL_ENEMY)
			{
				m_CurrentZombie = 0;
				m_message = true;
				m_messageTime = 5.0f;
				m_NeutralPoint->SetSpawnedUnit(m_NeutralPoint->GetSpawnedUnits() - 1);
			}
			else if (sender->GetType() == ENT_ZOMBIE)
			{
				m_ZombiePoint->SetSpawnedUnit(m_ZombiePoint->GetSpawnedUnits() - 1);
				m_CurrentZombie++;
			}
		}
	}
}
