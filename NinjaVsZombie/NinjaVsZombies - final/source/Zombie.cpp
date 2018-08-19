#include "Zombie.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "DestroyEntityMessage.h"
#include "CreatePowerUpMessage.h"
#include "CreatePizzaMessage.h"
#include "GameOverMessage.h"

#include "Game.h"
#include "GamePlayState.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "Room.h"
#include "BitmapFont.h"

#include "Player.h"
#include "Hook.h"

#include <assert.h>
#include "Player.h"
#include "Tile.h"
#include "AnimationManager.h"

#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>

Zombie::Zombie(bool armor) : Listener(this)
{
	Listener::RegisterForEvent("GAME_OVER");
	//Listener::RegisterForEvent("ALERT_STATE");
	Listener::RegisterForEvent("CAUTION_STATE");
	Listener::RegisterForEvent("PLAYER_SPOTTED");
	Listener::RegisterForEvent("TargetDeleted");
	Listener::RegisterForEvent("Target");
	Listener::RegisterForEvent("fight_back");
	Listener::RegisterForEvent("EnemyDeleted");


	SetSize({ 32, 32 });
	SetScale({ 0.5f, 0.5f });
	SetPosition({ 400, 300 });
	SetIsArmored(armor);

	if (IsArmored())
	{
		SetMoveSpeed(50);
		SetSize({ 64, 64 });
		SetMaxHP(150);
		SetCurrHP(150);
		m_Armoredpic = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/ArmoredZombie.png");
	}
	else
	{
		SetMoveSpeed(75);
		SetCurrHP(40);
	}

	//m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/Zombie_01.png");
	m_hit = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Zombie In Pain.wav");
	m_death = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Zombie Long Death.wav");
	m_attack = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Bite.wav");
	m_alarmed = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Zombiesmoan.wav");
	playonce = false;
}

Zombie::~Zombie()
{
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Armoredpic);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hit);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_death);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_attack);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_alarmed);


	m_Waypoints.clear();

	SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

	myEvent->QueueEvent(nullptr);
	myEvent = nullptr;

	Enemy::~Enemy();
}

