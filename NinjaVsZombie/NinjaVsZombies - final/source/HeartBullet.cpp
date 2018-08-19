#include "HeartBullet.h"
#include "DestroyEntityMessage.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


HeartBullet::HeartBullet(SGD::Point position, float rotation, Entity* owner) : Bullet(position,rotation,owner)
{
	SetSize({16, 16 });
}


HeartBullet::~HeartBullet()
{
}

void HeartBullet::Update(float elapsedTime)
{
	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	m_Heartlife -= elapsedTime;

	if (GetTarget() != nullptr && GetTarget()->GetType() == ENT_PLAYER)
	{
		SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();
		if (m_Heartlife > 0)
		{
			SetSpeed(185);
		}
		else
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}

		float angle = orientation.ComputeAngle(targetLoc);

		float maxangle = SGD::PI / 1.5f * elapsedTime;

		if (angle > maxangle)
			angle = maxangle;

		if (orientation.ComputeSteering(targetLoc) >= 0)
			SetRotation(GetRotation() + angle);
		else
			SetRotation(GetRotation() - angle);

		orientation *= GetSpeed();
		SetVelocity(orientation);

		m_ptPosition += m_vtVelocity * elapsedTime;
	}
}

void HeartBullet::Render(void)
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {});
	SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), GetPositionInWorldSpace());
}

void HeartBullet::HandleCollision(const IEntity* pOther)
{
	Bullet::HandleCollision(pOther);
}