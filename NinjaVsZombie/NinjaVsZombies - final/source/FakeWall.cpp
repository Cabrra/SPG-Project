#include "FakeWall.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Player.h"
#include "Game.h"
#include "CreateEmiterMessage.h"
#include "DeleteEmitterMessage.h"
#include <cassert>

/**************************************************************/
// CONSTRUCTOR
FakeWall::FakeWall() : Listener(this)
{
	Listener::RegisterForEvent("PLAYER_SPOTTED");
	Listener::RegisterForEvent("PLAYER_OUT");
	Listener::RegisterForEvent("Target");

	m_myState = Enemy::AI_PATROL;
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/FakeWall.png");
	m_hHide = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Statue.png");
	SetSize({ 32, 32 });
	SetScale({ 1.0f, 1.0f });
}


/**************************************************************/
// DESTRUCTOR
/*virtual*/ FakeWall::~FakeWall(void)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHide);


	SetTarget(nullptr);
}


/**************************************************************/
// Update
//	- move the entity's position by its velocity
/*virtual*/ void FakeWall::Update(float elapsedTime)
{
	//if (m_particleTimer > 0)
	//	m_particleTimer -= elapsedTime;
	//if (m_ParticleActivated == true && m_particleTimer < 0)
	//{
	//	DeleteEmitterMessage* delemi = new DeleteEmitterMessage(this);
	//	delemi->QueueMessage();
	//	delemi = nullptr;
	//}

	if (m_Target != nullptr)
	{
		SGD::Rectangle myrect = GetRect();
		if (myrect.IsIntersecting(m_Target->GetRect()) == false || m_myState == Enemy::AI_ALERT)
		{
			m_Target->SetSafeZone(false);
			m_ParticleActivated = false;
		}
	}
}


/**************************************************************/
// Render
//	- draw the entity's image at its position
/*virtual*/ void FakeWall::Render(void)
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	if (m_Target != nullptr)
	{
		if (GetRect().IsIntersecting(m_Target->GetRect()) && m_myState == Enemy::AI_PATROL)
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_hHide, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, { 0.7f , 0.7f });
		else
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, GetScale());
	}
	else
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, GetScale());

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 0);
}

/**************************************************************/
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void FakeWall::HandleCollision(const IEntity* pOther)
{

	if (pOther->GetType() == ENT_PLAYER)
	{
		Player* plyr = (Player*)pOther;
		SGD::Vector distance = plyr->GetPosition() - GetPosition();
		if (distance.ComputeLength() < 32)
		{
			if (m_myState == Enemy::AI_PATROL)
			{
				plyr->SetSafeZone(true);
				//if (m_ParticleActivated == false)
				//{
				//	CreateEmiterMessage* emi = new CreateEmiterMessage(this, 5);
				//	emi->SendMessageNow();
				//	delete emi;
				//	emi = nullptr;
				//	m_particleTimer = 1.5f;
				//	m_ParticleActivated = true;
				//}
			}
			else
			{
				plyr->SetSafeZone(false);
			}
		}
	}
}

void FakeWall::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "PLAYER_SPOTTED")
	{
		m_myState = Enemy::AI_ALERT;

	}
	else if (pEvent->GetEventID() == "PLAYER_OUT")
	{
		m_myState = Enemy::AI_PATROL;
	}
	else if (pEvent->GetEventID() == "Target")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender->GetType() == ENT_PLAYER && m_Target == nullptr)
		{
			Player* plr = (Player*)sender;
			SetTarget(plr);
		}
	}
}


void FakeWall::SetTarget(Player* newTarget)
{
	if (m_Target != nullptr)
	{
		m_Target->Release();
		m_Target = nullptr;
	}

	m_Target = newTarget;

	if (m_Target != nullptr)
		m_Target->AddRef();
}