/*virtual*/ void Zombie::Update(float elapsedTime)		/*override;*/
{
	if (GetRotation() < SGD::PI / 4 && GetRotation() >= SGD::PI * 7 / 4)
	{
		left = false;
		right = true;
		up = false;
		down = false;
	}
	else if (GetRotation() >= SGD::PI / 4 && GetRotation() < SGD::PI * 3 / 4)
	{
		left = false;
		right = false;
		up = true;
		down = false;
	}
	else if (GetRotation() >= SGD::PI * 3 / 4 && GetRotation() < SGD::PI * 5 / 4)
	{
		left = true;
		right = false;
		up = false;
		down = false;
	}
	else
	{
		left = false;
		right = false;
		up = false;
		down = true;
	}

	if (GetMoveSpeed() == 0)
	{
		if (IsArmored())
			SetMoveSpeed(50);
		else
			SetMoveSpeed(75);
	}
	if (!IsFalling() && !m_IsHooked && m_stunTimer < 0)
	{
		switch (m_myState)
		{
		case(AI_IDLE) :
		{
						  if (!fighting)
						  {
							  UpdateIdle(elapsedTime);
						  }
						  break;
		}
		case(AI_ATTACK) :
		{
							UpdateAttack(elapsedTime);
							break;
		}
		case(AI_ALERT) :
		{
						   if (!fighting)
						   {
							   UpdateAlert(elapsedTime);
						   }
						   break;
		}
		case(AI_CAUTION) :
		{
							 if (!fighting)
							 {
								 UpdateCaution(elapsedTime);
							 }
							 break;
		}
		case(AI_PATROL) :
		{
							if (!fighting)
							{
								UpdatePatrol(elapsedTime);
							}
							break;
		}
		case(AI_ESCAPE) :
		{
							if (!fighting)
							{
								UpdateEscape(elapsedTime);
							}
		}
		}
	}
	else if (IsFalling())
	{
		SetScale({ GetScale().width - elapsedTime, GetScale().height - elapsedTime });
		if (GetScale().width <= 0.0f && GetScale().height <= 0.0f)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
	m_stunTimer -= elapsedTime;
	if (fighting)
		m_fcountinc += elapsedTime;
}

/*virtual*/ void Zombie::Render(void)					/*override;*/
{
	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();
	char buf[255];

	if (IsArmored())
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_Armoredpic, { GetPositionInWorldSpace() }, GetRotation(), GetSize(), {}, GetScale());
	}
	else
	{
		if (attacking && down)
		{
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, attackdown, false);
		}											
		if (attacking && left)						
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, attackleft, false);
		}										
		if (attacking && up)					
		{										
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, attackup, false);
		}											
		if (attacking && right)						
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, attackleft, true);
		}										
												
		if (walking && down)					
		{										
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, walkdown, false);
		}											
		if (walking && left)						
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, walkleft, false);
		}											
		if (walking && up)							
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, walkup, false);
		}											
		if (walking && right)						
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, walkleft, true);
		}										
												
		if (stealing && down)					
		{										
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, stealdown, false);
		}											
		if (stealing && left)						
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, stealleft, false);
		}											
		if (stealing && up)							
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, stealup, false);
		}											
		if (stealing && right)						
		{											
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, stealleft, true);
		}														

		if (dying)												
		{														
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y, death, true);
		}
	}
	if (showfightbar)
	{
		SGD::Rectangle myRect = SGD::Rectangle(
			GetPositionInWorldSpace().x - 5,
			GetPositionInWorldSpace().y - 5,
			GetPositionInWorldSpace().x,
			GetPositionInWorldSpace().y);

		SGD::OStringStream ostring;
		if (m_IsHooked)
			LoadString(HInstance, IDS_TUGOFWAR, buf, 255);
		else
			LoadString(HInstance, IDS_FIGHTFORIT, buf, 255);

		const BitmapFont* pFont = Game::GetInstance()->GetFont();
		pFont->Draw(buf, { myRect.left - 2, myRect.top - 30 }, 0.4f, {});
		pFont->Draw(buf, { myRect.left, myRect.top - 28 }, 0.4f, { 0, 0, 0 });


		SGD::GraphicsManager::GetInstance()->DrawRectangle(
		{ myRect.left, myRect.top, myRect.left + 200, myRect.bottom + 10 },
		{ 0, 0, 0, 0 }, SGD::Color{ 255, 20, 147 },
		5);
		SGD::GraphicsManager::GetInstance()->DrawRectangle(
		{ myRect.left, myRect.top, myRect.left + ((m_countTobeat * 200) / 15), myRect.bottom + 10 },
		{ 255, 0, 255, 0 }, { 0, 0, 90, 175 },
		0);
	}
}

