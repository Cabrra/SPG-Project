#include "FakeTrap.h"
#include "DestroyEntityMessage.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Player.h"

#define BreakTick 1.5

FakeTrap::FakeTrap()
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Breakablefloortiles.png");
}

FakeTrap::~FakeTrap()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void FakeTrap::Update(float elapsedTime)
{
	if (GetTickNow() == true && GetActive() == true)
	{
		damageTimer += elapsedTime;
		SetTickNow(false);
	}

}

void FakeTrap::Render(void)
{
	if (GetActive() == true)
	{
		SGD::Rectangle rect = GetRect();
		rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage,
		{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
		{ 0 + (32.0f*(int)(damageTimer/0.75f)), 0, 32 + (32.0f*(int)(damageTimer/0.75f)), 32 }, 0, {}, { 255, 255, 255 }, { 1.0f, 1.0f });

		//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 100, 0, 100 }, { 255, 0, 0, 0 }, 4);

		/*SGD::OStringStream oss;
		oss << m_nCurrTick;

		SGD::GraphicsManager::GetInstance()->DrawString(oss.str().c_str(),
		{ rect.left + (GetSize().width / 4), rect.top + (GetSize().height / 4) }, { 0, 0, 0 });*/
		//m_pAnimation->Render();
	}
}

void FakeTrap::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER && ((Player*)pOther)->IsFalling() == false)
	{
		SetTickNow(true);

		if (damageTimer >= BreakTick)
		{
			m_pTile->type = Tile::m_eTiletype::hole;
			m_pTile->m_nID = -1;
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}