#include "GluttonyGreed.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "DestroyEntityMessage.h"
#include "CreatePowerUpMessage.h"
#include "Game.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "Player.h"

#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>
#include "GameOverMessage.h"
#include "CreatePizzaMessage.h"
#include "AnimationManager.h"

GluttonyGreed::GluttonyGreed() : Listener(this)
{
	Listener::RegisterForEvent("GAME_OVER");
	//Listener::RegisterForEvent("ALERT_STATE");
	Listener::RegisterForEvent("TargetDeleted");
	Listener::RegisterForEvent("Target");

	//m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/GluttonyGreed.png");
	SetSize({ 32, 32 });
	SetMoveSpeed(65);
	speed = 65;
	pizzaCount = 0;
	SetMaxHP(300);
	SetCurrHP(300);
}


GluttonyGreed::~GluttonyGreed()
{
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::Event* take = new SGD::Event{ "DEFEATED", nullptr, this };
	take->QueueEvent(nullptr);
	take = nullptr;

	m_Waypoints.clear();
	SetTarget(nullptr);
	SetPizza(nullptr);
}

/*virtual*/ void GluttonyGreed::Update(float elapsedTime)		/*override;*/
{
	if (GetMoveSpeed() == 0)
		SetMoveSpeed(speed);
	if (m_HitTimer > 0)
		m_HitTimer -= elapsedTime;
	switch (m_myState)
	{
	case(AI_PATROL) : //move around
	{
						  UpdatePatrol(elapsedTime);
						  break;
	}
	case(AI_ATTACK) : //attack with a rush 
	{
						  UpdateAttack(elapsedTime);
						  break;
	}
					  //case(AI_ALERT) : //heal itself or run
					  //{
					  //					 UpdateAlert(elapsedTime);
					  //					 break;
					  //}
	case(AI_IDLE) : //cooldowm after colliding with the room edge or something
	{
						UpdateIdle(elapsedTime);
						SetMoveSpeed(0);
						break;
	}
	}
}

/*virtual*/ void GluttonyGreed::Render(void)					/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	const BitmapFont* pFont = Game::GetInstance()->GetFont();
	if (m_myState == AI_IDLE)
	{


		SGD::Rectangle myRect = SGD::Rectangle(
			(Game::GetInstance()->GetWorldCamera().right - Game::GetInstance()->GetWorldCamera().left) / 2 - 40,
			(Game::GetInstance()->GetWorldCamera().bottom - Game::GetInstance()->GetWorldCamera().top) / 2 - 10,
			(Game::GetInstance()->GetWorldCamera().right - Game::GetInstance()->GetWorldCamera().left) / 2 + 40,
			(Game::GetInstance()->GetWorldCamera().bottom - Game::GetInstance()->GetWorldCamera().top) / 2 + 10);

		HINSTANCE HInstance = Game::GetInstance()->GetHInstance();

		char buf[255];
		LoadString(HInstance, IDS_GLUTTONY, buf, 255);

		pFont->Draw(buf, { myRect.left - 205, myRect.top - 205 }, 0.8f, { 0, 0, 0 });
		pFont->Draw(buf, { myRect.left - 200, myRect.top - 200 }, 0.8f, { 255, 255, 255 });

	}

	if (tired)
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 85), m_Tired, left, m_Scale);
	}
	else
	{
		AnimationManager::GetInstance()->Render((int)GetPosition().x + 30, (int)GetPosition().y - (int)(GetSize().height / 2 - 85), m_Run, left, m_Scale);
	}
	/*SGD::Rectangle rec = GetRect();
	rec.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);
	
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rec, { 255, 0, 0 }, {}, 4);*/
	SGD::OStringStream output;
	output << pizzaCount;
	unsigned char g;
	g = 255 - (pizzaCount * 36);
	pFont->Draw(output.str().c_str(), { GetPosition().x + GetSize().width, GetPosition().y + GetSize().height }, 0.8f, { 255, g, g });
}

