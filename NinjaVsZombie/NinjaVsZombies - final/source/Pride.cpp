#include "Pride.h"
#include "AnimationManager.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "DestroyEntityMessage.h"
#include "FireTrap.h"
#include "LightingTrap.h"
#include "SpikeTrap.h"
#include "Trap.h"


Pride::Pride() : Listener(this)
{
	Listener::RegisterForEvent("Target");
	SetSize({ 64, 128 });
	SetScale({ 1.2f, 1.2f });
	SetPosition({ 200, 100 });
	SetMoveSpeed(200);
}
Pride::~Pride()
{
	SGD::Event* take = new SGD::Event{ "DEFEATED", nullptr, this };
	take->QueueEvent(nullptr);
	take = nullptr;
}
/*virtual*/ void			Pride::Update(float elapsedTime)		/*override*/
{
	
	if (m_taunt)
	{
		AnimationManager::GetInstance()->Update(elapsedTime, m_Taunting);
		m_taunttimer += elapsedTime;
		if (m_taunttimer > 2)
		{
			m_taunt = false;
			m_taunttimer = 0;
		}
	}
	else if (m_takeaknee)
	{
		m_kneetimer += elapsedTime;
		AnimationManager::GetInstance()->Update(elapsedTime, m_TakeaKnee);
		if (m_kneetimer > 10)
		{
			m_numTrapsTapsTaken = 0;
			m_kneetimer = 0;
			m_takeaknee = false;
		}
	}
	else if (m_dead)
	{
		AnimationManager::GetInstance()->Update(elapsedTime, m_Dying);
		m_deathtimer += elapsedTime;
		if (m_deathtimer > 2)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
	else
	{
		AnimationManager::GetInstance()->Update(elapsedTime, m_Idle);
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());

		if (GetTarget() != nullptr && GetTarget()->GetType() == ENT_PLAYER)
		{
			SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();
			if (targetLoc.ComputeLength() < 50)
			{
				SetMoveSpeed(0);
				m_attacktimer += elapsedTime;
				AnimationManager::GetInstance()->Update(elapsedTime, m_Attacking);
				m_att = true;
				if (m_attacktimer > 3.5f)
				{
					m_attacktimer = 0;
					SGD::Event* take = new SGD::Event{ "Player_HIT", nullptr, this };
					take->QueueEvent(nullptr);
					take = nullptr;
				}

			}
			else
			{
				AnimationManager::GetInstance()->Update(elapsedTime, m_Walking);
				m_attacktimer = 0;
				SetMoveSpeed(200);
				m_att = false;
				m_move = true;
			}

			float angle = orientation.ComputeAngle(targetLoc);

			float maxangle = SGD::PI / 2 * elapsedTime;

			if (GetRotation() >= SGD::PI / 2 && GetRotation() <= (SGD::PI * 3) / 2)
			{
				flipped = true;
			}
			else
			{
				flipped = false;
			}
			if (angle > maxangle)
				angle = maxangle;
			if (orientation.ComputeSteering(targetLoc) >= 0)
			{
				SetRotation(GetRotation() + angle);
			}
			else
			{
				SetRotation(GetRotation() - angle);
			}

			orientation *= (float)GetMoveSpeed();
			SetVelocity(orientation);
		}

	}
	if (CheckCollision(GetPosition() + GetVelocity()*elapsedTime) && !m_taunt)
		Entity::Update(elapsedTime);

}
/*virtual*/ void			Pride::Render(void)					/*override*/
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	//
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 4);
	//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, GetScale());
	if (m_taunt)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 125), m_Taunting, flipped, m_Scale);
	}
	else if (m_att)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 125), m_Attacking, flipped, m_Scale);
	}
	else if (m_move)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 125), m_Walking, flipped, m_Scale);
	}
	else if (m_dead)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 125), m_Dying, flipped, m_Scale);
	}
	else if (m_takeaknee)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 125), m_TakeaKnee, flipped, m_Scale);
	}
	else
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 125), m_Idle, flipped);
	}
	//Entity::Render();
}
/*virtual*/ void			Pride::HandleCollision(const IEntity* pOther)	/*override*/
{
	if (pOther->GetType() == ENT_SWORD)
	{
		if (m_numTrapsTapsTaken <= 4 && m_numTrapsTapsTaken >= 0)
		{
			m_taunt = true;
		}
		else
		{
			m_dead = true;
			isDead = true;
		}
	}
	if (pOther->GetType() == ENT_FIRETRAP)
	{
		FireTrap* off = (FireTrap*)pOther;
		off->SetActive(false);
		m_numTrapsTapsTaken++;
		if (m_numTrapsTapsTaken > 4)
		{
			m_move = false;
			m_takeaknee = true;
		}
	}
	if (pOther->GetType() == ENT_ELECTRICTRAP)
	{
		LightingTrap* off = (LightingTrap*)pOther;
		off->SetActive(false);
		m_numTrapsTapsTaken++;
		if (m_numTrapsTapsTaken > 4)
		{
			m_move = false;
			m_takeaknee = true;
		}
	}
	if (pOther->GetType() == ENT_SPIKETRAP)
	{
		SpikeTrap* off = (SpikeTrap*)pOther;
		off->SetActive(false);
		m_numTrapsTapsTaken++;
		if (m_numTrapsTapsTaken > 4)
		{
			m_move = false;
			m_takeaknee = true;
		}
	}
}
/*virtual*/ void			Pride::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "Target")
	{
		Entity* newTarget = (Entity*)pEvent->GetSender();
		if (GetTarget() == nullptr)
		{
			SGD::Vector distance = newTarget->GetPosition() - GetPosition();
			if (distance.ComputeLength() < 200)
			{
				distance.Normalize();
				SGD::Vector orientation = { 0, -1 };
				orientation.Rotate(GetRotation());
				if (distance.ComputeDotProduct(orientation)> 0.80)
				{
					if (newTarget->GetType() != ENT_PIZZA)
					{
						SetTarget(newTarget);
					}

					SGD::Event* myEvent = new SGD::Event{ "PLAYER_SPOTTED", "", this };

					myEvent->QueueEvent(nullptr);
					myEvent = nullptr;
				}
			}
		}
	}
}