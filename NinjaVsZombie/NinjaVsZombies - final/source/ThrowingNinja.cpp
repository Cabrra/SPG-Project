#include "ThrowingNinja.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "DestroyEntityMessage.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "CreatePowerUpMessage.h"
#include "CreateShurikenMessage.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GamePlayState.h"

#include <assert.h>
#define SHOT_DELAY 1.5f

ThrowingNinja::ThrowingNinja() : Listener(this)
{
	/*Listener::*/RegisterForEvent("Target");
	Listener::RegisterForEvent("EnemyDeleted");
	Listener::RegisterForEvent("CAUTION_STATE");

	SetSize({ 64, 64 });
	SetPosition({ 200, 400 });
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/throwingninja.png");
	SGD::Point way = {  Game::GetInstance()->GetWorldCamera().left / 2 - 2000, Game::GetInstance()->GetWorldCamera().top / 2 - 2000};
	m_Waypoints.push_back(way);
	way = { Game::GetInstance()->GetWorldCamera().left / 2, Game::GetInstance()->GetWorldCamera().top / 2 };
	m_Waypoints.push_back(way);
	way = {Game::GetInstance()->GetWorldCamera().left / 2 + 2000, Game::GetInstance()->GetWorldCamera().top / 2 + 2000};
	m_Waypoints.push_back(way);
	m_hit = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/NinjaHurt.wav");
	m_attack = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/shuriken.wav");
	m_death = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Neck Breaking.wav");

}


ThrowingNinja::~ThrowingNinja()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	m_Waypoints.clear();

	SGD::AudioManager::GetInstance()->UnloadAudio(m_hit);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_attack);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_death);



	SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

	myEvent->QueueEvent(nullptr);
	myEvent = nullptr;

	SetTarget(nullptr);
}

