#include "Zombie.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "DestroyEntityMessage.h"

#include "Game.h"

Zombie::Zombie()
{
	SetSize({ 64, 64 });
	SetPosition({200,400});

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Zombie_01.png");
}


Zombie::~Zombie()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/*virtual*/ void			Zombie::Update(float elapsedTime)		/*override;*/
{

}

/*virtual*/ void			Zombie::Render(void)					/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 0);

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), GetRotation(), {}, {}, { 0.5f, 0.5f });
}

/*virtual*/ void			Zombie::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_SWORD)
	{
		if (GetCurrHP() <= 0)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}