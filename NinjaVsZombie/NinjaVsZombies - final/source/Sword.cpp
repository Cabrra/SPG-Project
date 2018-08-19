#include "Sword.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "Player.h"

#include "Lever.h"

#include "DestroyEntityMessage.h"
#include "GluttonyGreed.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "Game.h"

Sword::Sword(SGD::Point position, float rotation, Entity* owner)
{
	SetOwner(owner);
	SetRotation(rotation);
	m_startRot = rotation;
	SetPosition(position);
	
	SetSize({ 32, 32 });
	if (owner != nullptr && owner->GetType() == ENT_PLAYER)
	{
		Player* player = (Player*)owner;
		SetDamage(player->GetStrength() + (10 * player->GetSwordLevel()));
	}

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/katana2.png");
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
		if (GetOwner()->GetRotation() == 0)
			SetPosition({ GetOwner()->GetPosition().x - GetSize().width / 2, GetOwner()->GetPosition().y + GetSize().height / 2 });
		else if (GetOwner()->GetRotation() == SGD::PI)
			SetPosition({ GetOwner()->GetPosition().x - GetSize().width / 2, GetOwner()->GetPosition().y - GetSize().height*1.5f });
		else if (GetOwner()->GetRotation() == SGD::PI / 2)
			SetPosition({ GetOwner()->GetPosition().x - GetSize().width*1.5f, GetOwner()->GetPosition().y - GetSize().height/2 });
		else if (GetOwner()->GetRotation() == (3*SGD::PI)/2)
			SetPosition({ GetOwner()->GetPosition().x+ GetSize().width/2, GetOwner()->GetPosition().y - GetSize().height/2 });
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
	rect.Offset({ Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y });

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 0);

	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	if (GetOwner() != nullptr)
	{
		SGD::Point point = GetOwner()->GetPositionInWorldSpace();
		if (GetOwner()->GetRotation() == 0)
			SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), { point.x, point.y + GetSize().height / 2 }, GetRotation(), {}, {}, { 0.3f, 0.5f });
		else if (GetOwner()->GetRotation() == SGD::PI)
			SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), { point.x, point.y - GetSize().height / 2 }, GetRotation(), {}, {}, { 0.3f, 0.5f });
		else if (GetOwner()->GetRotation() == SGD::PI / 2)				 
			SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), { point.x-GetSize().width/2, point.y }, GetRotation(), {}, {}, { 0.3f, 0.5f });
		else if (GetOwner()->GetRotation() == (3 * SGD::PI) / 2)		 
			SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), { point.x + GetSize().width / 2, point.y }, GetRotation(), {}, {}, { 0.3f, 0.5f });
	}
	else
		SGD::GraphicsManager::GetInstance()->DrawTexture(GetImage(), { GetPositionInWorldSpace().x - 32, GetPositionInWorldSpace().y + 32 }, GetRotation(), GetSize() / 2, {}, { 0.5f, 0.5f });

	std::vector<SGD::OStringStream> dam;
	dam.resize(m_attackedEnemies.size());
	for (size_t i = 0; i < m_attackedEnemies.size(); i++)
	{
		dam[i] << "-" << m_attackedEnemies[i].DamTaken;
		SGD::GraphicsManager::GetInstance()->DrawRectangle({ m_attackedEnemies[i].pos.x, m_attackedEnemies[i].pos.y - 64, m_attackedEnemies[i].pos.x + 40, m_attackedEnemies[i].pos.y - 32 }, {}, {}, 1);
		pFont->Draw(dam[i].str().c_str(), { m_attackedEnemies[i].pos.x, m_attackedEnemies[i].pos.y - 32 }, 0.5f, { 255, 0, 0 });
	}
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
				GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_SwordBox);
				player->PlayPickupSound();
				player->SetHaveSword(true);
			}

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
	else if (pOther->GetType() == ENT_ZOMBIE || pOther->GetType() == ENT_NINJA || pOther->GetType() == ENT_NEUTRAL_ENEMY)
	{
		if (GetOwner() != nullptr)
		{
			Player* player = (Player*)GetOwner();
			m_attacked = (Unit*)pOther;
			AttackedEnemy enemy;
			bool att = false;
			for (size_t i = 0; i < m_attackedEnemies.size(); i++)
			{
				if (m_attacked == m_attackedEnemies[i].unit)
					att = true;
			}
		
			if (att == false)
			{
				enemy.unit = m_attacked;
				enemy.pos = m_attacked->GetPositionInWorldSpace() + m_attacked->GetSize();
				enemy.DamTaken = GetDamage();
				m_attackedEnemies.push_back(enemy);
				
				m_attacked->SetCurrHP(m_attacked->GetCurrHP() - GetDamage());
			}
		}
	}
	else if (pOther->GetType() == ENT_BOSS)
	{

		Player* player = (Player*)GetOwner();
		GluttonyGreed* boss = (GluttonyGreed*)pOther;
		if (boss->GetMyState() == Enemy::AI_IDLE)
		{
			if (GetOwner() != nullptr)
			{
				boss->SetCurrHP(boss->GetCurrHP() - GetDamage() * 3);
				boss->SetMyState(Enemy::AI_PATROL);
				boss->SetCurrentPoint(0);
			}
			else
				SetDamage(0);
		}
	}
	else if (pOther->GetType() == ENT_AMBUSHNINJA)
	{
		
		if ( GetOwner() != nullptr)
		{
			Player* player = (Player*)GetOwner();
			m_attacked = (Unit*)pOther;
			AttackedEnemy enemy;
			bool att = false;
			for (size_t i = 0; i < m_attackedEnemies.size(); i++)
			{
				if (m_attacked == m_attackedEnemies[i].unit)
					att = true;
			}

			if (att == false)
			{
				enemy.unit = m_attacked;
				enemy.pos = m_attacked->GetPositionInWorldSpace() + m_attacked->GetSize();
				enemy.DamTaken = GetDamage() * 2;
				m_attackedEnemies.push_back(enemy);

				m_attacked->SetCurrHP(m_attacked->GetCurrHP() - GetDamage());
			}
		}
	}
	else if (pOther->GetType() == ENT_LEVER)
	{
		Lever* lever = (Lever*)pOther;

		if (m_Lever != lever)
		{
			if (lever->GetActive() == true)
			{
				lever->TurnOff();
				lever->PlayOffSound();

			}
			else
			{
				lever->PlayOnSound();
				lever->TurnOn();

			}


			SGD::Event* activ = new SGD::Event("PUZZLE_OBJECT_ACTIVATED", nullptr, lever);
			activ->QueueEvent();
			activ = nullptr;

			m_Lever = lever;
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