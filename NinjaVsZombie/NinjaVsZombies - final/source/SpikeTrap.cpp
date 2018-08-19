#include "SpikeTrap.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include"../SGD Wrappers/SGD_String.h"
#include "Player.h"

#define RiseTick 3.0f
#define FallTick 0.5f

SpikeTrap::SpikeTrap()
{
	m_imgup = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/spike_up.png");
	m_imgdown = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/spike_down.png");
}


SpikeTrap::~SpikeTrap()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_imgup);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_imgdown);
}

void SpikeTrap::Update(float elapsedTime)
{
	if (GetActive() == true)
	{
		SetTickNow(true);
		//update animation
	}

	if (GetTickNow() == true && GetActive() == true)
	{
		damageTimer += elapsedTime;
		if (damageTimer > 1)
		{
			m_nCurrTick++;
			damageTimer = 0.0f;
		}

		if (isUp == false && m_nCurrTick >= RiseTick)
		{
			isUp = true;
			m_nCurrTick = 0;
		}
		else if (isUp == true && m_nCurrTick >= FallTick)
		{
			isUp = false;
			m_nCurrTick = 0;
			hitCount = 0;
			GamePlayState::GetInstance()->GetPlayer()->SetSpikeHit(false);
		}
	}

}

void SpikeTrap::Render(void)
{
	if (GetActive() == true)
	{
		SGD::Rectangle rect = GetRect();
		rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

		if (isUp == false)
		{
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_imgdown, 
			{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
				0, {}, { 255, 255, 255 }, { 1.0f,1.0f });
			//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 100, 100, 100 }, { 255, 0, 0, 0 }, 4);
		}
		else
		{
			SGD::GraphicsManager::GetInstance()->DrawTexture(m_imgup,
			{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
				0, {}, { 255, 255, 255 }, { 1.0f,1.0f });
			//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 255, 255 }, { 255, 0, 0, 0 }, 4);
		}

		/*SGD::OStringStream oss;
		oss << m_nCurrTick;

		SGD::GraphicsManager::GetInstance()->DrawString(oss.str().c_str() ,
		{ rect.left + (GetSize().width / 4), rect.top + (GetSize().height / 4) }, { 0, 0, 0 });*/
		//m_pAnimation->Render();
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_imgdown,
		{ GetPosition().x + Game::GetInstance()->GetWorldSpace().x, GetPosition().y + Game::GetInstance()->GetWorldSpace().y },
		0, {}, { 255, 255, 255 }, { 1.0f, 1.0f });
	}
}

void SpikeTrap::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER && GetActive() == true && ((Player*)pOther)->IsFalling() == false)
	{
		if (isUp == true && hitCount < 1 && GetRect().IsPointInRectangle(pOther->GetRect().ComputeCenter()))
		{
			SGD::Event* hitEvent = new SGD::Event{ "Player_HIT", nullptr, this };
			hitEvent->QueueEvent(pOther);
			hitEvent = nullptr;
			hitCount++;
		}
	}
}