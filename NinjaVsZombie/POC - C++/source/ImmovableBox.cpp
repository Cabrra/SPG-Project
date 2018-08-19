#include "ImmovableBox.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Hook.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "Game.h"

ImmovableBox::ImmovableBox()
{
	SetPosition({ 500, 500 });
	SetSize({ 128, 128 });
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
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 1);

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), 0.0f, {}, {}, { 1.0f, 1.0f });
}

/*virtual*/ void	ImmovableBox::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_HOOK)
	{
		SGD::Event* pEvent = new SGD::Event{ "MovePlayer" };
		pEvent->QueueEvent();
		pEvent = nullptr;
	}
}