#include "Hook.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "DestroyEntityMessage.h"

#include "Game.h"

#include "MovableBox.h"
#include "ImmovableBox.h"
#include "Player.h"

Hook::Hook(SGD::Point position, float rotation, Entity* owner) : Bullet(position,rotation,owner)
{
	if (owner != nullptr)
	{
		Player* player = dynamic_cast<Player*>(owner);
		SetSpeed(250 * player->GetHookLevel());
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/grapplingHook.png");
		m_Rope = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/SkinnyRope.png");
		m_HookSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/hooksound.wav");
		SetSize({ 16, 16 });
	}
	else
		SetSize({ 32, 32 });
	
	SGD::Vector newVel = { 0, 1 };
	newVel.Rotate(rotation);
	newVel *= GetSpeed();
	SetVelocity(newVel);

	SetPosition({ position.x - GetSize().width / 2, position.y - GetSize().height / 2 });
}


Hook::~Hook()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Rope);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_HookSound);
}


/*virtual*/ void	Hook::Update(float elapsedTime)		/*override;*/
{
	if (GetOwner() != nullptr)
	{
		Player* player = (Player*)GetOwner();
		if ((GetOwner()->GetPosition() - GetPosition()).ComputeLength() > 100 * player->GetHookLevel())
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
		Entity::Update(elapsedTime);
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_HookSound))
			SGD::AudioManager::GetInstance()->PlayAudio(m_HookSound);
	}
}

/*virtual*/ void	Hook::Render(void)					/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	if (GetOwner() != nullptr)
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_Rope, GetOwner()->GetPositionInWorldSpace(), { 0, 0, (GetOwner()->GetPosition() - GetPosition()).ComputeLength(), 8 }, GetRotation()+SGD::PI/2, {}, {}, {1.0f,0.5f});
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, {}, {}, 3);
	if (GetOwner() != nullptr)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), GetRotation(), GetSize()/2, {}, { 1.0f, 1.0f });
	else
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { GetPositionInWorldSpace().x - 32,GetPositionInWorldSpace().y- 25 }, GetRotation(), {}, {}, { 0.3f, 0.3f });
}

/*virtual*/ void	Hook::HandleCollision(const IEntity* pOther)	/*override;*/
{
	float rotate = 0;
	if (pOther->GetType() == ENT_MOVABLE)
	{
		if (pOther != m_Box)
		{
			if (GetRotation() == SGD::PI)
				rotate = 0;
			else if (GetRotation() == 0)
				rotate = SGD::PI;
			else if (GetRotation() == (3 * SGD::PI) / 2)
				rotate = SGD::PI / 2;
			else if (GetRotation() == SGD::PI / 2)
				rotate = (3 * SGD::PI) / 2;

			SGD::Vector newVel = { 0, 1 };
			newVel.Rotate(rotate);
			newVel *= GetSpeed();
			SetVelocity(newVel);
			MovableBox* moveBox = (MovableBox*)(pOther);
			m_Box = moveBox;
		}
	}
	else if (pOther->GetType() == ENT_PLAYER)
	{
		if (m_Box != nullptr || GetOwner() == nullptr)
		{
			Player* player = (Player*)pOther;
			if (!player->HaveHook())
			{
				player->PlayPickupSound();
				player->SetHaveHook(true);
			}

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
	else if (pOther->GetType() == ENT_IMMOVABLE)
	{
		if (pOther != m_Box)
		{
			SetVelocity({ 0, 0 });
			ImmovableBox* moveBox = (ImmovableBox*)(pOther);
			m_Box = moveBox;
		}
	}
	else 	if (pOther->GetType() == ENT_PIZZA)
	{
		if (pOther != m_Box)
		{
			if (GetRotation() == SGD::PI)
				rotate = 0;
			else if (GetRotation() == 0)
				rotate = SGD::PI;
			else if (GetRotation() == (3 * SGD::PI) / 2)
				rotate = SGD::PI / 2;
			else if (GetRotation() == SGD::PI / 2)
				rotate = (3 * SGD::PI) / 2;

			SGD::Vector newVel = { 0, 1 };
			newVel.Rotate(rotate);
			newVel *= GetSpeed();
			SetVelocity(newVel);
			MovableBox* moveBox = (MovableBox*)(pOther);
			m_Box = moveBox;
		}
	}
}