/*virtual*/ void Zombie::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_SWORD)
	{
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hit);
		}
	}
	else if (pOther->GetType() == ENT_HOOK)
	{
		Hook* hook = (Hook*)pOther;
		if (!IsArmored()&& hook->HookedItem() == this)
		{
			if (GamePlayState::GetInstance()->GetPlayer()->GetHookLevel() >= 3)
				SetCurrHP(GetCurrHP() - 5 * GamePlayState::GetInstance()->GetPlayer()->GetHookLevel());
			else
				m_stunTimer = 0.5f * (float)GamePlayState::GetInstance()->GetPlayer()->GetHookLevel();
		}
		else if (IsArmored() && hook->HookedItem() == this)
		{
			Hook* hook = (Hook*)pOther;
			SetTarget(hook->GetOwner());

			if (!m_Won)
			{
				SetMoveSpeed(0);
				showfightbar = true;
				fighting = true;
				SGD::Event* playerturn = new SGD::Event{ "Player_Fight" };
				playerturn->SendEventNow(nullptr);
				delete playerturn;
				m_IsHooked = true;
			}
			if (m_fcountinc > 1 && !m_Won)
			{
				m_fcountinc = 0;
				if (IsArmored())
					m_countTobeat++;
				m_countTobeat++;
				if (m_countTobeat < 0)
				{
					m_countTobeat = 10;
					SGD::Event* lost = new SGD::Event{ "fight_over", nullptr, this };
					lost->SendEventNow(nullptr);
					delete lost;

					showfightbar = false;

					DestroyEntityMessage* pMsg = new DestroyEntityMessage(hook);

					pMsg->QueueMessage();
					pMsg = nullptr;

					if (GetHavePizza())
					{
						hook->GetOwner()->SetHavePizza(true);
						SetHavePizza(false);
					}
					else
					{
						SetMoveSpeed(75);
						SetIsArmored(false);
						SetCurrHP(40);
					}

					m_IsHooked = false;
					fighting = false;

					m_myState = AI_ALERT;
				}
				else if (m_countTobeat >= 15)
				{
					m_Won = true;
				}
			}
			else if (m_Won && !m_movedPlayer)
			{
				m_movedPlayer = true;

				fighting = false;

				SGD::Event* pEvent = new SGD::Event{ "MovePlayer", nullptr, this };
				pEvent->QueueEvent();
				pEvent = nullptr;

				SGD::Event* lost = new SGD::Event{ "fight_over", nullptr, this };
				lost->SendEventNow(nullptr);
				delete lost;

				showfightbar = false;
				m_countTobeat = 10;
				m_myState = AI_ALERT;

				SetMoveSpeed(50);
			}
			if ((hook->GetOwner()->GetRect().ComputeCenter() - hook->GetRect().ComputeCenter()).ComputeLength() < 30)
			{
				DestroyEntityMessage* pMsg = new DestroyEntityMessage(hook);

				pMsg->QueueMessage();
				pMsg = nullptr;

				Player* ply = (Player*)hook->GetOwner();
				ply->SetBeingPulled(false);

				m_Won = false;
				fighting = false;
				m_IsHooked = false;
				showfightbar = false;
				m_movedPlayer = false;

				SetMoveSpeed(50);
			}
		}
	}

	if (GetCurrHP() <= 0 && !GetHavePizza() && IsArmored() == false)
		CreateDrop();
	else if (GetCurrHP() <= 0 && GetHavePizza() && IsArmored() == false)
	{
		CreatePizzaMessage* pizzaMsg = new CreatePizzaMessage(GetPosition());
		pizzaMsg->QueueMessage();
		pizzaMsg = nullptr;
		SetHavePizza(false);

		SGD::Event* mydeath = new SGD::Event{ "EnemyDeleted", "", this };

		mydeath->SendEventNow(nullptr);
		delete mydeath;
		mydeath = nullptr;

		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;

	}
	else if (IsArmored() && GetCurrHP() <= 100)
	{
		SetIsArmored(false);
		SetMoveSpeed(75);
	}
	else if (pOther->GetType() == ENT_SMOKEBOMB)
	{
		m_myState = AI_IDLE;
		m_StateTimer = 1.5f;
		SetTarget(nullptr);

		SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

		myEvent->QueueEvent(nullptr);
		myEvent = nullptr;

	}
}

