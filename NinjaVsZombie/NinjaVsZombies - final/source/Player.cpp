#include "Player.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "Game.h"
#include "GamePlayState.h"
#include "EntityManager.h"

#include "ImmovableBox.h"
#include "MovableBox.h"
#include "SpawnPoint.h"
#include "BitmapFont.h"
#include "LevelManager.h"
#include "Tile.h"
#include "Trap.h"
#include "Map.h"
#include "Door.h"
#include "LustWrath.h"

//string table
#include <Windows.h>
#include "../resource2.h"
#include <atlstr.h>

#include "Emiter.h"
#include "AnimationManager.h"
#include "DeleteEmitterMessage.h"
#include "GameOverMessage.h"
#include "GamePlayState.h"
#include "LevelManager.h"
#include "Room.h"

Player::Player() : Listener(this)
{
	Listener::RegisterForEvent("MovePlayer");
	Listener::RegisterForEvent("PLAYER_SPOTTED");
	Listener::RegisterForEvent("PLAYER_OUT");
	Listener::RegisterForEvent("FrameTest");
	Listener::RegisterForEvent("Player_HIT");
	Listener::RegisterForEvent("Player_Fight");
	Listener::RegisterForEvent("fight_over");

	m_pickUpSound = SGD::AudioManager::GetInstance()->LoadAudio("resource/audio/soundeffects/pizza take.wav");
	m_hSmokebomb = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/SmokeBomb.png");
	SetPosition(SGD::Point{ 100, 100 });
	SetSize(SGD::Size{ 32, 32 });

	//HUD
	m_HHidden = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Hidden.png");
	m_HSpotted = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/Spotted.png");
	m_HNinjaIcon = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/NinjaIcon.png");
	m_hPizza = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/pizzainstruct.png");
	m_fullhealth = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/health bar_green.png");
	m_halfhealth = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/health bar_yellow.png");
	m_lowhealth = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/health bar_red.png");
	m_hurtbar = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/health bar_hurtnew.png");
	m_bar = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/health bar.png");
	m_DAQS = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/Graphics/coin.png");

	m_hit = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/NinjaHurt.wav");
	m_death = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/Neck Breaking.wav");

	SetTimer(35);
	SetFalling(false);
	SetHavePizza(false);

	//PU
	m_HealthPU = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/HealthPower.wav");
	m_MoneyPU = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/MoneyPower.wav");
	m_SpeedPU = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/SpeedPower.wav");
	m_StrengthPU = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/SwordPower.wav");
	m_TimePU = SGD::AudioManager::GetInstance()->LoadAudio("resource/Audio/SoundEffects/TimerPower.wav");
}

Player::~Player()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_HNinjaIcon);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_HHidden);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_HSpotted);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSmokebomb);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPizza);
	// hud
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_fullhealth);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_halfhealth);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_lowhealth);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hurtbar);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_bar);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_DAQS);

	SGD::AudioManager::GetInstance()->UnloadAudio(m_hit);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_death);

	SGD::AudioManager::GetInstance()->UnloadAudio(m_pickUpSound);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_HealthPU);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_MoneyPU);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_SpeedPU);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_StrengthPU);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_TimePU);

	//SetEmiter(nullptr);
}

