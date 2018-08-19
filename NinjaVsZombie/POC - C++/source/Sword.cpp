#include "Sword.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "Player.h"

#include "DestroyEntityMessage.h"

#include "Game.h"

Sword::Sword(SGD::Point position, float rotation, Entity* owner)
{
	SetOwner(owner);
	SetRotation(rotation);
	m_startRot = rotation;
	SetPosition(position);
	
	SetSize({ 64, 64 });


	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/katana.png");
	m_SlashSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/sword.wav");
}


Sword::~Sword()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_SlashSound);
}


/*virtual*/ void	Sword::Update(float elapsedTime)		/*override;*/
{

	if (GetOwner() != nullptr)
	{
		if (GetRotation() == 0)
			SetPosition(GetOwner()->GetPosition());
		else if (GetRotation() == SGD::PI)
			SetPosition(GetOwner()->GetPosition() - GetSize());
		else if (GetRotation() == SGD::PI / 2)
			SetPosition({ GetOwner()->GetPosition().x - GetSize().width, GetOwner()->GetPosition().y });
		else if (GetRotation() == 3 * (SGD::PI / 2))
			SetPosition({ GetOwner()->GetPosition().x, GetOwner()->GetPosition().y - GetSize().height });
	}

	if (GetIsSwinging() && m_endRot - m_startRot < SGD::PI / 2)
		Swing(elapsedTime);

	else if (m_endRot - m_startRot > SGD::PI / 2)
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
		Player* player = (Player*)GetOwner();
		player->SetSwordActive(false);
	}
}

/*virtual*/ void	Sword::Render(void)					/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255,255,255 }, {}, 1);

	if (GetOwner() != nullptr)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), GetOwner()->GetPositionInWorldSpace(), GetRotation(), {}, {}, { 0.3f, 0.5f });
		SGD::GraphicsManager::GetInstance()->DrawLine(GetOwner()->GetPositionInWorldSpace(), GetPositionInWorldSpace(), { 255, 0, 0 }, 3);
	}
	else
		SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), { GetPositionInWorldSpace().x - 32, GetPositionInWorldSpace().y + 32 }, GetRotation(), GetSize() / 2, {}, { 0.5f, 0.5f });
	
}

/*virtual*/ void	Sword::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		if (GetOwner() == nullptr)
		{
			Player* player = (Player*)pOther;
			if (!player->HaveSword())
			{
				player->PlayPickupSound();
				player->SetHaveSword(true);
			}

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
	else if (pOther->GetType() == ENT_ZOMBIE)
	{
		if (m_attacked != pOther)
		{
			Player* player = (Player*)GetOwner();
			m_attacked = (Unit*)pOther;
			m_attacked->SetCurrHP(m_attacked->GetCurrHP() - (10 * (int)player->GetSwordLevel()));
		}
	}
}

void Sword::Swing(float elapsedTime)
{
	if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_SlashSound))
		SGD::AudioManager::GetInstance()->PlayAudio(m_SlashSound);

	m_endRot = CONVERTTODEG*GetRotation();

	m_endRot = m_endRot + 270.0f*elapsedTime;

	m_endRot = m_endRot * CONVERTTORAD;

	SetRotation(m_endRot);
}