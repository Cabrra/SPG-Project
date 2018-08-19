#include "NeutralEnemy.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "DestroyEntityMessage.h"
#include "CreatePowerUpMessage.h"
#include "GamePlayState.h"

#include "Game.h"

#include "Sword.h"
#include <assert.h>
#include "Player.h"
#include "Tile.h"

NeutralEnemy::NeutralEnemy() : Listener(this)
{
	Listener::RegisterForEvent("GAME_OVER");
	Listener::RegisterForEvent("EnemyDeleted");
	Listener::RegisterForEvent("NeutralE_HIT");

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/neutralEnemy.png");

	SetSize({ 64, 64 });
	SetScale({ 1.0f, 1.0f });

	m_myState = AI_PATROL;
}


NeutralEnemy::~NeutralEnemy()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	m_Waypoints.clear();

	SetTarget(nullptr);
}

/*virtual*/ void NeutralEnemy::Render(void)					/*override;*/
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 125, 125, 255 }, {}, 0);
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), GetRotation(), GetSize()/2, {}, GetScale());
}

/*virtual*/ void NeutralEnemy::Update(float elapsedTime)		/*override;*/
{
	if (GetMoveSpeed() == 0)
		SetMoveSpeed(75);

	if (!IsFalling())
	{
		switch (m_myState)
		{
		case(AI_IDLE) :
		{
						  UpdateIdle(elapsedTime);
						  break;
		}
		case(AI_ATTACK) :
		{
							UpdateAttack(elapsedTime);
							break;
		}
		case(AI_CAUTION) :
		{
							 UpdateCaution(elapsedTime);
							 break;
		}
		case(AI_PATROL) :
		{
							UpdatePatrol(elapsedTime);
							break;
		}
		}
		SGD::Event* Target = new SGD::Event{ "Target", nullptr, this };
		Target->QueueEvent();
		Target = nullptr;
	}
	else /*if (IsFalling())*/
	{
		SetScale({ GetScale().width - elapsedTime, GetScale().height - elapsedTime });
		if (GetScale().width <= 0.0f && GetScale().height <= 0.0f)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}

void NeutralEnemy::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "EnemyDeleted")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender != nullptr && sender != this && sender->GetType() == ENT_SMOKEBOMB)
		{
			Entity* tar = (Entity*)pEvent->GetData();
			if (tar == GetTarget())
			{
				SetTarget(nullptr);
				m_myState = AI_IDLE;
				m_StateTimer = 3.0f;
			}
		}
		else if (sender == GetTarget())
		{
			SetTarget(nullptr);
			m_myState = AI_PATROL;
		}
	}
	else if (pEvent->GetEventID() == "NeutralE_HIT")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		Entity* data = (Entity*)pEvent->GetData();
		if (sender != this && data != nullptr && data == this)
		{
			SetCurrHP(GetCurrHP() - 20);
			if (GetCurrHP() <= 0)
			{

				CreateDrop();

			}
			else
			{
				SetTarget(data);
				m_myState = AI_ATTACK;
			}
		}
	}
}

void NeutralEnemy::UpdateIdle(float elapsedTime)
{
	m_StateTimer -= elapsedTime;
	if (m_StateTimer <= 0)
	{
		m_myState = AI_PATROL;
		m_StateTimer = 2.5f;
	}
	float angle = SGD::PI / 2 * elapsedTime;
	SetRotation(GetRotation() + angle);
}

void NeutralEnemy::UpdateAttack(float elapsedTime)
{
	m_StateTimer -= elapsedTime;

	SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();
	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	if (targetLoc.ComputeLength() <50)
	{
		SetMoveSpeed(0);

		if (m_StateTimer <= 0)
		{
			if (GetTarget()->GetType() == ENT_PLAYER)
			{
				m_StateTimer = 1.5;

				SGD::Event* take = new SGD::Event{ "Player_HIT", nullptr, this };
				take->QueueEvent(nullptr);
				take = nullptr;
			}
			else
			{
				m_StateTimer = 1.5;
				SGD::Event* take = new SGD::Event{ "NeutralE_HIT", GetTarget(), this };
				take->QueueEvent(nullptr);
				take = nullptr;
			}
		}
	}
	float angle = orientation.ComputeAngle(targetLoc);

	float maxangle = SGD::PI / 2 * elapsedTime;

	if (angle > maxangle)
		angle = maxangle;

	if (orientation.ComputeSteering(targetLoc) >= 0)
		SetRotation(GetRotation() + angle);
	else
		SetRotation(GetRotation() - angle);

	orientation *= (float)GetMoveSpeed();
	SetVelocity(orientation);

	if (CheckCollision(GetPosition() + GetVelocity()*elapsedTime))
		Entity::Update(elapsedTime);
}

void NeutralEnemy::UpdateCaution(float elapsedTime)
{
	//run away if HP is low
}

void NeutralEnemy::UpdatePatrol(float elapsedTime)
{
	if (m_currPoint >= m_Waypoints.size() || m_currPoint < 0)
	{
		m_currPoint = 0;
		m_myState = AI_IDLE;
		m_StateTimer = 3.0f;
	}
	if (m_Waypoints.size() > 0)
	{
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());
		
		SGD::Vector direction = m_Waypoints[m_currPoint] - GetPosition();
		if (direction.ComputeLength() < 25)
			m_currPoint++;

		direction.Normalize();

		if (direction.ComputeDotProduct(orientation) < 0.999)
		{
			if (direction.ComputeSteering(orientation) >= 0)
				SetRotation(GetRotation() - SGD::PI*elapsedTime);
			else
				SetRotation(GetRotation() + SGD::PI*elapsedTime);
		}

		orientation *= (float)GetMoveSpeed();
		SetVelocity(orientation);
		
		if (CheckCollision(GetPosition() + GetVelocity()*elapsedTime))
			Entity::Update(elapsedTime);
	}
}

/*virtual*/ void NeutralEnemy::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_SWORD)
	{
		if (GetCurrHP() <= 0)
		{
			CreateDrop();
		}
		else
		{
			Sword* mySw = (Sword*)pOther;
			if (mySw->GetOwner() != nullptr)
			{
				SetTarget(mySw->GetOwner());
				m_myState = AI_ATTACK;
				m_StateTimer = 1.5f;
			}
		}
	}
	if (pOther->GetType() == ENT_SMOKEBOMB)
	{
		m_myState = AI_IDLE;
		m_StateTimer = 1.5f;
		SetTarget(nullptr);

	}
}

void NeutralEnemy::CreateDrop()
{
	//create drop
	int isloot = rand() % 25 + 1;
	if (isloot <= 25 && GetHavePizza() == false)
	{
		int loottype = rand() % 100 + 1;

		//fix level

		if (loottype <= 20) //Health
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(4, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 20 && loottype <= 50) //Money
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(3, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 50 && loottype <= 75) //Time
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(2, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 75 && loottype <= 86) //smokebomb
		{
			GamePlayState::GetInstance()->SpawnSmokeBomb(GetPosition());
		}
		else if (loottype > 80 && loottype <= 90) //Strength
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(0, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 90) //speed
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(1, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
	}
	SGD::Event* mydeath = new SGD::Event{ "EnemyDeleted", "", this };

	mydeath->QueueEvent(nullptr);
	mydeath = nullptr;

	DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

	pMsg->QueueMessage();
	pMsg = nullptr;
}