/*virtual*/ void GluttonyGreed::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_SWORD)
	{
		if (GetCurrHP() <= 0)
		{
			// create drop

			//create drop
			for (int i = 0; i < 5; i++)
			{
				int isloot = rand() % 100 + 1;
				if (isloot <= 45)
				{
					int loottype = rand() % 100 + 1;

					//fix level

					if (loottype <= 40) //Health
					{
						CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(4, { GetPosition().x + (i * 5), GetPosition().y + (i * 10) }, 5);

						newMsg->QueueMessage();
						newMsg = nullptr;
					}
					if (loottype > 40 && loottype <= 80) //Money
					{
						CreatePowerUpMessage* newMsg = new CreatePowerUpMessage(3, { GetPosition().x + (i * 5), GetPosition().y + (i * 10) }, 5);

						newMsg->QueueMessage();
						newMsg = nullptr;
					}
					if (loottype > 80 && loottype <= 100) //smokebomb
					{
						GamePlayState::GetInstance()->SpawnSmokeBomb(this->GetPosition());
					}
				}
			}
			//returns 80% of stolen money


			Player* myPlay = (Player*)GetTarget();
			myPlay->SetDAQ(myPlay->GetDAQ() + (int)(m_moneyCount * 0.8));

			DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

			pMsg->QueueMessage();
			pMsg = nullptr;

		}
		else if ((GetCurrHP() / GetMaxHP()) <= 0.75)
		{
			SetMoveSpeed(100);
			speed = 100;
		}
		else if ((GetCurrHP() / GetMaxHP()) <= 0.5)
		{
			SetMoveSpeed(150);
			speed = 150;
		}
		else if ((GetCurrHP() / GetMaxHP()) <= 0.25)
		{
			SetMoveSpeed(200);
			speed = 200;
			//m_myState = AI_ALERT;
		}
	}
	if (pOther->GetType() == ENT_NEUTRAL_ENEMY)
	{
		// heal eating the enemy and destroy it

		SetCurrHP(GetCurrHP() + (int)(GetMaxHP() * 0.25));
		Entity* myEnt = (Entity*)pOther;
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(myEnt);

		pMsg->QueueMessage();
		pMsg = nullptr;

		m_myState = AI_PATROL;
	}

	if (pOther->GetType() == ENT_PLAYER)
	{
		if (m_HitTimer < 0)
		{
			if ((GetCurrHP() / GetMaxHP()) >= 0.75f) // steal money
			{
				Player* myPlay = (Player*)GetTarget();
				int money = (int)(myPlay->GetDAQ() * 0.1);
				m_moneyCount += money;
				myPlay->SetDAQ(myPlay->GetDAQ() - money);
			}
			else if ((GetCurrHP() / GetMaxHP()) >= 0.50f) // steal more money
			{
				Player* myPlay = (Player*)GetTarget();
				myPlay->SetDAQ(myPlay->GetDAQ() - (int)(myPlay->GetDAQ() * 0.25));
				if (myPlay->GetHavePizza() == true)
					pizzaCount++;
			}
			else if ((GetCurrHP() / GetMaxHP()) < 0.50f) // steal more money, pizza and health
			{
				Player* myPlay = (Player*)GetTarget();
				myPlay->SetDAQ(myPlay->GetDAQ() - (int)(myPlay->GetDAQ() * 0.5));

				myPlay->SetHavePizza(false);

				myPlay->SetCurrHP(myPlay->GetCurrHP() - 15);

				float width = Game::GetInstance()->GetWorldCamera().right - Game::GetInstance()->GetWorldCamera().left;
				float height = Game::GetInstance()->GetWorldCamera().bottom - Game::GetInstance()->GetWorldCamera().top;

				if (GetPosition().x <= width / 2 && GetPosition().y <= height / 2) // first cuadrant
				{
					CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ (width / 2) + (width / 4) + 20, (height / 2) + (height / 4) });
					creat->QueueMessage();
					creat = nullptr;
				}
				else if (GetPosition().x > width / 2 && GetPosition().y <= height / 2) // second cuadrant
				{
					CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ (width / 2) + (width / 4), height / 4 });
					creat->QueueMessage();
					creat = nullptr;
				}
				else if (GetPosition().x <= width / 2 && GetPosition().y > height / 2) // third cuadrant
				{
					CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ width / 4,  (height / 2) + (height / 4) -20});
					creat->QueueMessage();
					creat = nullptr;
				}
				else if (GetPosition().x > width / 2 && GetPosition().y > height / 2) // fourth cuadrant
				{
					CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ (width / 2) + (width / 4), (height / 2) + (height / 4) -20});
					creat->QueueMessage();
					creat = nullptr;
				}
				m_myState = AI_PATROL;
			}
			m_HitTimer = 3.0f;
		}
	}
	if (pOther->GetType() == ENT_PIZZA)
	{
		pizzaCount++;
		m_myState = AI_PATROL;
		//destroy pizza
		if (m_Pizza != nullptr)
		{
			DestroyEntityMessage* pMs = new DestroyEntityMessage(m_Pizza);
			SetPizza(nullptr);
			pMs->QueueMessage();
			pMs = nullptr;
		}
		//create pizza
		float width = Game::GetInstance()->GetWorldCamera().right - Game::GetInstance()->GetWorldCamera().left;
		float height = Game::GetInstance()->GetWorldCamera().bottom - Game::GetInstance()->GetWorldCamera().top;

		if (GetPosition().x <= width / 2 && GetPosition().y <= height / 2) // first cuadrant
		{
			CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ (width / 2) + (width / 4) + 20, (height / 2) + (height / 4) });
			creat->QueueMessage();
			creat = nullptr;
		}
		else if (GetPosition().x > width / 2 && GetPosition().y <= height / 2) // second cuadrant
		{
			CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ (width / 2) + (width / 4), height / 4 });
			creat->QueueMessage();
			creat = nullptr;
		}
		else if (GetPosition().x <= width / 2 && GetPosition().y > height / 2) // third cuadrant
		{
			CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ width / 4 , (height / 2) + (height / 4) - 20 });
			creat->QueueMessage();
			creat = nullptr;
		}
		else if (GetPosition().x > width / 2 && GetPosition().y > height / 2) // fourth cuadrant
		{
			CreatePizzaMessage* creat = new CreatePizzaMessage(SGD::Point{ (width / 2) + (width / 4), (height / 2) + (height / 4) - 20 });
			creat->QueueMessage();
			creat = nullptr;
		}
		m_myState = AI_PATROL;

		if (pizzaCount == 7)
		{
			//gameover
			GameOverMessage* over = new GameOverMessage(this);
			over->SendMessageNow();
			delete over;
			over = nullptr;

		}
		m_HitTimer = 7.0f;
	}
}

