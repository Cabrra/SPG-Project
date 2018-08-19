#include "Hook.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "DestroyEntityMessage.h"

#include "Game.h"
#include "GamePlayState.h"
#include "LevelManager.h"
#include "ImmovableBox.h"
#include "MovableBox.h"
#include "Zombie.h"
#include "Player.h"
#include "Lever.h"
#include "Room.h"

Hook::Hook(SGD::Point position, float rotation, Entity* owner) : Bullet(position, rotation, owner)
{
	if (owner != nullptr)
	{
		Player* player = dynamic_cast<Player*>(owner);
		SetSpeed(250 * (float)player->GetHookLevel());
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/grapplingHook.png");
		m_Rope = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/SkinnyRope.png");
		SetSize({ 16, 16 });
		m_Box = nullptr;
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
}

/*virtual*/ void	Hook::Update(float elapsedTime)		/*override;*/
{
	SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
	SGD::Point point = GetRect().ComputeCenter() + GetVelocity()*elapsedTime;
	if (GetOwner() != nullptr)
	{
		Player* player = (Player*)GetOwner();
		if (m_Box != nullptr && m_Box->GetType() != ENT_ZOMBIE)
			m_lifeTimer -= elapsedTime;

		if (((GetOwner()->GetPosition() - GetPosition()).ComputeLength() > 150 * player->GetHookLevel()) || m_lifeTimer <= 0 ||
			CheckCollision(GetPosition() + GetVelocity()*elapsedTime) || !rect.IsPointInRectangle(point))
		{
			if (m_Box != nullptr && m_Box->GetType() == ENT_IMMOVABLE)
			{
				ImmovableBox* box = (ImmovableBox*)m_Box;
				box->SetHook(nullptr);
			}

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}


		Entity::Update(elapsedTime);
	}
}

/*virtual*/ void	Hook::Render(void)					/*override;*/
{
	if (GetOwner() != nullptr)
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_Rope, GetOwner()->GetPositionInWorldSpace(),
		{ 0, 0, (GetOwner()->GetPosition() - GetPosition()).ComputeLength(), 8 }, GetRotation() + SGD::PI / 2, {}, {}, { 1.0f, 0.5f });

	if (GetOwner() != nullptr)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), GetRotation(), GetSize() / 2, {}, { 1.0f, 1.0f });
	else
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { GetPositionInWorldSpace().x - 32, GetPositionInWorldSpace().y - 25 }, GetRotation(), {}, {}, { 0.3f, 0.3f });
}

/*virtual*/ void	Hook::HandleCollision(const IEntity* pOther)	/*override;*/
{
	float rotate = 0;
	if (pOther->GetType() == ENT_MOVABLE)
	{
		if (m_Box == nullptr)
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
			m_Box = (IEntity*)pOther;
		}
	}
	else if (pOther->GetType() == ENT_PLAYER)
	{
		if (m_Box != nullptr || GetOwner() == nullptr)
		{
			Player* player = (Player*)pOther;
			if (!player->HaveHook())
			{
				GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_HookBox);
				player->PlayPickupSound();
				player->SetHaveHook(true);
			}
			else if (player->IsBeingPulled())
				player->SetBeingPulled(false);

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
			m_Box = (IEntity*)pOther;
		}
	}
	else 	if (pOther->GetType() == ENT_PIZZA)
	{
		if (pOther != m_Box)
		{
			m_Box = nullptr;

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
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

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
			m_Lever = lever;
		}
	}
	else if (pOther->GetType() == ENT_ZOMBIE)
	{
		if (m_Box == nullptr)
		{
			Zombie* zomb = (Zombie*)(pOther);
			if (zomb->IsArmored())
			{
				zomb->SetHasWon(false);
				zomb->SetmovedPlayer(false);
				SetVelocity({ 0, 0 });
			}
			else
			{
				DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			m_Box = zomb;
		}
	}
}

bool Hook::CheckCollision(SGD::Point point)
{
	std::vector<Room*>	rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
	for (size_t i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->IsPlayerHere() && rooms[i]->CheckCollision(point, this))
		{
			return true;
		}
	}
	return false;
}