void Zombie::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "Target")
	{
		Entity* newTarget = (Entity*)pEvent->GetSender();
		if (GetTarget() == nullptr && GetHavePizza() == false)
		{
			SGD::Vector distance = newTarget->GetPosition() - GetPosition();
			if (distance.ComputeLength() < 200)
			{
				distance.Normalize();
				SGD::Vector orientation = { 0, -1 };
				orientation.Rotate(GetRotation());
				if (distance.ComputeDotProduct(orientation)> 0.80)
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

							m_myState = AI_ALERT;
						}
					}
					else if (newTarget->GetType() == ENT_PIZZA || (newTarget->GetType() == ENT_NEUTRAL_ENEMY))
					{
						SetTarget(newTarget);
						m_myState = AI_ALERT;;
					}
				}
			}
		}
		else if (GetHavePizza() == true)
		{
			m_myState = AI_ESCAPE;
			int a = rand() % 50;
			if (a % 2 == 0)
				m_EscapePoint = { Game::GetInstance()->GetWorldCamera().left + 32, Game::GetInstance()->GetWorldCamera().top + 32 };
			else
				m_EscapePoint = { Game::GetInstance()->GetWorldCamera().right - 32, Game::GetInstance()->GetWorldCamera().bottom - 32 };
		}
		else if (GetTarget()->GetType() == ENT_PLAYER && newTarget->GetType() == ENT_PIZZA)
		{
			SGD::Vector distance = newTarget->GetPosition() - GetPosition();
			if (distance.ComputeLength() < 200)
			{
				distance.Normalize();
				SGD::Vector orientation = { 0, -1 };
				orientation.Rotate(GetRotation());
				if (distance.ComputeDotProduct(orientation)> 0.80)
				{
					SetTarget(newTarget);
				}
				m_myState = AI_ALERT;
			}
		}
	}
	else if (pEvent->GetEventID() == "TargetDeleted")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender->GetType() == ENT_PIZZA && GetHavePizza() == false)
		{
			if (GetTarget() == sender)
			{
				SetTarget(nullptr);

				m_myState = AI_PATROL;
				m_StateTimer = 7.0f;
			}
		}
		if (sender->GetType() == ENT_SMOKEBOMB && m_myState != AI_PATROL)
		{
			Entity* thething = (Entity*)pEvent->GetData();
			if (GetTarget() == thething)
			{
				SetTarget(nullptr);

				m_myState = AI_PATROL;
				m_StateTimer = 7.0f;
			}
		}
	}
	else if (pEvent->GetEventID() == "CAUTION_STATE" && m_myState == AI_CAUTION)
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender != this)
		{
			SGD::Point* mypoint = reinterpret_cast<SGD::Point*> (pEvent->GetData());
			assert(mypoint != nullptr && "Zombie - failed reading CAUTION_STATE event last known point");


			m_LastKnownPoint = *mypoint;

			m_myState = AI_CAUTION;
			m_StateTimer = 7.0f;
			SetTarget(nullptr);

		}
	}
	else if (pEvent->GetEventID() == "fight_back")
	{
		m_countTobeat--;
	}
	else if (pEvent->GetEventID() == "PLAYER_SPOTTED" && GetTarget() == nullptr && (m_myState != AI_ALERT && m_myState != AI_ATTACK)) // tell where the other zombies where did he watch the player
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender != this)
		{
			SGD::Point* mypoint = reinterpret_cast<SGD::Point*> (pEvent->GetData());
			assert(mypoint != nullptr && "Zombie - failed reading PLAYER_SPOTTED event last known point");

			m_LastKnownPoint = *mypoint;

			//m_myState = AI_CAUTION;
			m_StateTimer = 7.0f;
			SetTarget(nullptr);

			m_myState = AI_CAUTION;
		}
	}
	else if (pEvent->GetEventID() == "EnemyDeleted")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender->GetType() == ENT_SMOKEBOMB)
		{
			Entity* tar = (Entity*)pEvent->GetData();
			if (tar == GetTarget())
			{
				SetTarget(nullptr);
				m_myState = AI_IDLE;
				m_StateTimer = 3.0f;
			}
		}
		else if (sender == GetTarget())
		{
			SetTarget(nullptr);
			m_myState = AI_PATROL;
		}
	}
	else if (pEvent->GetEventID() == "NeutralE_HIT")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		Entity* data = (Entity*)pEvent->GetData();
		if (sender != this && data != nullptr && data == this)
		{
			SetCurrHP(GetCurrHP() - 15);
			if (GetHavePizza() == true)
			{
				CreatePizzaMessage* pizzaMsg = new CreatePizzaMessage(GetPosition());
				pizzaMsg->QueueMessage();
				pizzaMsg = nullptr;
				SetHavePizza(false);
			}
			else
				CreateDrop();
		
			//DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);
			//pMsg->QueueMessage();
			//pMsg = nullptr;
		}
	}
}