/*virtual*/ void	Player::Update(float elapsedTime)		/*override;*/
{
	if (m_bCharmed != true)
	{
		SetMoveSpeed(m_nPrevSpeed);
		m_fCharmTime = 0.0f;
	}
	else
	{
		m_fCharmTime += elapsedTime;

		if (m_fCharmTime >= 5.0f)
		{
			m_bCharmed = false;
			m_fCharmTime = 0.0f;
		}
	}

	if (m_AlarmTimer > 0)
	{
		m_AlarmTimer -= elapsedTime;
		if (m_AlarmTimer < 0)
		{
			SGD::Event* myEvent = new SGD::Event{ "PLAYER_OUT", "", this };

			myEvent->QueueEvent(nullptr);
			myEvent = nullptr;
		}
	}
	if (m_bBurned == true)
	{
		SetMoveSpeed(m_nPrevSpeed/2);
		m_fBurntime += elapsedTime;

		if (m_fBurntime >= 0.25f)
		{
			m_fBurntime = 0.0f;
			m_bBurned = false;
			SetCurrHP(GetCurrHP() - 1);
		}
	}
	else
		SetMoveSpeed(m_nPrevSpeed);

	if (m_bShocked == true)
	{
		m_fShocktime += elapsedTime;

		if (m_fShocktime >= 1.0f && m_bStunned == false)
		{
			m_fShocktime = 0.0f;
			SetCurrHP(GetCurrHP() - 3);
			m_bShocked = false;
			//if (m_bStunned == false)
				m_bStunned = true;
		}
	}

	if (m_bStunned == true)
	{
		SetMoveSpeed(0);
		m_fStuntime += elapsedTime;

		if (m_fStuntime >= 0.5f)
		{
			m_fStuntime = 0.0f;
			m_bStunned = false;
		}
	}
	else
		SetMoveSpeed(m_nPrevSpeed);


	if (m_bPunctured == true)
	{
		if (m_spikeHit == false)
		{
			ApplyKnockBack();
			m_spikeHit = true;
		}

		m_fKnockBacktime += elapsedTime;

		if (m_fKnockBacktime >= 0.5f)
		{
			m_fKnockBacktime = 0.0f;
			m_bPunctured = false;
		}
	}

	if (hurthud == true)
	{
		hudtimer += elapsedTime;
		if (hudtimer > .5f)
		{
			hurthud = false;
			hudtimer = 0;
		}
	}

	if (GetCurrHP() <= 0)
	{
		dead = true;
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_death))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_death);
		}
	}

	if (m_timer > 0)
		m_timer -= elapsedTime;

	if (m_PowerUpTimer > 0)
		m_PowerUpTimer -= elapsedTime;
	else if (activePower.size() > 0) 
	{
		for (unsigned int i = 0; i < activePower.size(); i++)
		{
			if (activePower.size() == 0)
				break;
			if (activePower[i] == 0) // strength
				SetStrength(GetStrength() - 10 * GetGameLevel()); 
			else if (activePower[i] == 1) //speed
			{
				SetMoveSpeed(GetMoveSpeed() - 50 * GetGameLevel());
				SetPrevSpeed(GetPrevSpeed() - 50 * GetGameLevel());

			}
			//activePower.pop_back();
		}
		activePower.clear();
		DeleteEmitterMessage* emimess = new DeleteEmitterMessage(this);
		emimess->QueueMessage();
		emimess = nullptr;

		//SetEmiter(nullptr);

	}

	m_moveTimer += elapsedTime;
	if (!fighting && !m_bCharmed)
	{
		if (!m_IsHookActive && !IsFalling())
		{
			SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
			SGD::Point newPoint = { 0, 0 };
			SGD::Point centerPoint = GetRect().ComputeCenter();
			if ((pInput->IsKeyDown(SGD::Key::D) || pInput->IsDPadDown(0, SGD::DPad::Right) || pInput->GetLeftJoystick(0).x > 0) && m_moveTimer > 0.01f)
			{
				newPoint = { (GetPosition().x + (GetMoveSpeed())*elapsedTime) - GetSize().width / 2, GetPosition().y - GetSize().height / 2 };
				if (rect.IsPointInRectangle({ (centerPoint.x + (GetMoveSpeed())*elapsedTime) + GetSize().width / 2, centerPoint.y }) && CheckCollision(newPoint))
						SetPosition({ (GetPosition().x + (GetMoveSpeed())*elapsedTime), GetPosition().y });
				else if (!rect.IsPointInRectangle({ (centerPoint.x + (GetMoveSpeed())*elapsedTime) + GetSize().width / 2, centerPoint.y }) && GetHavePizza() && CheckCollision(newPoint))
					SetPosition({ (GetPosition().x + (GetMoveSpeed())*elapsedTime), GetPosition().y });

				SetRotation((3 * SGD::PI) / 2);
				AnimationManager::GetInstance()->Update(elapsedTime, m_Left);
				up = false;
				left = true;
				right = false;
				down = false;
			}

			if ((pInput->IsKeyDown(SGD::Key::A) || pInput->IsDPadDown(0, SGD::DPad::Left) || pInput->GetLeftJoystick(0).x < 0) && m_moveTimer > 0.01f)
			{
				newPoint = { (GetPosition().x - (GetMoveSpeed())*elapsedTime) - GetSize().width / 2, GetPosition().y - GetSize().height / 2 };
				if (rect.IsPointInRectangle({ (centerPoint.x - (GetMoveSpeed())*elapsedTime) - GetSize().width / 2, centerPoint.y }) && CheckCollision(newPoint))
					SetPosition({ (GetPosition().x - (GetMoveSpeed())*elapsedTime), GetPosition().y });
				else if (!rect.IsPointInRectangle({ (centerPoint.x - (GetMoveSpeed())*elapsedTime) - GetSize().width / 2, centerPoint.y }) && GetHavePizza() && CheckCollision(newPoint))
					SetPosition({ (GetPosition().x - (GetMoveSpeed())*elapsedTime), GetPosition().y });

				SetRotation(SGD::PI / 2);
				AnimationManager::GetInstance()->Update(elapsedTime, m_Left);
				up = false;
				left = false;
				right = true;
				down = false;
			}

			if ((pInput->IsKeyDown(SGD::Key::W) || pInput->IsDPadDown(0, SGD::DPad::Up) || pInput->GetLeftJoystick(0).y < 0) && m_moveTimer > 0.01f)
			{
				newPoint = { GetPosition().x - GetSize().width / 2, (GetPosition().y - (GetMoveSpeed())*elapsedTime) - GetSize().height / 2 };
				if (rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y - (GetMoveSpeed())*elapsedTime) - GetSize().height / 2 }) && CheckCollision(newPoint))
					SetPosition({ GetPosition().x, (GetPosition().y - (GetMoveSpeed())*elapsedTime) });
				else if (!rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y - (GetMoveSpeed())*elapsedTime) - GetSize().height / 2 }) && GetHavePizza() && CheckCollision(newPoint))
					SetPosition({ GetPosition().x, (GetPosition().y - (GetMoveSpeed())*elapsedTime) });

				SetRotation(SGD::PI);
				AnimationManager::GetInstance()->Update(elapsedTime, m_Up);
				up = true;
				left = false;
				right = false;
				down = false;
			}

			if ((pInput->IsKeyDown(SGD::Key::S) || pInput->IsDPadDown(0, SGD::DPad::Down) || pInput->GetLeftJoystick(0).y > 0) && m_moveTimer > 0.01f)
			{
				newPoint = { GetPosition().x - GetSize().width / 2, (GetPosition().y + (GetMoveSpeed())*elapsedTime) - GetSize().height / 2 };
				if (rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y + (GetMoveSpeed())*elapsedTime)+ GetSize().height / 2 }) && CheckCollision(newPoint))
					SetPosition({ GetPosition().x, (GetPosition().y + (GetMoveSpeed())*elapsedTime) });
				else if (!rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y + (GetMoveSpeed())*elapsedTime) + GetSize().height / 2 }) && GetHavePizza() && CheckCollision(newPoint))
					SetPosition({ GetPosition().x, (GetPosition().y + (GetMoveSpeed())*elapsedTime) });

				SetRotation(0);
				AnimationManager::GetInstance()->Update(elapsedTime, m_Down);
				up = false;
				left = false;
				right = false;
				down = true;
			}
		}
		if (m_IsBeingPulled && m_pulledTimer >= 0)
		{
			m_pulledTimer -= elapsedTime;
			float x = m_ptPosition.x, y = m_ptPosition.y;
			if (GetRotation() == SGD::PI)
				y = m_ptPosition.y - 100 * m_Hook * elapsedTime;
			else if (GetRotation() == 0)
				y = m_ptPosition.y + 100 * m_Hook * elapsedTime;
			else if (GetRotation() == (3 * SGD::PI) / 2)
				x = m_ptPosition.x + 100 * m_Hook * elapsedTime;
			else if (GetRotation() == SGD::PI / 2)
				x = m_ptPosition.x - 100 * m_Hook * elapsedTime;

			SGD::Point point = SGD::Point{ x, y };
			if (!GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point - GetSize() / 2, this, GamePlayState::GetInstance()->BUCKET_BOX_IMMOVABLE)
				&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point - GetSize() / 2, this, GamePlayState::GetInstance()->BUCKET_ENEMY))
				SetPosition(point);
		}
	}
	else
	{
		if (pInput->IsKeyPressed(SGD::Key::Space) || pInput->IsButtonPressed(0,3))
		{
			SGD::Event* fightback = new SGD::Event{ "fight_back", nullptr, this };
			fightback->SendEventNow(nullptr);
			delete fightback;
		}
	}

	if (m_moveTimer > 0.01f)
		m_moveTimer = 0;

	if (IsFalling())
	{
		m_Scale.width -= elapsedTime;
		m_Scale.height -= elapsedTime;

		if (m_Scale.width <= 0 && m_Scale.height <= 0)
		{
			vector<Room*> rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
			for (size_t i = 0; i < rooms.size(); i++)
			{
				if (rooms[i]->IsPlayerHere())
					SetPosition(rooms[i]->GetRespawnpoint());
				SGD::AudioManager::GetInstance()->PlayAudio(m_hit);
			}
			SetCurrHP(GetCurrHP() - 10);
			m_Scale = { 1.0f, 1.0f };
			SetFalling(false);
			fighting = false;
			m_bCharmed = false;
			m_IsBeingPulled = false;
		}
	}

	if (GetCurrHP() <= 0)
	{
		dead = true;
		SGD::AudioManager::GetInstance()->PlayAudio(m_death);

		GameOverMessage* gameover = new GameOverMessage(this);
		gameover->QueueMessage();
		gameover = nullptr;
	}
	else
	{
		SGD::Event* Target = new SGD::Event{ "Target", nullptr, this };
		Target->QueueEvent();
		Target = nullptr;
	}
}

