#include "Pizza.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "GamePlayState.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "AmbushNinja.h"
#include "Player.h"
#include "Hook.h"
#include "Game.h"

#include "DestroyEntityMessage.h"

Pizza::Pizza() : SGD::Listener(this)
{
	Listener::RegisterForEvent("ENEMY_TAKE_PIZZA");
	
	SetSize({ 32, 32 });
	SetScale({0.03f,0.06f});

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/pizzaimage.jpg");
}

Pizza::~Pizza()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/*virtual*/ void	Pizza::Update(float elapsedTime)		/*override;*/
{
	if (m_IsAlive)
	{
		SGD::Event* Target = new SGD::Event{ "Target", nullptr, this };
		Target->QueueEvent();
		Target = nullptr;

		if (IsFalling())
		{
			m_Scale.width -= elapsedTime;
			m_Scale.height -= elapsedTime;
			if (m_Scale.width <= 0 && m_Scale.height <= 0)
			{
				m_IsAlive = false;

				SGD::Event* Target = new SGD::Event{ "TargetDeleted", nullptr, this };
				Target->QueueEvent();

				m_IsAlive = false;

				DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

				pMsg->QueueMessage();
				pMsg = nullptr;
			}
		}
	}
}

/*virtual*/ void	Pizza::Render(void)					/*override;*/
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 1);

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, GetPositionInWorldSpace(), 0.0f, {}, {}, GetScale());
}

/*virtual*/ void	Pizza::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (m_IsAlive)
	{
		if (pOther->GetType() == ENT_HOOK)
		{
			const Hook* hook = dynamic_cast<const Hook*>(pOther);
			Player* player = (Player*)(hook->GetOwner());

			player->PlayPickupSound();
			player->SetHavePizza(true);
			GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_PizzaBox);

			SGD::Event* Target = new SGD::Event{ "TargetDeleted", nullptr, this };
			Target->QueueEvent();

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
		else if (pOther->GetType() == ENT_PLAYER)
		{
			Player* player = (Player*)pOther;

			if (player->IsBeingPulled() == false)
			{
				GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_PizzaBox);
				player->PlayPickupSound();
				player->SetHavePizza(true);

				SGD::Event* Target = new SGD::Event{ "TargetDeleted", nullptr, this };
				Target->QueueEvent();

				DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

				pMsg->QueueMessage();
				pMsg = nullptr;
			}
		}
	}
}

/*virtual*/ void	Pizza::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "ENEMY_TAKE_PIZZA")
	{
		SGD::Event* Target = new SGD::Event{ "TargetDeleted", nullptr, this };
		Target->QueueEvent();
		
		m_IsAlive = false;

		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

bool Pizza::CheckCollision(SGD::Point point)
{
	std::vector<Room*>	rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
	bool TileCollision = false;
	for (size_t i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->IsPlayerHere() && rooms[i]->CheckCollision(point, this))
			return true;
	}
	if (GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_MOVABLE)
		|| GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_IMMOVABLE)
		|| (GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_ENEMY)))
		return true;
	return false;
}