void Zombie::UpdateIdle(float elapsedTime)
{
	m_StateTimer -= elapsedTime;
	if (m_StateTimer < 0)
	{
		m_myState = AI_PATROL;
		m_StateTimer = 3.0f;
	}
}

void Zombie::UpdateAttack(float elapsedTime)
{
	m_StateTimer -= elapsedTime;

	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	if (GetTarget()->GetType() == ENT_PLAYER && GetTarget()->GetHavePizza())
	{
		//SGD::Event* myEvent = new SGD::Event{ "PLAYER_SPOTTED", &m_LastKnownPoint, this };
		//myEvent->QueueEvent();
		//myEvent = nullptr;

		SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();
		if (targetLoc.ComputeLength() < 40)
		{
			SetMoveSpeed(0);
			showfightbar = true;
			fighting = true;
			SGD::Event* playerturn = new SGD::Event{ "Player_Fight" };
			playerturn->SendEventNow(nullptr);
			delete playerturn;
			if (m_fcountinc > 1)
			{
				m_fcountinc = 0;
				if (IsArmored())
					m_countTobeat++;
				m_countTobeat++;
				if (m_countTobeat < 0)
				{
					m_countTobeat = 10;
					SGD::Point point = { GetPosition().x + 100, GetPosition().y + 100 };
					while (!CheckCollision(point))
					{
						point = { point.x - 5, point.y - 5 };
					}
					SetPosition(point);
					SGD::Event* lost = new SGD::Event{ "fight_over", nullptr, this };
					lost->SendEventNow(nullptr);
					delete lost;
					showfightbar = false;
					fighting = false;
					m_myState = AI_IDLE;
					m_StateTimer = 0.9f;

				}
				else if (m_countTobeat >= 15)
				{
					//take pizza
					SetHavePizza(true);
					GetTarget()->SetHavePizza(false);
					SGD::Event* take = new SGD::Event{ "ENEMY_TAKE_PIZZA", nullptr, this };
					take->SendEventNow(nullptr);
					delete take;
					SGD::Event* lost = new SGD::Event{ "fight_over", nullptr, this };
					lost->SendEventNow(nullptr);
					delete lost;
					showfightbar = false;
					m_countTobeat = 10;
					SetTarget(nullptr);
					m_myState = AI_ESCAPE;
					int a = rand() % 50;
					if (a % 2 == 0)
						m_EscapePoint = { Game::GetInstance()->GetWorldCamera().left + 32, Game::GetInstance()->GetWorldCamera().top + 32 };
					else
						m_EscapePoint = { Game::GetInstance()->GetWorldCamera().right - 32, Game::GetInstance()->GetWorldCamera().bottom - 32 };
				}
			}

		}

		float angle = orientation.ComputeAngle(targetLoc);

		float maxangle = SGD::PI / 2 * elapsedTime;

		if (angle > maxangle)
			angle = maxangle;

		if (orientation.ComputeSteering(targetLoc) >= 0)
			SetRotation(GetRotation() + angle);
		else
			SetRotation(GetRotation() - angle);

		orientation *= (float)GetMoveSpeed();
		SetVelocity(orientation);
	}
	else if (GetTarget()->GetType() == ENT_PIZZA)
	{
		if (down)
		{
			AnimationManager::GetInstance()->Update(elapsedTime, stealdown);
		}
		else if (up)
		{
			AnimationManager::GetInstance()->Update(elapsedTime, stealup);
		}
		else 
		{
			AnimationManager::GetInstance()->Update(elapsedTime, stealleft);
		}
		if (m_StateTimer <= 0)
		{
			SetHavePizza(true);
			SGD::Event* take = new SGD::Event{ "ENEMY_TAKE_PIZZA", nullptr, this };
			take->SendEventNow(nullptr);
			delete take;
			m_myState = AI_ESCAPE;
			int a = rand() % 50;
			if (a % 2 == 0)
				m_EscapePoint = { Game::GetInstance()->GetWorldCamera().left + 32, Game::GetInstance()->GetWorldCamera().top + 32 };
			else
				m_EscapePoint = { Game::GetInstance()->GetWorldCamera().right - 32, Game::GetInstance()->GetWorldCamera().bottom - 32 };
		}
	}
	else if (GetTarget()->GetType() == ENT_PLAYER)
	{
		SGD::Event* myEvent = new SGD::Event{ "PLAYER_SPOTTED", &m_LastKnownPoint, this };
		myEvent->QueueEvent();
		myEvent = nullptr;

		SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();

		if (targetLoc.ComputeLength() < 50)
		{
			SetMoveSpeed(0);
			if (down)
			{
				AnimationManager::GetInstance()->Update(elapsedTime, attackdown);
			}														
			else if (up)											
			{														
				AnimationManager::GetInstance()->Update(elapsedTime, attackup);
			}														
			else													
			{														
				AnimationManager::GetInstance()->Update(elapsedTime, attackleft);
			}
			if (m_StateTimer <= 0)
			{
				m_StateTimer = 1.5;
				SGD::AudioManager::GetInstance()->PlayAudio(m_attack);

				SGD::Event* take = new SGD::Event{ "Player_HIT", nullptr, this };
				take->QueueEvent(nullptr);
				take = nullptr;
			}
		}
		else if (targetLoc.ComputeLength() > 300)
		{
			m_StateTimer = 5.0f;
			m_myState = AI_CAUTION;
			SetTarget(nullptr);
		}
		float angle = orientation.ComputeAngle(targetLoc);

		float maxangle = SGD::PI / 2 * elapsedTime;

		if (angle > maxangle)
			angle = maxangle;

		if (orientation.ComputeSteering(targetLoc) >= 0)
			SetRotation(GetRotation() + angle);
		else
			SetRotation(GetRotation() - angle);

		orientation *= (float)GetMoveSpeed();
		SetVelocity(orientation);
	}
	else if (GetTarget()->GetType() == ENT_NEUTRAL_ENEMY)
	{

		SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();

		if (targetLoc.ComputeLength() < 50)
		{
			SetMoveSpeed(0);

			if (m_StateTimer <= 0)
			{
				m_StateTimer = 1.5;

				SGD::Event* take = new SGD::Event{ "NeutralE_HIT", GetTarget(), this };
				take->QueueEvent(nullptr);
				take = nullptr;
			}
		}
		float angle = orientation.ComputeAngle(targetLoc);

		float maxangle = SGD::PI / 2 * elapsedTime;

		if (angle > maxangle)
			angle = maxangle;

		if (orientation.ComputeSteering(targetLoc) >= 0)
			SetRotation(GetRotation() + angle);
		else
			SetRotation(GetRotation() - angle);

		orientation *= (float)GetMoveSpeed();
		SetVelocity(orientation);
	}
	if (CheckCollision(GetPosition() + GetVelocity()*elapsedTime))
		Entity::Update(elapsedTime);
}