/*virtual*/ void	Player::Render(void)				/*override;*/
{
	SGD::Rectangle rect = GetRect();
	rect.Offset(Game::GetInstance()->GetWorldSpace().x, Game::GetInstance()->GetWorldSpace().y);

	//SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 0, 0, 0 }, {}, 0);
	
	//health bar
	if ((float)GetCurrHP() / GetMaxHP() >= 0.75)
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_fullhealth, { 125, Game::GetInstance()->GetScreenHeight() - 55 }, SGD::Rectangle{ 140, 116, 140 + ((float)GetCurrHP() * 275 / GetMaxHP()), 148 }, 0, {}, {}, { .8f, .7f });
	}
	else if ((float)GetCurrHP() / GetMaxHP() < 0.75 && (float)GetCurrHP() / GetMaxHP() > .25f)
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_halfhealth, { 125, Game::GetInstance()->GetScreenHeight() - 55 }, SGD::Rectangle{ 140, 116, 140 + ((float)GetCurrHP() * 275 / GetMaxHP()), 148 }, 0, {}, {}, { .8f, .7f });
	}
	else if ((float)GetCurrHP() / GetMaxHP() <= 0.25)
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_lowhealth, { 125, Game::GetInstance()->GetScreenHeight() - 55 }, SGD::Rectangle{ 140, 116, 140 + ((float)GetCurrHP() * 275 / GetMaxHP()), 148 }, 0, {}, {}, { .8f, .7f });
	}
	
	if (!hurthud)
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_bar, { 85, Game::GetInstance()->GetScreenHeight() - 90 }, SGD::Rectangle{ 94, 75, 417, 188 }, 0, {}, {}, { .8f, .8f });
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hurtbar, { 85, Game::GetInstance()->GetScreenHeight() - 90 }, SGD::Rectangle{ 94, 75, 417, 188 }, 0, {}, {}, { .8f, .8f });
	}


	const BitmapFont* pFont = Game::GetInstance()->GetFont();


	// animations
	if (m_SafeZone == false)
	{
		if (up)
		{
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y - (int)(GetSize().height / 2), m_Up, false, m_Scale);
		}
		else if (down)
		{
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y - (int)(GetSize().height / 2), m_Down, false, m_Scale);
		}
		else if (left)
		{
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y - (int)(GetSize().height / 2), m_Left, false, m_Scale);
		}
		else if (right)
		{
			AnimationManager::GetInstance()->Render((int)GetPosition().x, (int)GetPosition().y - (int)(GetSize().height / 2), m_Left, true, m_Scale);
		}
	}

	//timer
	SGD::OStringStream output;

	HINSTANCE HInstance = Game::GetInstance()->GetHInstance();
	char buf[255];
	
	if (m_timer >= 0)
	{

		output.precision(4);

		if (m_timer > 100)
			output.precision(4);
		else
			output.precision(3);

		LoadString(HInstance, IDS_TIME, buf, 255);
		output << buf << m_timer;
	}

	if (m_timer > 20.0f)
		pFont->Draw(
		output.str().c_str(),
		{ (float)((Game::GetInstance()->GetScreenWidth()) - ((Game::GetInstance()->GetScreenWidth() / 4) + 250)),
		Game::GetInstance()->GetScreenHeight() * 6 / 7 + 50 },
		0.8f, { 30, 144, 255 });

	else if (m_timer > 0.0f)
	{
		pFont->Draw(
			output.str().c_str(),
			{ (float)((Game::GetInstance()->GetScreenWidth()) - ((Game::GetInstance()->GetScreenWidth() / 4) + 250)),
			Game::GetInstance()->GetScreenHeight() * 6 / 7 + 50 },
			0.8f, { 255, 0, 0 });
	}
	else if (m_timer < 0.0f)
	{
		LoadString(HInstance, IDS_TIMEOVER, buf, 255);

		pFont->Draw(
			buf,
			{ (float)((Game::GetInstance()->GetScreenWidth()) - ((Game::GetInstance()->GetScreenWidth() / 4) + 250)),
			Game::GetInstance()->GetScreenHeight() * 6 / 7 + 50 },
			0.8f, { 255, 0, 0 });
	}

	// DAQS
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_DAQS, { (Game::GetInstance()->GetScreenWidth() / 100) + 575, Game::GetInstance()->GetScreenHeight() * 6 / 7 + 20 }, 0, {}, {172, 172, 172}, { .1f, .1f });
	SGD::OStringStream DAqs;
	DAqs << "$" << m_DAQ;

	pFont->Draw(DAqs.str().c_str(), { (Game::GetInstance()->GetScreenWidth() / 100) + 572, Game::GetInstance()->GetScreenHeight() * 6 / 7 + 60 },
		0.8f, { 0, 0, 0 });

	pFont->Draw(DAqs.str().c_str(), { (Game::GetInstance()->GetScreenWidth() / 100) + 572, Game::GetInstance()->GetScreenHeight() * 6 / 7 + 60 },
		0.78f, { 255, 255, 255 });
	//stealth HUD
	if (m_AlarmSpotted == false && m_SafeZone == false)
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_HNinjaIcon, { (Game::GetInstance()->GetScreenWidth() / 100) - 5, (Game::GetInstance()->GetScreenHeight() * 6 / 7) + 10},
		0.0f, {}, { 255, 255, 255, 255 }, { 0.15f, 0.15f });

	else if (m_AlarmSpotted == true)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_HSpotted, { Game::GetInstance()->GetScreenWidth() / 18 - 5, (Game::GetInstance()->GetScreenHeight() * 6 / 7) + 10},
			0.0f, {}, { 255, 255, 255, 255 }, { 0.15f, 0.15f });
	}
	else if (m_SafeZone == true)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_HHidden, { Game::GetInstance()->GetScreenWidth() / 39 - 5, (Game::GetInstance()->GetScreenHeight() * 5 / 6) + 10 },
			0.0f, {}, { 255, 255, 255, 255 }, { 0.15f, 0.15f });
	}

	// having the pizza vs not
	if (GetHavePizza())
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hPizza, { (Game::GetInstance()->GetScreenWidth() / 100) + 642, Game::GetInstance()->GetScreenHeight() * 6 / 7  + 20},
			0.0f, {}, { 255, 255, 255, 255 }, { 0.2f, 0.2f });
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hPizza, { (Game::GetInstance()->GetScreenWidth() / 100) + 642, Game::GetInstance()->GetScreenHeight() * 6 / 7 + 20},
			0.0f, {}, { 130, 255, 255, 255 }, { 0.2f, 0.2f });
	}


	// how many smokebombs
	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hSmokebomb, { (Game::GetInstance()->GetScreenWidth() / 100) + 715, (Game::GetInstance()->GetScreenHeight() * 6 / 9) + 145 },
		0.0f, {}, { 255, 255, 255, 255 }, { 0.075f, 0.075f });

	SGD::OStringStream smokebomb;
	smokebomb << "x" << m_numSmokeBomb;
	pFont->Draw(smokebomb.str().c_str(), { (Game::GetInstance()->GetScreenWidth() / 100) + 715, (Game::GetInstance()->GetScreenHeight() * 6 / 9) + 145 }, .82f, { 255, 0, 0, 0 });

	pFont->Draw(smokebomb.str().c_str(), { (Game::GetInstance()->GetScreenWidth() / 100) + 715, (Game::GetInstance()->GetScreenHeight() * 6 / 9) + 145 }, .8f, { 255, 255, 255, 255 });}