/*virtual*/ void			ThrowingNinja::Update(float elapsedTime)		/*override*/
{
	if (GetMoveSpeed() != 0)
		SetMoveSpeed(0);


	if (m_currentwaypoint >= m_Waypoints.size() || m_currentwaypoint < 0)
		m_currentwaypoint = 0;


	if (GetTarget() != nullptr && GetTarget()->GetType() == ENT_PLAYER)
	{
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());

		SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();

		if (targetLoc.ComputeLength() > 400)
		{
			SetTarget(nullptr);

			SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

			myEvent->QueueEvent(nullptr);
			myEvent = nullptr;
		}

		float angle = orientation.ComputeAngle(targetLoc);

		float maxangle = SGD::PI / 2 * elapsedTime;

		if (angle > maxangle)
			angle = maxangle;

		if (orientation.ComputeSteering(targetLoc) >= 0)
			SetRotation(GetRotation() + angle);
		else
			SetRotation(GetRotation() - angle);
		shooting += elapsedTime;
		if (shooting > SHOT_DELAY)
		{
			shooting = 0;
			float plusormin = (float)(rand() % 2);
			float halfcircle = (float)(rand() % 90 + 1);
			halfcircle *= CONVERTTORAD;
			if (plusormin == 1)
			{
				SetRotation(GetRotation() - halfcircle);
			}
			else
			{
				SetRotation(GetRotation() + halfcircle);
			}
			SGD::AudioManager::GetInstance()->PlayAudio(m_attack);
			CreateShurikenMessage* pMsg = new CreateShurikenMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}

	}
	else if (GetTarget() == nullptr)
	{

		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());

		SGD::Vector direction = m_Waypoints[m_currentwaypoint] - GetPosition();

		direction.Normalize();

		if (direction.ComputeDotProduct(orientation) > 0.98)
		{
			m_currentwaypoint++;
		}
		if (direction.ComputeSteering(orientation) >= 0)
			SetRotation(GetRotation() - SGD::PI*elapsedTime / 10);
		else
			SetRotation(GetRotation() + SGD::PI*elapsedTime / 10);

		/*SGD::Vector orientation = { Game::GetInstance()->GetScreenWidth() / 2, Game::GetInstance()->GetScreenHeight() / 2 };
		orientation.Rotate(GetRotation());
		if (testspin)
		{
		SetRotation((-.02f) + GetRotation());
		}
		else
		{
		SetRotation((.02f) + GetRotation());
		}
		if (GetRotation() > SGD::PI && testspin == false)
		{
		testspin = true;
		}
		else if (GetRotation() < 0 && testspin == true)
		{
		testspin = false;
		}*/
	}

	Entity::Update(elapsedTime);

}
/*virtual*/ void			ThrowingNinja::Render(void)					/*override*/
{
	/*SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 255, 0 }, {}, 0);*/
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, { 0.5f, 0.5f });
}
/*virtual*/ void			ThrowingNinja::HandleCollision(const IEntity* pOther)	/*override*/
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
			CreateDrop();

		}
	}
	if (pOther->GetType() == ENT_SMOKEBOMB)
	{
		SetTarget(nullptr);
	}
}
/*virtual*/ void			ThrowingNinja::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "Target")
	{
		Entity* newTarget = (Entity*)pEvent->GetSender();
		if (GetTarget() == nullptr)
		{
			SGD::Vector distance = newTarget->GetPosition() - GetPosition();
			if (distance.ComputeLength() < 200)
			{
				distance.Normalize();
				SGD::Vector orientation = { 0, -1 };
				orientation.Rotate(GetRotation());
				if (distance.ComputeDotProduct(orientation)> 0.80)
				{
					if (newTarget->GetType() != ENT_PIZZA)
					{
						if (newTarget->GetType() == ENT_PLAYER)
						{
							Player* plr = (Player*)pEvent->GetSender();
							if (plr->GetSafeZone() == false)
							{
								SetTarget(newTarget);
								m_LastKnownPoint = newTarget->GetPosition();

								SGD::Event* myEvent = new SGD::Event{ "PLAYER_SPOTTED", &m_LastKnownPoint, this };
								myEvent->QueueEvent();
								myEvent = nullptr;
							}
						}
						else
						{
							SetTarget(newTarget);
						}
					}
				}
			}
		}
	}
	else if (pEvent->GetEventID() == "EnemyDeleted")
	{
		Entity* sender = (Entity*)pEvent->GetSender();

		if (sender != nullptr && sender->GetType() == ENT_SMOKEBOMB)
		{
			Entity* tar = (Entity*)pEvent->GetData();
			if (tar == GetTarget() && tar->GetType() == ENT_PLAYER)
			{
				SetTarget(nullptr);

				SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

				myEvent->QueueEvent(nullptr);
				myEvent = nullptr;
			}
			else if(tar == GetTarget())
			{
				SetTarget(nullptr);
			}
		}
		else if (sender == GetTarget())
		{
			SetTarget(nullptr);
		}
	}
	else if (pEvent->GetEventID() == "CAUTION_STATE")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender != this)
		{
			SGD::Point* mypoint = reinterpret_cast<SGD::Point*> (pEvent->GetData());
			assert(mypoint != nullptr && "Throwing Ninja - failed reading CAUTION_STATE event last known point");

			SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

			myEvent->QueueEvent(nullptr);
			myEvent = nullptr;

			m_LastKnownPoint = *mypoint;
			SetTarget(nullptr);

		}
	}
}

void ThrowingNinja::CreateDrop()
{
	//create drop
	int isloot = rand() % 100; 
	if (isloot <= 10 )
	{
		int loottype = rand() % 100 + 1;

		//fix level

		if (loottype <= 10) //Health
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(4, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 70 && loottype <= 100) //Money
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(3, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 45 && loottype <= 70) //Time
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(2, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 30 && loottype <= 45) //smokebomb
		{
			GamePlayState::GetInstance()->SpawnSmokeBomb(GetPosition());
		}
		else if (loottype > 20 && loottype <= 30) //Strength
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(0, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 10 && loottype <= 20) //speed
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(1, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
	}

	SGD::Event* mydeath = new SGD::Event{ "EnemyDeleted", "", this };

	mydeath->SendEventNow(nullptr);
	delete mydeath;
	mydeath = nullptr;
	DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

	pMsg->QueueMessage();
	pMsg = nullptr;
}