void Zombie::UpdateAlert(float elapsedTime)
{
	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	SGD::Vector targetLoc = GetTarget()->GetPosition() - GetPosition();

	if (GetTarget()->GetType() == ENT_PLAYER)
		m_LastKnownPoint = GetTarget()->GetPosition();

	if (targetLoc.ComputeLength() <= 300 && GetTarget()->GetType() == ENT_PLAYER)
	{
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_alarmed))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_alarmed);
			playonce = true;
		}
		SGD::Event* myEvent = new SGD::Event{ "PLAYER_SPOTTED", &m_LastKnownPoint, this };
		myEvent->QueueEvent();
		myEvent = nullptr;
	}
	if (targetLoc.ComputeLength() > 300)
	{
		m_StateTimer = 7.0f;
		m_myState = AI_CAUTION;

		SGD::Event* myEvent = new SGD::Event{ "CAUTION_STATE", &m_LastKnownPoint, this };

		myEvent->QueueEvent(nullptr);
		myEvent = nullptr;

		SetTarget(nullptr);
	}
	else if (targetLoc.ComputeLength() < 50)
	{
		SetMoveSpeed(0); //change state to attack
		m_myState = AI_ATTACK;
		m_StateTimer = 1.5f;
	}

	float angle = orientation.ComputeAngle(targetLoc);

	float maxangle = SGD::PI * 2 * elapsedTime;

	if (angle > maxangle)
		angle = maxangle;

	if (orientation.ComputeSteering(targetLoc) >= 0)
		SetRotation(GetRotation() + angle);
	else
		SetRotation(GetRotation() - angle);

	orientation *= (float)GetMoveSpeed();
	SetVelocity(orientation);

	SGD::Point point = GetPosition() + GetVelocity()*elapsedTime;
	//if (!CheckCollision(point))
	//{		
	//	while (!CheckCollision(point))
	//	{
	//		SetRotation(GetRotation() + SGD::PI / 2);

	//		orientation = { 0, -1 };
	//		orientation.Rotate(GetRotation());
	//		orientation *= (float)GetMoveSpeed();
	//		SetVelocity(orientation);
	//		point = GetPosition() + GetVelocity()*elapsedTime;
	//	}
	//}
	if (CheckCollision(point))
	{
		if (down)
		{
			AnimationManager::GetInstance()->Update(elapsedTime, walkdown);
		}
		else if (up)
		{
			AnimationManager::GetInstance()->Update(elapsedTime, walkup);
		}
		else
		{
			AnimationManager::GetInstance()->Update(elapsedTime, walkleft);
		}
		Entity::Update(elapsedTime);
	}
}

