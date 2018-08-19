#include "PressurePoint.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "AnimationManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Game.h"

PressurePoint::PressurePoint(SGD::Point pos)
{
	SetPosition(pos);
	SetSize({ 32, 32 });
	m_Compressed = false;
}

PressurePoint::~PressurePoint()
{
}

void PressurePoint::Update(float elapsedTime)
{
	if (m_Compressed)
		AnimationManager::GetInstance()->Update(elapsedTime, Compress);
	else
		AnimationManager::GetInstance()->Update(elapsedTime, Decompress);

	m_CompressedTimer -= elapsedTime;
	if (m_CompressedTimer <= 0)
	{
		Compress.currFrame = 1;
		m_Compressed = false;

		SGD::Event* activ = new SGD::Event("PRESSURE_DECOMPRESSED", nullptr, this);
		activ->QueueEvent();
		activ = nullptr;
	}
}

void PressurePoint::Render(void)
{
	SGD::Rectangle rect = GetRect();
	rect.Offset({ Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y });
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 0);

	if (m_Compressed)
		AnimationManager::GetInstance()->Render((int)(GetPosition().x + GetSize().width / 2), (int)(GetPosition().y + GetSize().height / 2), Compress, false, { 1.0f, 1.0f });
	else
		AnimationManager::GetInstance()->Render((int)(GetPosition().x + GetSize().width / 2), (int)(GetPosition().y + GetSize().height / 2), Decompress, false, { 1.0f, 1.0f });
}

void PressurePoint::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER || pOther->GetType() == ENT_MOVABLE || pOther->GetType() == ENT_ZOMBIE || pOther->GetType() == ENT_ARMOREDZOMBIE ||
		pOther->GetType() == ENT_AMBUSHNINJA || pOther->GetType() == ENT_NEUTRAL_ENEMY)
	{
		Decompress.currFrame = 1;
		m_Compressed = true;

		SGD::Event* activ = new SGD::Event("PRESSURE_COMPRESSED", nullptr, this);
		activ->QueueEvent();
		activ = nullptr;

		m_CompressedTimer = 0.25f;
	}
}