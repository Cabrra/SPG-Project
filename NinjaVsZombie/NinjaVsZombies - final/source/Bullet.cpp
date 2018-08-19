

#include "Bullet.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Game.h"
#include "DestroyEntityMessage.h"

Bullet::Bullet(SGD::Point position, float rotation, Entity* owner)
{
	SetPosition(position);

	SetRotation(rotation);
	SetOwner(owner);
	SetSize({ 4, 4 });

	SGD::Vector newVel = { 0, 1 };
	newVel.Rotate(rotation);
	newVel *= m_speed;
	SetVelocity(newVel);
	m_bulletlife = 0;
}


Bullet::~Bullet()
{
	if (GetOwner() != nullptr)
		GetOwner()->Release();
}

/*virtual*/ void	Bullet::Render(void)					/*override;*/
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {});
	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), GetPositionInWorldSpace());
}

void Bullet::SetOwner(Entity* newOwner)
{
	if (m_eOwner != nullptr)
	{
		m_eOwner->Release();
	}
	m_eOwner = newOwner;
	if (m_eOwner != nullptr)
	{
		m_eOwner->AddRef();
	}
}


/*virtual*/ void	Bullet::HandleCollision(const IEntity* pOther)	/*override*/
{
	if ((pOther->GetType() == ENT_PLAYER) && pOther != m_eOwner)
	{
		SGD::Event* hitEvent = new SGD::Event{ "Player_HIT",nullptr,this };
		hitEvent->QueueEvent(pOther);
		hitEvent = nullptr;
		DestroyEntityMessage* dMsg = new DestroyEntityMessage(this);
		dMsg->QueueMessage();
		dMsg = nullptr;
	}
	if ((pOther->GetType() == ENT_SWORD))
	{
		DestroyEntityMessage* dMsg = new DestroyEntityMessage(this);
		dMsg->QueueMessage();
		dMsg = nullptr;
	}

}

/*virtual*/ void	Bullet::Update(float elapsedTime)		/*override*/
{
	Entity::Update(elapsedTime);
	//if (m_ptPosition.x > Game::GetInstance()->GetScreenWidth() + GetPosition().x || m_ptPosition.x < 0 || m_ptPosition.y > Game::GetInstance()->GetScreenHeight() + GetPosition().y || m_ptPosition.y < 0)
	//{
	//	
	//}
	m_bulletlife += elapsedTime;
	if (m_bulletlife >= 1.5f)
	{
		// queueing the bullet message
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

}