void Zombie::UpdateCaution(float elapsedTime)
{
	m_StateTimer -= elapsedTime;
	if (m_StateTimer <= 0)
	{
		m_myState = AI_PATROL;
		SetTarget(nullptr);

		SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

		myEvent->QueueEvent(nullptr);
		myEvent = nullptr;
		m_StateTimer = 7.0f;
	}
	else
	{
		SGD::Vector targetLoc = m_LastKnownPoint - GetPosition();
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());

		if (targetLoc.ComputeLength() < 32)
		{
			SetMoveSpeed(0); //look around

			float angler = SGD::PI / 2 * elapsedTime;

			SetRotation(GetRotation() + angler);

		}
		else
		{
			float angle = orientation.ComputeAngle(targetLoc);

			float maxangle = SGD::PI / 2 * elapsedTime;

			if (angle > maxangle)
				angle = maxangle;

			if (orientation.ComputeSteering(targetLoc) >= 0)
				SetRotation(GetRotation() + angle);
			else
				SetRotation(GetRotation() - angle);

			orientation *= (float)GetMoveSpeed();
			SetVelocity(orientation);
			if (CheckCollision(GetPosition() + GetVelocity()*elapsedTime))
			{
				if (down)
				{
					AnimationManager::GetInstance()->Update(elapsedTime, walkdown);
				}
				else if (up)
				{
					AnimationManager::GetInstance()->Update(elapsedTime, walkup);
				}
				else
				{
					AnimationManager::GetInstance()->Update(elapsedTime, walkleft);
				}
				Entity::Update(elapsedTime);
			}
		}
	}
}

