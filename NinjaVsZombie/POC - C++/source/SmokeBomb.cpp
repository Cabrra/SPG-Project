#include "SmokeBomb.h"

#include "Game.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "DestroyEntityMessage.h"
#include "Player.h"

SmokeBomb::SmokeBomb(Entity* owner)
{
	m_owner = owner;
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/smokebomb.png");
	m_Sound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/air.wav");

	SetSize({ 16, 16 });
	
	if (GetOwner() != nullptr)
	{
		SetSize({ 128, 128 });
		if (GetRotation() == 0)
			SetPosition(GetOwner()->GetPosition());
		else if (GetRotation() == SGD::PI)
			SetPosition(GetOwner()->GetPosition() - GetSize());
		else if (GetRotation() == SGD::PI / 2)
			SetPosition({ GetOwner()->GetPosition().x - GetSize().width, GetOwner()->GetPosition().y });
		else if (GetRotation() == 3 * (SGD::PI / 2))
			SetPosition({ GetOwner()->GetPosition().x, GetOwner()->GetPosition().y - GetSize().height });
	}
}


SmokeBomb::~SmokeBomb()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_Sound);
}

/*virtual*/ void	SmokeBomb::Update(float elapsedTime)		/*override;*/
{
	if (GetOwner() != nullptr)
	{
		m_lifeSpan -= elapsedTime;
		if (m_lifeSpan <= 0)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_Sound))
			SGD::AudioManager::GetInstance()->PlayAudio(m_Sound);
	}
}

/*virtual*/ void	SmokeBomb::Render(void)					/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 255, 255 }, {}, 1);
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), GetRotation(), {}, {}, { 0.05f, 0.05f });
}

/*virtual*/ void	SmokeBomb::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		if (GetOwner()==pOther)
		{

		}
		else
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;

			Player* player = (Player*)pOther;
			player->PlayPickupSound();
			player->SetNumSmokeBombs(player->GetNumSmokeBombs() + 5);
		}
	}
}