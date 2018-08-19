

#include "Bullet.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "DestroyEntityMessage.h"

Bullet::Bullet(SGD::Point position, float rotation, Entity* owner)
{
	SetPosition(position);

	SetRotation(rotation);
	SetOwner(owner);
	SetSize({ 4, 4 });
	if (owner!=nullptr)
		GetOwner()->AddRef();

	SGD::Vector newVel = { 0, 1 };
	newVel.Rotate(rotation);
	newVel *= m_speed;
	SetVelocity(newVel);
}


Bullet::~Bullet()
{
	if (GetOwner() != nullptr)
		GetOwner()->Release();
}

/*virtual*/ void	Bullet::Render(void)					/*override;*/
{
	SGD::GraphicsManager::GetInstance()->DrawRectangle(GetRect(), {}, {});
	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), GetPosition());
}

/*virtual*/ void	Bullet::HandleCollision(const IEntity* pOther)	/*override;*/
{
	DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

	pMsg->QueueMessage();
	pMsg = nullptr;
}