#include "Pizza.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Hook.h"
#include "Player.h"

#include "Game.h"

#include "DestroyEntityMessage.h"

Pizza::Pizza()
{
	SetSize({ 32, 32 });

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/pizza.png");
}


Pizza::~Pizza()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/*virtual*/ void	Pizza::Update(float elapsedTime)		/*override;*/
{

}

/*virtual*/ void	Pizza::Render(void)					/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 1);

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), 0.0f, {}, {}, { 1.0f, 1.0f });
}

/*virtual*/ void	Pizza::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_HOOK)
	{

		const Hook* hook = dynamic_cast< const Hook* >(pOther);
		Player* player =(Player*)(hook->GetOwner());

		player->PlayPickupSound();
		player->SetHavePizza(true);

		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
	}
	else if (pOther->GetType() == ENT_PLAYER)
	{
		Player* player = (Player*)pOther;

		player->PlayPickupSound();
		player->SetHavePizza(true);

		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}