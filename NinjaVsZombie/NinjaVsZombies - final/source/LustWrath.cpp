#include "LustWrath.h"

#include "AnimationManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "DestroyEntityMessage.h"
#include "CreateHeartMessage.h"
#include "Game.h"
#include "GamePlayState.h"

#include "Player.h"

#define CalmSpeed 125
#define RageSpeed 250

LustWrath::LustWrath() : Listener(this)
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/health_PowerUp.png");
	Listener::RegisterForEvent("Target");
	Listener::RegisterForEvent("TargetDeleted");
	Listener::RegisterForEvent("ControlPlayer");
	SetMoveSpeed(CalmSpeed);
	SetCurrHP(200);

	//m_fKissTimer = 0.0f;
	m_fAttackTimer = 0.5f;
	m_fDeathTimer = 3.0f;
	m_fIdleTimer = 2.0f;
	m_fMoveTimer = 3.5f;
	m_fKissTimer = 1.5f;	
}


LustWrath::~LustWrath()
{
	if (GetTarget() != nullptr)
		((Player*)GetTarget())->SetCharmed(false);

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::Event* take = new SGD::Event{ "DEFEATED", nullptr, this };
	take->QueueEvent(nullptr);
	take = nullptr;
}

void LustWrath::Update(float elapsedTime)
{	 
	if (GetMoveSpeed() == 0 && m_bWrath == false)
		SetMoveSpeed(CalmSpeed);
	else if (GetMoveSpeed() == 0 && m_bWrath == true)
		SetMoveSpeed(RageSpeed);	

	switch (GetCurrState())
	{
	case IDLE:
		Idle(elapsedTime);
		break;

	case APPROACH:
		Approach(elapsedTime);
		break;

	case FLEE:
		Flee(elapsedTime);
		break;

	case ATTACK:
		Attack(elapsedTime);
		break;

	case FLIRT:
		Flirt(elapsedTime);
		break;

	case DEAD:
		m_fDeathTimer += elapsedTime;
		if (m_fDeathTimer > 2)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
		break;

	default:
		break;
	}

	if (CheckCollision(GetPosition() + GetVelocity()*elapsedTime))
		Entity::Update(elapsedTime);

}	 
	 
void LustWrath::Render(void)
{	 
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	//
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 4);

	//if (m_kiss)
	//{
	//	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(),
	//	{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
	//	0, {}, { 255, 0, 127 }, {1.0f,1.0f});
	//}
	//else if (m_att)
	//{
	//	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(),
	//	{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
	//	0, {}, { 255, 0, 0 }, { 1.0f, 1.0f });
	//}
	//else if (m_move)
	//{
	//	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(),
	//	{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
	//	0, {}, { 0, 0, 255 }, { 1.0f, 1.0f });
	//}
	//else if (m_dead)
	//{
	//	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(),
	//	{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
	//	0, {}, { 0, 255, 0 }, { 1.0f, 1.0f });
	//}
	//else
	//{
	//	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(),
	//	{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
	//	0, {}, { 255, 255, 255 }, { 1.0f, 1.0f });
	//}

	if (m_kiss)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x , (int)GetPosition().y - (int)(GetSize().height / 2 - 35), m_Kissing, flipped, m_Scale);
	}
	else if (m_att)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x , (int)GetPosition().y - (int)(GetSize().height / 2 - 35), m_Attacking, flipped, m_Scale);
	}
	else if (m_move)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x , (int)GetPosition().y - (int)(GetSize().height / 2 - 35 ), m_Walking, flipped, { 1.5f, 1.5f });
	}
	else
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y - (int)(GetSize().height / 2 - 35 ), m_Idle, flipped);
	}
}	 
	 
void LustWrath::HandleCollision(const IEntity* pOther)
{	 
	if (pOther->GetType() == ENT_SWORD && m_bAttacked == false)
	{
		SetCurrHP(GetCurrHP() - 20);
		m_bAttacked = true;
		m_CurrState = FLEE;

		if (GetCurrHP() <= 0)
		{
			m_dead = true;
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}

}	 
	 
void LustWrath::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "Target")
	{
		Entity* newTarget = (Entity*)pEvent->GetSender();
		if (newTarget->GetType() == ENT_PLAYER)
		{
			SetTarget(newTarget); //boss always knows where is player
		}
	}
	if (pEvent->GetEventID() == "ControlPlayer")
	{
		Entity* controlTarget = (Entity*)pEvent->GetSender();
		if (controlTarget->GetType() != ENT_PLAYER)
			return;

		m_ptControlPos = GetPosition();
	}
}

SGD::Rectangle LustWrath::GetRect(void)const
{
	SGD::Rectangle myrect = { { m_ptPosition.x-32,m_ptPosition.y-32 }, SGD::Size{ 64, 64 } };
	//myrect.Offset({ 75, 75 });
	return myrect;
}