void GluttonyGreed::SetPizza(Entity* newPizza)
{
	if (m_Pizza != nullptr)
	{
		m_Pizza->Release();
		m_Pizza = nullptr;
	}
	m_Pizza = newPizza;

	if (m_Pizza != nullptr)
		m_Pizza->AddRef();
}


void GluttonyGreed::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "Target")
	{
		Entity* newTarget = (Entity*)pEvent->GetSender();
		if (newTarget->GetType() == ENT_PLAYER)
		{
			SetTarget(newTarget); //boss always knows where  player is
		}
		else if (newTarget->GetType() == ENT_PIZZA)
		{
			SetPizza(newTarget); // boss always knows where  pizza is
		}
		else if (newTarget->GetType() == ENT_NEUTRAL_ENEMY)
		{
			Entity* newTarget = (Entity*)pEvent->GetSender();
			m_myCurrentway = newTarget->GetPosition(); //heal
		}
	}

	if (pEvent->GetEventID() == "TargetDeleted" && m_myState != AI_ATTACK)
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		if (sender->GetType() == ENT_PIZZA)
		{
			SetPizza(nullptr);

			m_myState = AI_ATTACK;
			//m_myCurrentway = { Game::GetInstance()->GetWorldCamera().left + 100, Game::GetInstance()->GetWorldCamera().top +200 };
		}
	}
}

