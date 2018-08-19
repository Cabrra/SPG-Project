#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Game.h"

#include "ImmovableBox.h"
#include "MovableBox.h"
#include "BitmapFont.h"
#include "../SGD Wrappers/SGD_String.h"

Player::Player() : Listener(this)
{
	Listener::RegisterForEvent("MovePlayer");

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/lilNinja.png");
	m_pickUpSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/pizza take.wav");

	SetPosition(SGD::Point{ 100, 100 });
	SetSize(SGD::Size{ 64, 64 });

	
	SetTimer(200);
}


Player::~Player()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_pickUpSound);
}

/*virtual*/ void	Player::Update(float elapsedTime)		/*override;*/
{
	if (m_timer > 0)
		m_timer -= elapsedTime;

	m_moveTimer += elapsedTime;
	if (!m_IsHookActive)
	{
		if ((pInput->IsKeyDown(SGD::Key::D) || pInput->IsDPadDown(0, SGD::DPad::Right) || pInput->GetLeftJoystick(0).x > 0) && m_moveTimer > 0.01f)
		{
			SetPosition({ GetPosition().x + (GetMoveSpeed() * 20)*elapsedTime, GetPosition().y });
			SetRotation((3 * SGD::PI) / 2);
		}

		if ((pInput->IsKeyDown(SGD::Key::A) || pInput->IsDPadDown(0, SGD::DPad::Left) || pInput->GetLeftJoystick(0).x < 0) && m_moveTimer > 0.01f)
		{
			SetPosition({ GetPosition().x - (GetMoveSpeed() * 20)*elapsedTime, GetPosition().y });
			SetRotation(SGD::PI / 2);
		}

		if ((pInput->IsKeyDown(SGD::Key::W) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && m_moveTimer > 0.01f)
		{
			SetPosition({ GetPosition().x, GetPosition().y - (GetMoveSpeed() * 20)*elapsedTime });
			SetRotation(SGD::PI);
		}

		if ((pInput->IsKeyDown(SGD::Key::S) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && m_moveTimer > 0.01f)
		{
			SetPosition({ GetPosition().x, GetPosition().y + (GetMoveSpeed() * 20)*elapsedTime });
			SetRotation(0);
		}
		/*if ((pInput->IsKeyPressed(SGD::Key::K)))
			SetCurrHealth(GetCurrHealth() - 10);
		
		if ((pInput->IsKeyPressed(SGD::Key::L)))
			SetCurrHealth(GetCurrHealth() + 10);*/
	}
	
	if (m_moveTimer > 0.01f)
		m_moveTimer = 0;
}

/*virtual*/ void	Player::Render(void)					/*override;*/
{
	float posx = GetPosition().x - (GetSize().width / 2) + Game::GetInstance()->GetWorldSpace().x;
	float posy = GetPosition().y - (GetSize().height / 2) + Game::GetInstance()->GetWorldSpace().y;

	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0 }, {}, 4);
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, { posx, posy }, GetRotation(), GetSize()/2, {}, {1.0F,1.0F});

	//health bar

		SGD::Rectangle myRect = SGD::Rectangle(
			150,
			Game::GetInstance()->GetScreenHeight() - 65,
			350,
			(float)(Game::GetInstance()->GetScreenHeight() - 45));

		SGD::GraphicsManager::GetInstance()->DrawRectangle(
			myRect,
			{ 0, 0, 0, 0 }, { 255, 20, 147 },
			5);
		if (GetCurrHP() / GetMaxHP() >= 0.75)
		{
			SGD::GraphicsManager::GetInstance()->DrawRectangle(
			{ myRect.left, myRect.top, myRect.left + ((GetCurrHP() * 200) / GetMaxHP()), myRect.bottom },
			{ 255, 0, 255, 0 }, { 0, 175, 90, 175 },
			0);
		}
		else if (GetCurrHP() / GetMaxHP() < 0.75)
		{
			SGD::GraphicsManager::GetInstance()->DrawRectangle(
			{ myRect.left, myRect.top, myRect.left + ((GetCurrHP() * 200) / GetMaxHP()), myRect.bottom },
			{ 255, 255, 140, 0 }, { 0, 175, 90, 175 },
			0);
		}
		else if (GetCurrHP() / GetMaxHP() <= 0.25)
		{
			SGD::GraphicsManager::GetInstance()->DrawRectangle(
			{ myRect.left, myRect.top, myRect.left + ((GetCurrHP() * 200) / GetMaxHP()), myRect.bottom },
			{ 255, 255, 0, 0 }, { 0, 175, 90, 175 },
			0);
		}

		const BitmapFont* pFont = Game::GetInstance()->GetFont();

		//timer
		SGD::OStringStream output;
		output << "Time:" << m_timer;

		if (m_timer > 10.0f)
			pFont->Draw(
			output.str().c_str(),
			{ (float)((Game::GetInstance()->GetScreenWidth()) - ((Game::GetInstance()->GetScreenWidth() / 4) + 150)),
			55 },
			0.3f, { 30, 144, 255 });
		else
			pFont->Draw(
			output.str().c_str(),
			{ (float)((Game::GetInstance()->GetScreenWidth()) - ((Game::GetInstance()->GetScreenWidth() / 4) + 150)),
			55 },
			0.3f, { 255, 0, 0 });

		//Sound

}

/*virtual*/ SGD::Rectangle	Player::GetRect(void)		const	/*override;*/
{
	SGD::Rectangle rect{ SGD::Point{ GetPosition().x - GetSize().width / 2, GetPosition().y - GetSize().height / 2 },GetSize() };
	return rect;
}

/*virtual*/ void	Player::HandleCollision(const IEntity* pOther)	/*override;*/
{
	if (pOther->GetType() == ENT_MOVABLE)
	{

	}
	else if (pOther->GetType() == ENT_IMMOVABLE)
	{

	}
}

/*virtual*/ void Player::HandleEvent(const SGD::Event* pEvent) /*override*/
{

	if (pEvent->GetEventID() == "MovePlayer")
	{
		if (GetRotation() == SGD::PI)
			m_ptPosition.y -= m_Hook+1;
		else if (GetRotation() == 0)
			m_ptPosition.y += m_Hook+1;
		else if (GetRotation() == (3 * SGD::PI) / 2)
			m_ptPosition.x += m_Hook+1;
		else if (GetRotation() == SGD::PI / 2)
			m_ptPosition.x -= m_Hook+1;		
	}
}

void	Player::PlayPickupSound()
{
	if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_pickUpSound))
		SGD::AudioManager::GetInstance()->PlayAudio(m_pickUpSound);
}