/*virtual*/ SGD::Rectangle	Player::GetRect(void)		const	/*override;*/
{
	SGD::Rectangle rect{ SGD::Point{ GetPosition().x - GetSize().width / 2, GetPosition().y - GetSize().height / 2 }, GetSize() };
	return rect;
}

/*virtual*/ void	Player::HandleCollision(const IEntity* pOther)	/*override;*/
{

}

/*virtual*/ void	Player::HandleEvent(const SGD::Event* pEvent) /*override*/
{

	if (pEvent->GetEventID() == "MovePlayer")
	{
		m_IsBeingPulled = true;
		m_pulledTimer = 2.0f;
	}
	if (pEvent->GetEventID() == "PLAYER_SPOTTED")
	{
		m_AlarmSpotted = true;
		m_AlarmTimer = 15.0f;
	}
	if (pEvent->GetEventID() == "PLAYER_OUT")
	{
		m_AlarmSpotted = false;
	}
	if (pEvent->GetEventID() == "Player_HIT")
	{
		Entity* sender = (Entity*)pEvent->GetSender();
		
		hurthud = true;
		if (sender != nullptr)
		{
			if (sender->GetType() == ENT_BULLET)
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
					SetCurrHP(GetCurrHP() - 10);

			}

			if (sender->GetType() == ENT_HEART)
			{
				m_bCharmed = true;
			}

			if (sender->GetType() == ENT_FIRETRAP && m_bBurned == false)
			{
				m_bBurned = true; 
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
				SetCurrHP(GetCurrHP() - ((Trap*)sender)->GetDamage());
			}

			if (sender->GetType() == ENT_ELECTRICTRAP && m_bShocked == false)
			{
				m_bShocked = true;
				m_bStunned = true;
				SetCurrHP(GetCurrHP() - ((Trap*)sender)->GetDamage());
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit) )
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
			}

			if (sender->GetType() == ENT_SPIKETRAP && m_bPunctured == false)
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
				m_bPunctured = true;
				SetCurrHP(GetCurrHP() - ((Trap*)sender)->GetDamage());
			}

			if (sender->GetType() == ENT_ZOMBIE)
			{
				SetCurrHP(GetCurrHP() - 3);
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
			}
			if (sender->GetType() == ENT_NEUTRAL_ENEMY)
			{
				SetCurrHP(GetCurrHP() - 10);
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
			}

			if (sender->GetType() == ENT_BOSS)
			{
				SetCurrHP(GetCurrHP() - 70);
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
			}

			if (sender->GetType() == ENT_LUST)
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hit))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hit);

				}
				if (((LustWrath*)sender)->GetWrath() == true)
					SetCurrHP(GetCurrHP() - 50);
				else
					SetCurrHP(GetCurrHP() - 30);

				ApplyKnockBack();
				m_bCharmed = false;
			}

		}
	}
	if (pEvent->GetEventID() == "Player_Fight")
	{
		fighting = true;
	}
	if (pEvent->GetEventID() == "fight_over")
	{
		fighting = false;
	}
}