void GluttonyGreed::UpdatePatrol(float elapsedTime)
{
	if (m_currPoint >= m_Waypoints.size())
	{
		m_currPoint = 0;
		m_myState = AI_ATTACK;
	}
	if (m_Waypoints.size() > 0)
	{
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());

		SGD::Vector direction = m_Waypoints[m_currPoint] - GetPosition();
		if (direction.ComputeLength() < 64)
			m_currPoint++;

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
		if (Unit::CheckBossCollision(GetPosition() + GetVelocity()*elapsedTime))
			//is going to collide
			Entity::Update(elapsedTime);
		else
		{
			m_currPoint++;
			SetMoveSpeed(0);

			if (GetPosition().x + GetSize().width / 2 >= Game::GetInstance()->GetWorldCamera().right - 100)
				SetPosition({ GetPosition().x - 30, GetPosition().y });
			else if (GetPosition().x - GetSize().width / 2 <= Game::GetInstance()->GetWorldCamera().left + 32)
				SetPosition({ GetPosition().x + 30, GetPosition().y });
			if (GetPosition().y + GetSize().height / 2 >= Game::GetInstance()->GetWorldCamera().bottom - 112)
				SetPosition({ GetPosition().x, GetPosition().y - 30 });
			else if (GetPosition().y - GetSize().height / 2 <= Game::GetInstance()->GetWorldCamera().top + 32)
				SetPosition({ GetPosition().x, GetPosition().y + 30 });

			m_setPath = true;
		}
	}
}

