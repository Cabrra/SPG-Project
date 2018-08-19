#include "SpawnTrap.h"
#include "DestroyEntityMessage.h"
using namespace std;

SpawnTrap::SpawnTrap(SpawnPoint::m_eObjectType	spawnType, SGD::Point pos, float timer, unsigned int max) : Listener(this)
{
	Listener::RegisterForEvent("Target");

	m_SpawnType = spawnType;
	SetPosition(pos);
	m_SpawnTimer = timer;
	m_MaxEnemies = max;
	m_Active	= false;
	std::vector<SGD::Point> aux;
	aux.push_back(GetPosition());
	m_Spawner = new SpawnPoint(m_SpawnTimer, GetPosition(), m_MaxEnemies, true, m_SpawnType, false, aux);
}


SpawnTrap::~SpawnTrap()
{
	if (m_Spawner != nullptr)
		delete m_Spawner;
	m_Spawner = nullptr;
}

void SpawnTrap::Update(float elapsedTime)
{
	if (m_Active == true)
	{
		m_Spawner->Update(elapsedTime);
	}
	if (m_Spawner->GetSpawnedUnits() == m_Spawner->GetMaxEnemies())
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;

		m_Active = false;
	}
}

void SpawnTrap::Render(void)
{

}
void SpawnTrap::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "Target")
	{
		Entity* newTarget = (Entity*)pEvent->GetSender();
		SGD::Vector distance = newTarget->GetPosition() - GetPosition();
		if (distance.ComputeLength() < 350)
		{
			m_Active = true;
		}
	}
}