void Zombie::UpdatePatrol(float elapsedTime)
{
	if (m_currPoint >= m_Waypoints.size() || m_currPoint < 0)
		m_currPoint = 0;

	
	if (m_Waypoints.size() > 0)
	{
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());
		if (GetTarget() == nullptr)
		{
			SGD::Vector direction = m_Waypoints[m_currPoint] - GetPosition();
			if (direction.ComputeLength() < 25)
				m_currPoint++;

			direction.Normalize();

			if (direction.ComputeDotProduct(orientation) < 0.999)
			{
				if (direction.ComputeSteering(orientation) >= 0)
					SetRotation(GetRotation() - SGD::PI*elapsedTime);
				else
					SetRotation(GetRotation() + SGD::PI*elapsedTime);
			}
		}
		orientation *= (float)GetMoveSpeed();
		SetVelocity(orientation);
		if (CheckCollision(GetPosition() + GetVelocity()*elapsedTime))
		{
			if (down)
			{
				AnimationManager::GetInstance()->Update(elapsedTime, walkdown);
			}
			else if (up)
			{
				AnimationManager::GetInstance()->Update(elapsedTime, walkup);
			}
			else
			{
				AnimationManager::GetInstance()->Update(elapsedTime, walkleft);
			}
			Entity::Update(elapsedTime);
		}
		else
		{

		}
	}
}

void Zombie::UpdateEscape(float elapsedTime)
{
	SGD::Vector orientation = { 0, -1 };
	orientation.Rotate(GetRotation());

	SGD::Vector waypoint = m_EscapePoint - GetPosition();

	if (waypoint.ComputeLength() < 13)
	{
		SetMoveSpeed(0);
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
	}
	float angle = orientation.ComputeAngle(waypoint);

	float maxangle = SGD::PI / 2 * elapsedTime;

	if (angle > maxangle)
		angle = maxangle;

	if (orientation.ComputeSteering(waypoint) >= 0)
		SetRotation(GetRotation() + angle);
	else
		SetRotation(GetRotation() - angle);

	orientation *= (float)GetMoveSpeed();
	SetVelocity(orientation);
	if (EscapeCollision(GetPosition() + GetVelocity()*elapsedTime))
	{
		if (down)
		{
			AnimationManager::GetInstance()->Update(elapsedTime, walkdown);
		}
		else if (up)
		{
			AnimationManager::GetInstance()->Update(elapsedTime, walkup);
		}
		else
		{
			AnimationManager::GetInstance()->Update(elapsedTime, walkleft);
		}
		SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
		//if (!rect.IsPointInRectangle(GetPosition() + GetVelocity()*elapsedTime) && GetHavePizza()== true)
		/*{
			GameOverMessage* pMsg = new GameOverMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}*/

		Entity::Update(elapsedTime);
	}
}

void Zombie::CreateDrop()
{
	//create drop
	int isloot = rand() % 100 + 1;
	if (isloot <= 25 && GetHavePizza() == false)
	{
		int loottype = rand() % 100 + 1;

		//fix level

		if (loottype <= 20) //Health
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(4, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 20 && loottype <= 50) //Money
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(3, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 50 && loottype <= 75) //Time
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(2, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 75 && loottype <= 86) //smokebomb
		{
			GamePlayState::GetInstance()->SpawnSmokeBomb(GetPosition());
		}
		else if (loottype > 80 && loottype <= 90) //Strength
		{
			CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(0, GetPosition(), 1);

			newMsg->QueueMessage();
			newMsg = nullptr;
		}
		else if (loottype > 90) //speed
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

bool Zombie::EscapeCollision(SGD::Point point)
{
	std::vector<Room*>	rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
	bool TileCollision = false;
	for (size_t i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->IsPlayerHere())
		{
			TileCollision = rooms[i]->CheckCollision(point, this);
			break;
		}
	}
	if (TileCollision == false
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_MOVABLE)
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_IMMOVABLE)
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_PLAYER))
		return true;
	return false;
}