void GluttonyGreed::UpdateAttack(float elapsedTime)
{
	//rush to player in straight line and stop when hit the end

	if ((GetCurrHP() / GetMaxHP()) > 0.25)
	{
		AnimationManager::GetInstance()->Update(elapsedTime, m_Run);
		tired = false;
		run = true;
		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(GetRotation());

		if (m_RushTimer < 0)
		{
			SetMoveSpeed(200);
			if (m_setPath == true) //lock point
			{
				m_myCurrentway = GetTarget()->GetPosition();
				m_setPath = false;
				SGD::Vector distance = m_myCurrentway - GetPosition();

				distance.Normalize();

				if (distance.ComputeDotProduct(orientation) < 0.999)
				{
					if (distance.ComputeSteering(orientation) >= 0)
						SetRotation(GetRotation() - SGD::PI*elapsedTime);
					else
						SetRotation(GetRotation() + SGD::PI*elapsedTime);

					if (GetRotation() >= SGD::PI && GetRotation() < SGD::PI * 2)
					{
						left = true;
					}
					else
					{
						left = false;
					}
				}
				orientation *= (float)GetMoveSpeed();
				SetVelocity(orientation);
			}
			if (Unit::CheckBossCollision(GetPosition() + GetVelocity()*elapsedTime))
				Entity::Update(elapsedTime);
		}
		else
		{

			SGD::Vector distance = GetTarget()->GetPosition() - GetPosition();

			distance.Normalize();

			if (distance.ComputeDotProduct(orientation) < 0.999)
			{
				if (distance.ComputeSteering(orientation) >= 0)
					SetRotation(GetRotation() - SGD::PI*elapsedTime);
				else
					SetRotation(GetRotation() + SGD::PI*elapsedTime);

				if (GetRotation() >= SGD::PI / 2 && GetRotation() < (SGD::PI * 3) / 2)
				{
					left = false;
				}
				else
				{
					left = true;
				}
			}
			m_RushTimer -= elapsedTime;
		}
	}
	else //if health less than 25%
	{
		//go for pizza
		if (m_Pizza != nullptr)
		{
			SGD::Vector orientation = { 0, -1 };
			orientation.Rotate(GetRotation());

			if (m_setPath == true)
			{
				m_myCurrentway = m_Pizza->GetPosition(); //lock position
				m_setPath = false;
			}

			SGD::Vector distance = m_myCurrentway - GetPosition();

			if (distance.ComputeLength() < 60)
			{
				m_myState = AI_IDLE;
				m_RushTimer = 3.0f;
				pizzaCount++;

			}
			distance.Normalize();

			if (distance.ComputeDotProduct(orientation) < 0.999)
			{
				if (distance.ComputeSteering(orientation) >= 0)
					SetRotation(GetRotation() - SGD::PI*elapsedTime);
				else
					SetRotation(GetRotation() + SGD::PI*elapsedTime);

				if (GetRotation() >= SGD::PI / 2 && GetRotation() < (SGD::PI * 3) / 2)
				{
					left = false;
				}
				else
				{
					left = true;
				}
			}

			orientation *= (float)GetMoveSpeed();
			SetVelocity(orientation);
			if (Unit::CheckBossCollision(GetPosition() + GetVelocity()*elapsedTime) == true)
				Entity::Update(elapsedTime);
			else
			{

				if (GetPosition().x + GetSize().width / 2 >= Game::GetInstance()->GetWorldCamera().right - 100)
					SetPosition({ GetPosition().x - 30, GetPosition().y });
				else if (GetPosition().x - GetSize().width / 2 <= Game::GetInstance()->GetWorldCamera().left + 32)
					SetPosition({ GetPosition().x + 30, GetPosition().y });
				if (GetPosition().y + GetSize().height / 2 >= Game::GetInstance()->GetWorldCamera().bottom - 112)
					SetPosition({ GetPosition().x, GetPosition().y - 30 });
				else if (GetPosition().y - GetSize().height / 2 <= Game::GetInstance()->GetWorldCamera().top + 43)
					SetPosition({ GetPosition().x, GetPosition().y + 30 });

			}
			m_RushTimer = 3.0f;

		}
		else //go for player
		{
			SGD::Vector orientation = { 0, -1 };
			orientation.Rotate(GetRotation());

			if (m_RushTimer < 0)
			{
				SetMoveSpeed(200);

				if (m_setPath == true) //lock point
				{
					m_myCurrentway = GetTarget()->GetPosition();
					m_setPath = false;
					SGD::Vector distance = m_myCurrentway - GetPosition();

					distance.Normalize();

					if (distance.ComputeDotProduct(orientation) < 0.999)
					{
						if (distance.ComputeSteering(orientation) >= 0)
							SetRotation(GetRotation() - SGD::PI*elapsedTime);
						else
							SetRotation(GetRotation() + SGD::PI*elapsedTime);

						if (GetRotation() >= SGD::PI / 2 && GetRotation() < (SGD::PI * 3) / 2)
						{
							left = false;
						}
						else
						{
							left = true;
						}
					}

					orientation *= (float)GetMoveSpeed();

					SetVelocity(orientation);
				}
				if (Unit::CheckBossCollision(GetPosition() + GetVelocity()*elapsedTime))
					Entity::Update(elapsedTime);
				else
				{
					SetMoveSpeed(0);

					if (GetPosition().x + GetSize().width / 2 >= Game::GetInstance()->GetWorldCamera().right - 100)
						SetPosition({ GetPosition().x - 30, GetPosition().y });
					else if (GetPosition().x - GetSize().width / 2 <= Game::GetInstance()->GetWorldCamera().left + 32)
						SetPosition({ GetPosition().x + 30, GetPosition().y });
					if (GetPosition().y + GetSize().height / 2 >= Game::GetInstance()->GetWorldCamera().bottom - 112)
						SetPosition({ GetPosition().x, GetPosition().y - 30 });
					else if (GetPosition().y - GetSize().height / 2 <= Game::GetInstance()->GetWorldCamera().top + 32)
						SetPosition({ GetPosition().x, GetPosition().y + 30 });

				}
			}
			else
			{
				SGD::Vector distance = GetTarget()->GetPosition() - GetPosition();

				distance.Normalize();

				if (distance.ComputeDotProduct(orientation) < 0.999)
				{
					if (distance.ComputeSteering(orientation) >= 0)
						SetRotation(GetRotation() - SGD::PI*elapsedTime);
					else
						SetRotation(GetRotation() + SGD::PI*elapsedTime);

					if (GetRotation() >= SGD::PI / 2 && GetRotation() < (SGD::PI * 3) / 2)
					{
						left = false;
					}
					else
					{
						left = true;
					}
				}
				m_RushTimer -= elapsedTime;
				Entity::Update(elapsedTime);
			}
		}
	}

}


void GluttonyGreed::UpdateIdle(float elapsedTime)
{
	AnimationManager::GetInstance()->Update(elapsedTime, m_Tired);
	tired = true;
	run = false;
	if (m_IdleTimer < 0)
	{
		m_myState = AI_PATROL;
		m_IdleTimer = 3.0f;
		m_RushTimer = 3.0f;
	}
	else
		m_IdleTimer -= elapsedTime;
}
