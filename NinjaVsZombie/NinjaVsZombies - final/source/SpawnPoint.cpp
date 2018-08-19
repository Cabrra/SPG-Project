#include "SpawnPoint.h"
#include "GamePlayState.h"
#include "LevelManager.h"
#include "Player.h"

SpawnPoint::SpawnPoint() : SGD::Listener(this)
{
	Listener::RegisterForEvent("PLAYER_SPOTTED");
	Listener::RegisterForEvent("PLAYER_OUT");
	Listener::RegisterForEvent("PIZZA_FREE");
	Listener::RegisterForEvent("TargetDeleted");
}

SpawnPoint::SpawnPoint(float spawntime, SGD::Point pos, unsigned int max, bool active, m_eObjectType mytype, bool playerstart,
	std::vector<SGD::Point> waypoints)
	: SGD::Listener(this)
{
		Listener::RegisterForEvent("PLAYER_SPOTTED");
		Listener::RegisterForEvent("CAUTION_STATE");
		Listener::RegisterForEvent("PIZZA_FREE");

		m_SpawnTimer = spawntime;
		m_Position = pos;
		m_MaxEnemies = max;
		m_Active = active;
		playerStart = playerstart;
		m_Type = mytype;
		for (unsigned int i = 0; i < waypoints.size(); i++)
			m_vWaypoints.push_back(waypoints[i]);
}

SpawnPoint::~SpawnPoint()
{
	m_vWaypoints.clear();
}

void SpawnPoint::AddWaypoint(SGD::Point pos)
{
	m_vWaypoints.push_back(pos);
}

void SpawnPoint::Update(float elapsedTime)
{
	if (m_RestartTimer > 0)
	{
		m_RestartTimer -= elapsedTime;
		if (m_RestartTimer < 0)
		{
			m_SpawnedUnits = 0;
			m_RestartTimer = 20.0f;
		}
	}

	if (this->GetType() == PRIDE && playerStart == true)
	{
		GamePlayState::GetInstance()->CreatePride(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == LUST && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateLustWrath(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == KINGPIN && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateGluttony(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == END_POINT && playerStart == true)
	{
		SGD::Point endpoint = this->GetPosition();
		GamePlayState::GetInstance()->GetLevelManager()->SetEndPoint(endpoint);
		playerStart = false;
		return;
	}
	else if (this->GetType() == HDOOR && playerStart == true)
	{
		Entity* ent = GamePlayState::GetInstance()->CreateHDoor(this);
		ent->Release();
		playerStart = false;
		return;
	}
	else if (this->GetType() == VDOOR && playerStart == true)
	{
		Entity* ent = GamePlayState::GetInstance()->CreateVDoor(this);
		ent->Release();
		playerStart = false;
		return;
	}
	else if (this->GetType() == PIZZA && playerStart == true)
	{
		Player* tmp = GamePlayState::GetInstance()->GetPlayer();

		if (tmp->GetHavePizza() == false)
		{
			GamePlayState::GetInstance()->CreatePizza(nullptr, this->GetPosition());
		}

		playerStart = false;
		return;
	}
	else if (this->GetType() == HOOK && playerStart == true)
	{
		GamePlayState::GetInstance()->SpawnHook(nullptr, this->GetPosition());
		playerStart = false;
		return;
	}
	else if (this->GetType() == SWORD && playerStart == true)
	{
		GamePlayState::GetInstance()->SpawnSword(nullptr, this->GetPosition());
		playerStart = false;
		return;
	}
	else if (this->GetType() == SMOKE_BOMB && playerStart == true)
	{
		GamePlayState::GetInstance()->SpawnSmokeBomb(this->GetPosition());
		playerStart = false;
		return;
	}
	else if (this->GetType() == ZOMBIE && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateZombie(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == A_ZOMBIE && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateArmoredZombie(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == RANGED_NINJA && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateNinjaThrower(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == MOVABLE_BOX && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateMovableBox(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == IMMOVABLE_BOX && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateImmovableBox(this);
		playerStart = false;
		return;
	}
	else if (this->GetType() == NEUTRAL_ENEMY && playerStart == true)
	{
		GamePlayState::GetInstance()->CreateNeutralEnemy(this);
		playerStart = false;
		return;
	}

	/*if (this->GetType() == LEVER && playerStart == true)
	{
	SGD::Point lvr = this->GetPosition();
	GamePlayState::GetInstance()->CreateLever(lvr,nullptr);
	playerStart = false;
	return;
	}*/

	//if (this->GetType() == PRESSUREPOINT && playerStart == true)
	//{
	//	/*SGD::Point pp = this->GetPosition();
	//	GamePlayState::GetInstance()->CreatePressurePoint(pp, nullptr);
	//	playerStart = false;*/
	//	return;
	//}

	if (m_Active == true && m_SpawnedUnits < m_MaxEnemies)
	{
		m_CurrentTime += elapsedTime;
		if (m_CurrentTime >= m_SpawnTimer)
		{
			//spawn stuff
			switch (m_Type)
			{
			case ZOMBIE:
			{
				GamePlayState::GetInstance()->CreateZombie(this);
			}
				break;
			case A_ZOMBIE:
			{
				GamePlayState::GetInstance()->CreateArmoredZombie(this);
			}
			case RANGED_NINJA:
			{
				// GamePlayState::GetInstance()->CreateNinjaThrower(this);
			}
				break;
			case AMBUSH_NINJA:
			{
				 GamePlayState::GetInstance()->CreateAmbushNinja(this);
				 m_Active = false;
			}
				break;
			case NEUTRAL_ENEMY:
			{
				GamePlayState::GetInstance()->CreateNeutralEnemy(this);
			}
				break;


			default:
				break;
			}
			m_SpawnedUnits++;

			m_CurrentTime = 0.0f;
		}
	}
}

void SpawnPoint::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "PLAYER_SPOTTED")
	{
		if (m_Type != AMBUSH_NINJA)
		{
			m_Active = true;
			mystate = Enemy::AI_ALERT;
			m_RestartTimer = 20.0f;
		}
	}
	else if (pEvent->GetEventID() == "PLAYER_OUT")
	{
		m_Active = false;
		mystate = Enemy::AI_PATROL;
	}
	else if (pEvent->GetEventID() == "PIZZA_FREE")
	{
		SGD::Rectangle aux = SGD::Rectangle(GetPosition(), SGD::Size{ 5, 5 });
		if (m_Type == AMBUSH_NINJA && Game::GetInstance()->GetWorldCamera().IsIntersecting(aux))
		{
			m_Active = true;
		}	
	}
}