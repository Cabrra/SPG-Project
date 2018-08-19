#include "AmbushNinja.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Message.h"

#include "../SGD Wrappers/SGD_Geometry.h"	
#include "Game.h"

#include "PowerUp.h"
#include "GamePlayState.h"
#include "CreatePowerUpMessage.h"
#include "CreatePizzaMessage.h"
#include "GameOverMessage.h"
#include "Player.h"
#include "SpawnPoint.h"

AmbushNinja::AmbushNinja() : SGD::Listener(this)
{
	Listener::RegisterForEvent("Target");
	Listener::RegisterForEvent("TargetDeleted");
	Listener::RegisterForEvent("PIZZA_FREE");

	SetSize({ 64, 64 });
	SetPosition({ 0, 0 });
	SetTarget(nullptr);

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Ninju.png");
	SGD::HAudio steal = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Danger.wav");
	SetStealSound(steal);
	m_hit = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/NinjaHurt.wav");
	m_death = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Neck Breaking.wav");
	

	GamePlayState::GetInstance()->SetAmbush(true);
}

AmbushNinja::~AmbushNinja()
{
	if (GetHavePizza() == true)
	{
		GameOverMessage* pMsg = new GameOverMessage(this);
		pMsg->SendMessageNow();
		delete pMsg;
		pMsg = nullptr;
	}
	unsigned int num = GetSpawnPoint()->GetSpawnedUnits() - 1;
	GetSpawnPoint()->SetSpawnedUnit(num);
	num = GetSpawnPoint()->GetSpawnedUnits();
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hit);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_death);

	SetTarget(nullptr);

	SGD::AudioManager::GetInstance()->UnloadAudio(GetStealSound());
	SetStealSound(SGD::INVALID_HANDLE);

	GamePlayState::GetInstance()->SetAmbush(false);	
}

/*virtual*/ void AmbushNinja::Update(float elapsedTime)		/*override;*/
{
	if (GetMoveSpeed() == 0)
		SetMoveSpeed(100);

	if (!IsFalling())
	{
		switch (m_myState)
		{
		case(AI_ESCAPE) :
		{
							UpdateEscape(elapsedTime);
							break;
		}
		case(AI_ATTACK) :
		{
							UpdateAttack(elapsedTime);
							break;
		}
		}
	}
	else
	{
		SetScale({ GetScale().width - elapsedTime, GetScale().height - elapsedTime });
		if (GetScale().width <= 0.0f && GetScale().height <= 0.0f)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}

	//gameover
	//if (Game::GetInstance()->GetWorldCamera().IsIntersecting(GetRect()) == false)
	//{
	//	DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

	//	pMsg->QueueMessage();
	//	pMsg = nullptr;
	//}
}

/*virtual*/ void AmbushNinja::Render(void)					/*override;*/
{
	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	//
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 100, 255 }, {}, 0);

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, { 0.5f, 0.5f });
}

/*virtual*/ void AmbushNinja::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_SWORD)
	{
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hit);
		}

		if (GetCurrHP() <= 0)
		{
			if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_death))
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_death);

			}
			//create drop
			int isloot = rand() % 100 + 1;
			if (GetHavePizza() == true)
			{
				CreatePizzaMessage* pizzaMsg = new CreatePizzaMessage(GetPosition());
				pizzaMsg->QueueMessage();
				pizzaMsg = nullptr;
				SetHavePizza(false);
			}
			else if (isloot <= 25)
			{
				//fix level
				CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(3, GetPosition(), 0.04f);

				newMsg->QueueMessage();
				newMsg = nullptr;
			}


			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}

void AmbushNinja::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "Target")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender->GetType() == ENT_PIZZA && GetTarget() == nullptr)
		{
			
			SetTarget(sender);
			m_myState = AI_ATTACK;
		}
	}
	else if (pEvent->GetEventID() == "TargetDeleted")
	{
		if (GetHavePizza() == false)
		{
			SetTarget(nullptr);
			m_myState = AI_ESCAPE;
			int a = rand() % 50;
			if (a % 2 == 0)
				m_currPoint = { Game::GetInstance()->GetWorldCamera().left - 20, Game::GetInstance()->GetWorldCamera().top - 20 };
			else
				m_currPoint = { Game::GetInstance()->GetWorldCamera().right + 20, Game::GetInstance()->GetWorldCamera().bottom + 20 };
		}
	}
}

void AmbushNinja::UpdateAttack(float elapsedTime)
{
	SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
	SGD::Point point = GetRect().ComputeCenter() + GetVelocity()*elapsedTime;
	if (GetTarget() != nullptr)
	{
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());

		SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();
		m_currPoint = GetTarget()->GetPosition();
		if (targetLoc.ComputeLength() < 32)
		{
			SetMoveSpeed(0);
			if (m_StateTimer > 0.0f)
			{
				m_StateTimer -= elapsedTime;
			}
			else
			{
				m_StateTimer = 3.0f;
				m_myState = AI_ESCAPE;

				SGD::AudioManager::GetInstance()->PlayAudio(GetStealSound());
				//take pizza
				SetHavePizza(true);
				SGD::Event* take = new SGD::Event{ "ENEMY_TAKE_PIZZA", nullptr, this };
				take->SendEventNow(nullptr);
				delete take;

				//escape point
				int a = rand() % 50;
				if (a% 2 == 0)
					m_currPoint = { Game::GetInstance()->GetWorldCamera().left - 20, Game::GetInstance()->GetWorldCamera().top -20 };
				else
					m_currPoint = { Game::GetInstance()->GetWorldCamera().right + 20, Game::GetInstance()->GetWorldCamera().bottom + 20  };

				SetTarget(nullptr);
			}
		}

		SGD::Vector direction = m_currPoint - GetPosition();

		direction.Normalize();

		if (direction.ComputeDotProduct(orientation) < 0.999)
		{
			SetMoveSpeed(0);
			if (direction.ComputeSteering(orientation) >= 0)
				SetRotation(GetRotation() - SGD::PI*elapsedTime);
			else
				SetRotation(GetRotation() + SGD::PI*elapsedTime);
		}
		orientation *= (float)GetMoveSpeed();
		SetVelocity(orientation);
		if (rect.IsPointInRectangle(point))
			Entity::Update(elapsedTime);
		else
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}

void AmbushNinja::UpdateEscape(float elapsedTime)
{
	SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
	SGD::Point point = GetRect().ComputeCenter() + GetVelocity()*elapsedTime;
	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	Entity* temp = GetTarget();

		SGD::Vector direction = m_currPoint - GetPosition();
		if (direction.ComputeLength() < 40)
		{
			SetMoveSpeed(0);
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
		
			direction.Normalize();

			if (direction.ComputeDotProduct(orientation) < 0.999)
			{
				if (direction.ComputeSteering(orientation) >= 0)
					SetRotation(GetRotation() - SGD::PI*elapsedTime);
				else
					SetRotation(GetRotation() + SGD::PI*elapsedTime);
			}
	
		orientation *= (float)GetMoveSpeed();
		SetVelocity(orientation);

		if (rect.IsPointInRectangle(point))
			Entity::Update(elapsedTime);
}