#include "LightingTrap.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "CreateEmiterMessage.h"
#include "DeleteEmitterMessage.h"
#include "Player.h"

#define EndTick 3

LightingTrap::LightingTrap()
{
	SetSize({ 32, 32 });
	
}


LightingTrap::~LightingTrap()
{
	if (GetActive() == true)
	{
		DeleteEmitterMessage* deletmi = new DeleteEmitterMessage(this);
		deletmi->QueueMessage();
		deletmi = nullptr;
	}
	//SetEmiter(nullptr);
}

void LightingTrap::Update(float elapsedTime)
{
	if (GetActive() == true)
	{
		//m_pParticle->Update(elapsedTime);
	}

	if (GetTickNow() == true && GetActive() == true)
	{
		damageTimer += elapsedTime;
		if (damageTimer > 1)
		{
			m_nCurrTick++;
			damageTimer = 0.0f;
		}
	}

	if (m_nCurrTick >= EndTick)
	{
		//m_hithim = true;
		m_nCurrTick = 0;
	}

}

void LightingTrap::Render(void)
{
	if (GetActive() == true)
	{
		SGD::Rectangle rect = GetRect();
		rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

		SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 25, 25, 25 }, { 255, 0, 0, 0 }, 4);

		/*SGD::OStringStream oss;
		oss << m_nCurrTick;

		SGD::GraphicsManager::GetInstance()->DrawString(oss.str().c_str(),
		{ rect.left + (GetSize().width / 4), rect.top + (GetSize().height / 4) }, { 0, 0, 0 });*/
		//m_pParticle->Render();
	}
}

void LightingTrap::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER && GetActive() == true && ((Player*)pOther)->IsFalling() == false)
	{
		SGD::Event* hitEvent = new SGD::Event{ "Player_HIT", nullptr, this };
		hitEvent->QueueEvent(pOther);
		hitEvent = nullptr;
		m_nCurrTick = 0;
	}
}


void LightingTrap::SetActive(bool active)
{
	Trap::SetActive(active);
	if (active == true)
	{
		CreateEmiterMessage* ligtemi = new CreateEmiterMessage(this, 4); //electric

		ligtemi->QueueMessage();
		ligtemi = nullptr;
	}
	else
	{
		DeleteEmitterMessage* deletmi = new DeleteEmitterMessage(this);
		deletmi->QueueMessage();
		deletmi = nullptr;
	}
}