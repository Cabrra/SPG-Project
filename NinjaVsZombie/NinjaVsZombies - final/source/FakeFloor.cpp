#include "FakeFloor.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Player.h"
#include "Game.h"
#include <cassert>
#include "CreateEmiterMessage.h"
#include "DeleteEmitterMessage.h"

FakeFloor::FakeFloor() : Listener(this)
{
	Listener::RegisterForEvent("PLAYER_SPOTTED");
	Listener::RegisterForEvent("PLAYER_OUT");
	Listener::RegisterForEvent("Target");

	m_myState = Enemy::AI_PATROL;
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/FakeFloor.png");
	m_hHide = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Ninjaeyes.png");
	SetSize({ 32, 32 });
	SetScale({ 1.0f, 1.0f });
}

FakeFloor::~FakeFloor()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHide);

	SetTarget(nullptr);
}

/*virtual*/ void FakeFloor::Update(float elapsedTime)
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
/*virtual*/ void FakeFloor::Render(void)
{

	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	if (m_Target != nullptr)
	{
		if (GetRect().IsIntersecting(m_Target->GetRect()) && m_myState == Enemy::AI_PATROL)
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_hHide, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, GetScale());
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
/*virtual*/ void FakeFloor::HandleCollision(const IEntity* pOther)
{

	if (pOther->GetType() == ENT_PLAYER)
	{
		Player* plyr = (Player*)pOther;
		SGD::Vector distance = plyr->GetPosition() - GetPosition();
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

void FakeFloor::HandleEvent(const SGD::Event* pEvent)
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

void FakeFloor::SetTarget(Player* newTarget)
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