void LustWrath::Flirt(float elapsedTime)
{
	if (m_fKissTimer > 0)
		m_fKissTimer -= elapsedTime;

	if (GetTarget() != nullptr && GetTarget()->GetType() == ENT_PLAYER)
	{
		m_kiss = true;
		//create bullet heart img
		AnimationManager::GetInstance()->Update(elapsedTime, m_Kissing);
		if (((Player*)GetTarget())->GetCharmed() == false)
		{
			if (m_bHeartActive == false)
			{
				CreateHeartMessage* pMsg = new CreateHeartMessage(this);
				pMsg->QueueMessage();
				pMsg = nullptr;

				m_bHeartActive = true;
			}
		}

		if (m_fKissTimer <= 0)
		{
			/*if (((Player*)GetTarget())->GetCharmed() == true)
			{

			}
			else*/
			{
				/*((Player*)GetTarget())->SetCharmed(false);*/
				m_kiss = false;
				m_bHeartActive = false;
				m_fMoveTimer = 0;
				m_bWrath = true;
				SetMoveSpeed(0);
				SetCurrState(IDLE);
				m_fIdleTimer = 2.0f;
			}

		}

	}
	else if (m_bHeartActive == false && ((Player*)GetTarget())->GetCharmed() == false)
	{
		//m_kiss = false;
		m_bWrath = true;
		SetCurrState(IDLE);
		m_fIdleTimer = 2.0f;
	}

}

void LustWrath::Approach(float elapsedTime)
{
	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	m_fMoveTimer -= elapsedTime;

	if (GetTarget() != nullptr && GetTarget()->GetType() == ENT_PLAYER)
	{
		SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();
		if (m_fMoveTimer > 0)
		{
			if (targetLoc.ComputeLength() > 64)
			{
				m_move = true;
				AnimationManager::GetInstance()->Update(elapsedTime, m_Walking);
				if (m_bWrath == true)
					SetMoveSpeed(RageSpeed);
				else
					SetMoveSpeed(CalmSpeed);
			}
			else
			{
				m_move = false;
				SetMoveSpeed(0);
				SetCurrState(ATTACK);
				m_fAttackTimer = 0.50f;
			}
		}
		else
		{
			m_move = false;
			m_fMoveTimer = 0;
			m_bWrath = false;
			SetMoveSpeed(0);
			SetCurrState(IDLE);
			m_fIdleTimer = 2.0f;
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
		if (GetRotation() >= SGD::PI / 2 && GetRotation() <= (SGD::PI * 3) / 2)
		{
			flipped = false;
		}
		else
		{
			flipped = true;
		}
	}
}

void LustWrath::Idle(float elapsedTime)
{
	m_idle = true;
	m_att = false;
	m_kiss = false;
	SetMoveSpeed(0);
	AnimationManager::GetInstance()->Update(elapsedTime, m_Idle);
	m_fIdleTimer -= elapsedTime;

	if (m_fIdleTimer <= 0.0f)
	{
		m_bAttacked = false;
		m_idle = false;

		if (m_bWrath == true)
		{
			SetCurrState(APPROACH);
			m_fMoveTimer = 3.5f;
		}
		else
		{
			SetCurrState(FLIRT);
			m_fKissTimer = 1.5f;
		}
	}
	else if (((Player*)GetTarget())->GetCharmed() == true)
	{
		SetCurrState(APPROACH);
		m_fMoveTimer = 3.5f;
	}
}

void LustWrath::Flee(float elapsedTime)
{
	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	SGD::Vector targetLoc = m_ptFleePos - GetPosition();
	if (targetLoc.ComputeLength() > 100)
	{
		m_move = false;
		m_bWrath = false;
		SetCurrState(IDLE);
		m_fIdleTimer = 2.0f;
	}
	else
	{
		m_move = true;
		SetMoveSpeed(CalmSpeed);
		AnimationManager::GetInstance()->Update(elapsedTime, m_Walking);
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
	SetVelocity(-orientation);
	if (GetRotation() >= SGD::PI / 2 && GetRotation() <= (SGD::PI * 3) / 2)
	{
		flipped = false;
	}
	else
	{
		flipped = true;
	}
}

void LustWrath::Attack(float elapsedTime)
{
	m_fAttackTimer -= elapsedTime;
	m_att = true;
	AnimationManager::GetInstance()->Update(elapsedTime, m_Attacking);

	if (m_fAttackTimer <= 0 && m_bAttacked == false)
	{
		m_fAttackTimer = 0;

		SGD::Event* take = new SGD::Event{ "Player_HIT", nullptr, this };
		take->QueueEvent(nullptr);
		take = nullptr;
		SetCurrState(IDLE);
		m_fIdleTimer = 2.0f;
	}
	else if (m_bAttacked == true)
	{
		m_att = false;
		m_bWrath = false;
		SetCurrState(FLEE);
		m_fMoveTimer = 2.5f;
	}
}
