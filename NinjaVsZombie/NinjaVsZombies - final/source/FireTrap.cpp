#include "FireTrap.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "CreateEmiterMessage.h"
#include "DeleteEmitterMessage.h"
#include "Player.h"

#define EndTick 2

FireTrap::FireTrap()
{
	SetSize({ 32, 32 });
}

FireTrap::~FireTrap()
{
	if (GetActive() == true)
	{
		DeleteEmitterMessage* deletmi = new DeleteEmitterMessage(this);
		deletmi->QueueMessage();
		deletmi = nullptr;
	}
	
}

void FireTrap::Update(float elapsedTime)
{
	//if (GetActive() == true)
	//{
		//m_pParticle->Update(elapsedTime);
	///}

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
		m_nCurrTick = 0;
	}

}

 void FireTrap::Render(void)
{
	 if (GetActive() == true)
	 {
		 SGD::Rectangle rect = GetRect();
		 rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

		 /*SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {255,0,0,0}, 4);

		 SGD::OStringStream oss;
		 oss << m_nCurrTick;

		 SGD::GraphicsManager::GetInstance()->DrawString(oss.str().c_str(),
		 { rect.left + (GetSize().width / 4), rect.top + (GetSize().height / 4) }, { 0, 0, 0 });*/
	 }
}

 void FireTrap::HandleCollision(const IEntity* pOther)
 {
	 if (pOther->GetType() == ENT_PLAYER && GetActive() == true && ((Player*)pOther)->IsFalling() == false)
	 {
		 
		SGD::Event* hitEvent = new SGD::Event{ "Player_HIT",nullptr,this };
		hitEvent->QueueEvent(pOther);
		hitEvent = nullptr;

		m_nCurrTick = 0;
	 }
 }

 void FireTrap::SetActive(bool active)
 {
	 Trap::SetActive(active);
	 if (active == true)
	 {
		 CreateEmiterMessage* fireemi = new CreateEmiterMessage(this, 3); //fire

		 fireemi->QueueMessage();
		 fireemi = nullptr;
	 }
	 else
	 {
		 DeleteEmitterMessage* deletmi = new DeleteEmitterMessage(this);
		 deletmi->QueueMessage();
		 deletmi = nullptr;
	 }
 }