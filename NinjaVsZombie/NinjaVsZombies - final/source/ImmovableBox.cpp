#include "ImmovableBox.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Hook.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "Game.h"

ImmovableBox::ImmovableBox()
{
	SetSize({ 32, 32 });
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/MetalBox.png");
}

ImmovableBox::~ImmovableBox()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/*virtual*/ void	ImmovableBox::Update(float elapsedTime)		/*override;*/
{

}

/*virtual*/ void	ImmovableBox::Render(void)					/*override;*/
{
	/*SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 255 }, {});*/
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), 0.0f, {}, {}, { 0.3f, 0.3f });
}

/*virtual*/ void	ImmovableBox::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_HOOK)
	{
		Hook* hook = (Hook*)pOther;
		Player* player = (Player*)hook->GetOwner();
		float num = (player->GetPosition() - hook->GetPosition()).ComputeLength();
		if (m_hook != hook && ((player->GetPosition() - hook->GetPosition()).ComputeLength() <= 150 * player->GetHookLevel()))
		{
			m_hook = hook;
			SGD::Event* pEvent = new SGD::Event{ "MovePlayer" };
			pEvent->QueueEvent();
			pEvent = nullptr;
		}
	}
}