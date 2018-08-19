#include "MovableBox.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "Hook.h"

#include "Game.h"

MovableBox::MovableBox()
{
	SetPosition({ 250, 250 });
	SetSize({32,32});

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/wood-box-md.png");
}

MovableBox::~MovableBox()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/*virtual*/ void	MovableBox::Update(float elapsedTime)		/*override;*/
{

}

/*virtual*/ void	MovableBox::Render(void)					/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x,Game::GetInstance()->GetWorldSpace().y);
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 1);
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), 0.0f, {}, {}, { 0.07f, 0.07f });
}

/*virtual*/ void	MovableBox::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_PLAYER)
		return;
	else if (pOther->GetType() == ENT_HOOK)
	{
		const Hook* moveBox = dynamic_cast< const Hook* >(pOther);
		SetPosition(moveBox->GetPosition()-moveBox->GetSize()/2);
	}
}