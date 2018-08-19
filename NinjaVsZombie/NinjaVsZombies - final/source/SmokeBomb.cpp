#include "SmokeBomb.h"

#include "Game.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Listener.h"

#include "DeleteEmitterMessage.h"
#include "DestroyEntityMessage.h"
#include "Player.h"
#include "Zombie.h"
#include "NeutralEnemy.h"
#include "ThrowingNinja.h"

SmokeBomb::SmokeBomb(Entity* owner) : Listener(this)
{
	Listener::RegisterForEvent("Pause_Sound");

	SetOwner(owner);
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/smokebomb.png");
	m_Sound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/air.wav");

	SetSize({ 16, 16 });
	//SetPosition(owner->GetPosition());
	
	if (GetOwner() != nullptr)
	{
		SetSize({ 128, 128 });
		//if (GetRotation() == 0)
			SetPosition(GetOwner()->GetPosition());
		//else if (GetRotation() == SGD::PI)
		//	SetPosition(GetOwner()->GetPosition() - GetSize());
		//else if (GetRotation() == SGD::PI / 2)
		//	SetPosition({ GetOwner()->GetPosition().x - GetSize().width, GetOwner()->GetPosition().y });
		//else if (GetRotation() == (3*SGD::PI)/2)
		//	SetPosition({ GetOwner()->GetPosition().x, GetOwner()->GetPosition().y - GetSize().height });
	}
}

SmokeBomb::~SmokeBomb()
{
	if (m_owner != nullptr)
	{
		Player* plr = (Player*)m_owner;
		plr->SetSafeZone(false);
	}
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_Sound);
	//SetEmiter(nullptr);
	SetOwner(nullptr);
}

/*virtual*/ void	SmokeBomb::Update(float elapsedTime)		/*override;*/
{
	if (GetOwner() != nullptr)
	{
		m_lifeSpan -= elapsedTime;
		if (m_lifeSpan <= 0)
		{
			DeleteEmitterMessage* emi = new DeleteEmitterMessage(this);
			emi->QueueMessage();
			emi = nullptr;

			Player* plr = (Player*)GetOwner();
			plr->SetSafeZone(false);
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
		else
		{
			Player* plr = (Player*)GetOwner();
			if (GetRect().IsIntersecting(plr->GetRect()))
				plr->SetSafeZone(true);
			else
				plr->SetSafeZone(false);
		}
		
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_Sound))
			SGD::AudioManager::GetInstance()->PlayAudio(m_Sound);

	}
}

/*virtual*/ void	SmokeBomb::Render(void)					/*override;*/
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 255, 255 }, {}, 1);
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), GetRotation(), {}, {}, { 0.05f, 0.05f });
}

/*virtual*/ void	SmokeBomb::HandleCollision(const IEntity* pOther)	/*override;*/
{
		if (pOther->GetType() == ENT_PLAYER)
		{
			if (GetOwner() == pOther)
			{
				Player* myplr = (Player*)pOther;
				myplr->SetSafeZone(true);

				SGD::Event* smoke = new SGD::Event{ "TargetDeleted", myplr, this };
				smoke->QueueEvent(nullptr);
				smoke = nullptr;

				SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

				myEvent->QueueEvent(nullptr);
				myEvent = nullptr;
			}
			else
			{
				DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

				pMsg->QueueMessage();
				pMsg = nullptr;

				Player* player = (Player*)pOther;
				player->PlayPickupSound();
				if (player->GetNumSmokeBombs() + 5 > 10)
					player->SetNumSmokeBombs(10);
				else
					player->SetNumSmokeBombs(player->GetNumSmokeBombs() + 5);
			}
		}
		else if (pOther->GetType() == ENT_PIZZA)
		{
			if (GetOwner() != nullptr)
			{
				Pizza* myPi = (Pizza*)pOther;

				SGD::Event* smoke = new SGD::Event{ "TargetDeleted", myPi, this };
				smoke->QueueEvent(nullptr);
				smoke = nullptr;
			}
		}
		else if (pOther->GetType() == ENT_ZOMBIE || pOther->GetType() == ENT_ARMOREDZOMBIE)
		{
			Zombie* zmb = (Zombie*)pOther;
			SGD::Event* smoke = new SGD::Event{ "EnemyDeleted", zmb, this };
			smoke->QueueEvent(nullptr);
			smoke = nullptr;
		}

		else if (pOther->GetType() == ENT_NEUTRAL_ENEMY)
		{
			NeutralEnemy* neuE = (NeutralEnemy*)pOther;

			SGD::Event* smoke = new SGD::Event{ "EnemyDeleted", neuE, this };
			smoke->QueueEvent(nullptr);
			smoke = nullptr;
		}
}

/*virtual*/ void	SmokeBomb::HandleEvent(const SGD::Event* pEvent) /*override;*/
{
	if (pEvent->GetEventID() == "Pause_Sound")
	{
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_Sound))
			SGD::AudioManager::GetInstance()->StopAudio(m_Sound);
	}
}

SGD::Rectangle SmokeBomb::GetRect(void)	const
{
	return SGD::Rectangle{ m_ptPosition - m_szSize / 2, m_ptPosition + m_szSize/2 };
}