void Player::PlayPickupSound()
{
	if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_pickUpSound))
		SGD::AudioManager::GetInstance()->PlayAudio(m_pickUpSound);
}
	 
void Player::AddActivePower(int newpower)
{
	activePower.push_back(newpower);
}

void Player::ApplyKnockBack()
{
	int knockspeed = 50;

	SGD::Rectangle rect = Game::GetInstance()->GetWorldCamera();
	SGD::Point newPoint = { 0, 0 };
	SGD::Point centerPoint = GetRect().ComputeCenter();

	if (left)//right
	{
		newPoint = { (GetPosition().x - knockspeed)-GetSize().width / 2, GetPosition().y - GetSize().height / 2 };

		if (rect.IsPointInRectangle({ (centerPoint.x - knockspeed)-GetSize().width / 2, centerPoint.y }) && CheckCollision(newPoint))
			SetPosition({ (GetPosition().x - knockspeed), GetPosition().y });
		else if (!rect.IsPointInRectangle({ (centerPoint.x - knockspeed)-GetSize().width / 2, centerPoint.y }) && GetHavePizza() && CheckCollision(newPoint))
			SetPosition({ (GetPosition().x - knockspeed), GetPosition().y });
	}
	else if (right)//left
	{
		newPoint = { (GetPosition().x + knockspeed)-GetSize().width / 2, GetPosition().y - GetSize().height / 2 };

		if (rect.IsPointInRectangle({ (centerPoint.x + knockspeed)+GetSize().width / 2, centerPoint.y }) && CheckCollision(newPoint))
			SetPosition({ (GetPosition().x + knockspeed), GetPosition().y });
		else if (!rect.IsPointInRectangle({ (centerPoint.x + knockspeed) + GetSize().width / 2, centerPoint.y }) && GetHavePizza() && CheckCollision(newPoint))
			SetPosition({ (GetPosition().x + knockspeed), GetPosition().y });
	}
	else if (up)
	{
		newPoint = { GetPosition().x - GetSize().width / 2, (GetPosition().y + knockspeed)-GetSize().height / 2 };

		if (rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y + knockspeed)+GetSize().height / 2 }) && CheckCollision(newPoint))
			SetPosition({ GetPosition().x, (GetPosition().y + knockspeed) });
		else if (!rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y + knockspeed)+GetSize().height / 2 }) && GetHavePizza() && CheckCollision(newPoint))
			SetPosition({ GetPosition().x, (GetPosition().y + knockspeed) });
	}
	else if (down)
	{
		newPoint = { GetPosition().x - GetSize().width / 2, (GetPosition().y - knockspeed)-GetSize().height / 2 };

		if (rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y - knockspeed)-GetSize().height / 2 }) && CheckCollision(newPoint))
			SetPosition({ GetPosition().x, (GetPosition().y - knockspeed) });
		else if (!rect.IsPointInRectangle({ centerPoint.x, (centerPoint.y - knockspeed)-GetSize().height / 2 }) && GetHavePizza() && CheckCollision(newPoint))
			SetPosition({ GetPosition().x, (GetPosition().y - knockspeed) });
	}

}

