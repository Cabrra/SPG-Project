#include "PowerUp.h"


#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Game.h"
#include "DestroyEntityMessage.h"
#include "GamePlayState.h"
#include "CreateEmiterMessage.h"
#include "DeleteEmitterMessage.h"

#include "Player.h"

#define POWER_UP_LIFETIME	8.0f

PowerUp::PowerUp()
{
	SetPowerUpType(0);
	m_timer = POWER_UP_LIFETIME;
	SetSize({ 32, 32 });
	
}

PowerUp::PowerUp(int mytype, int level, SGD::Point myPos )
{
	SetSize({ 64, 64 });
	SetPowerUpType(mytype);
	SetPowerUpLevel(level);
	SetPosition(myPos);

	m_timer = POWER_UP_LIFETIME;
	switch (mytype)
	{
	case(PU_HEALTH):
	{
					   m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Powerups/ramen.png");
					   break;
	}
	case(PU_MONEY):
	{
					  m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Powerups/money_PowerUp.png");
					  break;
	}
	case(PU_SPEED):
	{
 					  m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Powerups/olive.png");
					  break;
	}
	case(PU_STRENGTH):
	{
						 m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Powerups/meatball.png");
						 break;
	}
	case(PU_TIME):
	{
					 m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Powerups/sausage.png");
					 break;
	}
	}
}

PowerUp::~PowerUp()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}


/*virtual*/ void	PowerUp::Update(float elapsedTime)		/*override;*/
{
	m_timer -= elapsedTime;
	if (m_collisionTimer > 0)
		m_collisionTimer -= elapsedTime;
	if (m_timer < 0)
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

/*virtual*/ void	PowerUp::Render(void)					/*override;*/
{
	float posx = GetPosition().x - ((GetSize().width * 0.5f) / 2) + Game::GetInstance()->GetWorldSpace().x;
	float posy = GetPosition().y - ((GetSize().height * 0.5f) / 2) + Game::GetInstance()->GetWorldSpace().y;

	//SGD::Rectangle rect = GetRect();
	//rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 255}, {});
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { posx, posy }, GetRotation(), GetSize() / 2, {}, { 1.0f, 1.0f });
}

/*virtual*/ void	PowerUp::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_PLAYER && m_collisionTimer < 0)
	{
		GamePlayState::GetInstance()->CreatMessage(GamePlayState::GetInstance()->_ItemBox);
		Player* mypla = (Player*)pOther;
		switch (m_type)
		{
		case(PU_HEALTH) :
		{
							mypla->SetCurrHP(mypla->GetCurrHP() + 10 * m_level);
							mypla->PlayHealth();
							break;
		}
		case(PU_MONEY) :
		{
						   mypla->SetDAQ(mypla->GetDAQ() + 25 * m_level);
						   mypla->PlayMoney();
						   break;
		}
		case(PU_SPEED) :
		{
						   mypla->SetMoveSpeed(mypla->GetMoveSpeed() + 50 * m_level);
						   mypla->SetPrevSpeed(mypla->GetPrevSpeed() + 50 * m_level);

						   mypla->SetPowerUpTimer(10.0f);
						   mypla->AddActivePower(1);
						   mypla->PlaySpeed();
						   //create emitter and unload previous 1
						   DeleteEmitterMessage* delemi = new DeleteEmitterMessage((Entity*)pOther);
						   delemi->QueueMessage();
						   delemi = nullptr;

						   CreateEmiterMessage* speedUp = new CreateEmiterMessage((Entity*)pOther, 1);

						   speedUp->QueueMessage();
						   speedUp = nullptr;
						   break;
		}
		case(PU_STRENGTH) :
		{
							  mypla->SetStrength(mypla->GetStrength() + 10 * m_level);
							  mypla->SetPowerUpTimer(10.0f);
							  mypla->AddActivePower(0);
							  mypla->PlayStrength();
							  //create emitter and unload previous 2
							  DeleteEmitterMessage* delemi = new DeleteEmitterMessage((Entity*)pOther);
							  delemi->QueueMessage();
							  delemi = nullptr;

							  CreateEmiterMessage* strengthUp = new CreateEmiterMessage((Entity*)pOther, 2);

							  strengthUp->QueueMessage();
							  strengthUp = nullptr;
							  break;
		}
		case(PU_TIME) :
		{
						  mypla->SetTimer(mypla->GetTimer() + 10 * m_level);
						  mypla->PlayTime();
						  break;
		}
		}


		//destroy this
		DestroyEntityMessage* pMsg = new DestroyEntityMessage(this);

		pMsg->QueueMessage();
		pMsg = nullptr;
	}

}

/*virtual*/ SGD::Rectangle	PowerUp::GetRect(void)		const	/*override;*/
{
	SGD::Rectangle rect{ SGD::Point{ GetPosition().x - (GetSize().width *0.5f)/ 2, GetPosition().y - (GetSize().height * 0.5f) / 2 }, GetSize() * 0.5f };
	return rect;
}