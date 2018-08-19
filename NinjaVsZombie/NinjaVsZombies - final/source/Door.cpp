#include "Door.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Player.h"

Door::Door(bool vertical) : Listener(this)
{
	Listener::RegisterForEvent("DEFEATED");
	/*if (vertical)
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/door_gate.png");
	else
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/door_gate2.png");*/

}

Door::~Door()
{
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void Door::Update(float elapsedTime)
{
	if (m_bUnlocked == true)
	{
		SetSize({ 0, 0 });
	}
	else
	{
		if (m_bVertical)
			SetSize({ 32, 64 });
		else
			SetSize({ 64, 32 });
		//SetSize({ 0, 0 });
	}

	//if (m_bVertical)
	//{
		if (m_bOpen)
			AnimationManager::GetInstance()->Update(elapsedTime, ts1);
		else if (m_bClose)
			AnimationManager::GetInstance()->Update(elapsedTime, ts2);
		else if (m_bUnlock)
			AnimationManager::GetInstance()->Update(elapsedTime, ts3);
	//}
}

void Door::Render(void)
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 4);
	if (m_bActive && m_bUnlocked == false)
	{
		if (m_bVertical)
		{
			if (m_bOpen)
				AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y + 2, ts1, false, { 1.0f, 1.0f });
			else if (m_bClose)
				AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y + 2, ts2, false, { 1.0f, 1.0f });
			else if (m_bUnlock)
				AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y + 2, ts3, false, { 1.0f, 1.0f });
			else
			{
				AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y + 2, ts1, false, { 1.0f, 1.0f });
			}
			/*SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(),
				SGD::Point(GetPosition().x + Game::GetInstance()->GetWorldSpace().x,
				GetPosition().y + Game::GetInstance()->GetWorldSpace().y));*/
		}
		else
		{
			if (m_bOpen)
				AnimationManager::GetInstance()->Render((int)GetPosition().x + 2, (int)GetPosition().y, ts1, false, { 1.0f, 1.0f });
			else if (m_bClose)
				AnimationManager::GetInstance()->Render((int)GetPosition().x + 2, (int)GetPosition().y, ts2, false, { 1.0f, 1.0f });
			else if (m_bUnlock)
				AnimationManager::GetInstance()->Render((int)GetPosition().x + 2, (int)GetPosition().y, ts3, false, { 1.0f, 1.0f });
			else
			{
				AnimationManager::GetInstance()->Render((int)GetPosition().x + 2, (int)GetPosition().y, ts1, false, { 1.0f, 1.0f });
			}
			/*SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(),
				SGD::Point(GetPosition().x + Game::GetInstance()->GetWorldSpace().x,
				GetPosition().y + Game::GetInstance()->GetWorldSpace().y));*/
		}
	}
}

SGD::Rectangle Door::GetRect(void) const
{
	return SGD::Rectangle{ {m_ptPosition.x-(m_szSize.width/2),m_ptPosition.y-(m_szSize.height/2) }, m_szSize};
}

void Door::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		if (m_bUnlocked == true)
			((Player*)pOther)->SetDoorCollide(false);
		else
			((Player*)pOther)->SetDoorCollide(true);
		/*Player* mypla = (Player*)pOther;
		if (mypla->GetHasKey())
		{
			m_bActive = false;
			mypla->SetHasKey(false);
		}*/
	}
}

void Door::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "DEFEATED" && Game::GetInstance()->GetWorldCamera().IsIntersecting(GetRect()))
	{
		SetUnlocked(true);
	}

}