void Player::PlayMoney()
{
	SGD::AudioManager::GetInstance()->PlayAudio(m_MoneyPU);
}

void Player::PlayStrength()
{
	SGD::AudioManager::GetInstance()->PlayAudio(m_StrengthPU);
}

void Player::PlayTime()
{
	SGD::AudioManager::GetInstance()->PlayAudio(m_TimePU);
}

void Player::PlaySpeed()
{
	SGD::AudioManager::GetInstance()->PlayAudio(m_SpeedPU);
}

void Player::PlayHealth()
{
	SGD::AudioManager::GetInstance()->PlayAudio(m_HealthPU);
}

bool Player::CheckCollision(SGD::Point point)
{
	std::vector<Room*>	rooms = GamePlayState::GetInstance()->GetLevelManager()->GetRooms();
	bool TileCollision = false;
	for (size_t i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->IsPlayerHere())
			TileCollision = rooms[i]->CheckCollision(point, this);
	}
	if (TileCollision == false && !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_MOVABLE)
		&& !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_BOX_IMMOVABLE)
		&& (!GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_ENEMY)
		|| !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_PRIDE)
		|| !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_LUST)
		|| !GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_PLAYER))
		&& (!GamePlayState::GetInstance()->GetEntityManager()->IsGoingToCollide(point, this, GamePlayState::GetInstance()->BUCKET_DOOR) && m_bDoorCol == false))
		return true;
	return false;
}

void Player::SetSafeZone(bool newZone)
{
	 m_SafeZone = newZone;
}

void Player::CheckPowerUp()
{
	if (activePower.size() > 0) //delete emitters
	{
		for (unsigned int i = 0; i < activePower.size(); i++)
		{
			if (activePower.size() == 0)
				break;
			if (activePower[i] == 0) // strength
				SetStrength(GetStrength() - 10 * GetGameLevel()); // needs * level
			else if (activePower[i] == 1) //speed
			{
				SetMoveSpeed(GetMoveSpeed() - 50 * GetGameLevel()); // needs * level
				SetPrevSpeed(GetPrevSpeed() - 50 * GetGameLevel());

			}
		}
		activePower.clear();
	}
}
