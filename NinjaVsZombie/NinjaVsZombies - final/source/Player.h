#pragma once


#include "Unit.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "TimeStamp.h"
#include <vector>
using namespace std;
/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Pizza;

class Player : public Unit, public SGD::Listener
{
	//bool m_HavePizza = false;
	bool m_IsHookActive = false;
	bool m_IsSwordActive = false;
	bool m_HaveHook = false;
	bool m_HaveSword = false;
	bool m_IsPlayerVisible = true;
	bool m_IsBeingPulled = false;
	bool fighting = false;
	bool m_bCharmed = false;
	float m_fCharmTime = 0.0f;
	//Alarm system
	bool	m_AlarmSpotted	= false;
	bool	m_SafeZone = false;

	// animations
	TimeStamp m_Left;
	TimeStamp m_Down;
	TimeStamp m_Up;
	bool up = false;
	bool left = true;
	bool right = false;
	bool down = false;



	int m_Hook = 1;
	int m_Sword = 1;
	int m_GameLvl = 0;

	SGD::Size m_Scale = { 1.0f, 1.0f };
	float m_moveTimer = 0.0f;
	float m_pulledTimer = 0.0f;

	//powerUp
	float m_PowerUpTimer = 0.0f;
	std::vector<int> activePower;
	SGD::HAudio		m_HealthPU = SGD::INVALID_HANDLE;
	SGD::HAudio		m_MoneyPU = SGD::INVALID_HANDLE;
	SGD::HAudio		m_SpeedPU = SGD::INVALID_HANDLE;
	SGD::HAudio		m_StrengthPU = SGD::INVALID_HANDLE;
	SGD::HAudio		m_TimePU = SGD::INVALID_HANDLE;

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::HAudio		m_pickUpSound = SGD::INVALID_HANDLE;

	//HUD
	float m_timer;
	bool m_stealth = false;
	int m_DAQ = 0;
	unsigned int m_numSmokeBomb = 0;
	SGD::HTexture			m_HHidden	= SGD::INVALID_HANDLE;
	SGD::HTexture			m_HSpotted	= SGD::INVALID_HANDLE;
	SGD::HTexture			m_HNinjaIcon = SGD::INVALID_HANDLE;
	SGD::HTexture			m_hSmokebomb = SGD::INVALID_HANDLE;
	SGD::HTexture			m_hPizza = SGD::INVALID_HANDLE;
	SGD::HTexture			m_DAQS = SGD::INVALID_HANDLE;
	// hud
	SGD::HTexture			m_fullhealth = SGD::INVALID_HANDLE;
	SGD::HTexture			m_halfhealth = SGD::INVALID_HANDLE;
	SGD::HTexture			m_lowhealth = SGD::INVALID_HANDLE;
	SGD::HTexture			m_hurtbar = SGD::INVALID_HANDLE;
	SGD::HTexture			m_bar = SGD::INVALID_HANDLE;

	bool					hurthud = false;
	float					hudtimer = 0;
	float					m_AlarmTimer = 0.0f;

	SGD::HAudio					m_hit = SGD::INVALID_HANDLE;
	SGD::HAudio					m_death = SGD::INVALID_HANDLE;

	// boss key
	bool m_bHaskey = false;

	//trap bools
	bool m_bBurned = false;
	bool m_bShocked = false;
	bool m_bStunned = false;
	bool m_bPunctured = false;
	bool m_spikeHit = false;
	bool m_shockhit = false;

	int m_nPrevSpeed = 100;

	//trap timers
	float m_fBurntime = 0.0f;
	float m_fShocktime = 0.0f;
	float m_fStuntime = 0.0f;
	float m_fKnockBacktime = 0.0f;

	//door bool
	bool m_bDoorCol = false;

public:
	Player();
	~Player();

	bool dead = false;


	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_PLAYER; }
	virtual SGD::Rectangle	GetRect(void)	const			override;
	virtual void			HandleCollision(const IEntity* pOther)	override;

	int						GetPrevSpeed(void)const								{ return m_nPrevSpeed; }
	//bool					HavePizza(void)const								{ return m_HavePizza; }
	bool					HaveHook(void)const									{ return m_HaveHook; }
	bool					HaveSword(void)const								{ return m_HaveSword; }
	bool					IsHookActive(void)const								{ return m_IsHookActive; }
	bool					IsSwordActive(void)const							{ return m_IsSwordActive; }
	bool					GetStealth(void)const								{ return m_stealth; }
	bool					GetSafeZone(void)const								{ return m_SafeZone; }
	bool					GetFighting()const									{ return fighting; };
	bool					IsBeingPulled()const								{ return m_IsBeingPulled; }
	int						GetDAQ(void)const									{ return m_DAQ; }
	unsigned int			GetNumSmokeBombs(void)const							{ return m_numSmokeBomb; }
	float					GetTimer(void)const									{ return m_timer; }
	float					GetPowerUpTimer(void)const							{ return m_PowerUpTimer; }
	int						GetHookLevel(void)const								{ return m_Hook; }
	int						GetSwordLevel(void)const							{ return m_Sword; }
	int						GetGameLevel(void)const								{ return m_GameLvl; }
	bool					GetHasKey() const									{ return m_bHaskey; }
	bool					GetCharmed()const									{ return m_bCharmed; }
	bool					GetDoorCollide()const								{ return m_bDoorCol; }

	void					SetPrevSpeed(int prev)						{ m_nPrevSpeed = prev; }
	//void					SetHavePizza(bool newHaveP)					{ m_havePizza = newHaveP; }
	void					SetHaveHook(bool newHaveH)					{ m_HaveHook = newHaveH; }
	void					SetHaveSword(bool newHaveS)					{ m_HaveSword = newHaveS; }
	void					SetHookActive(bool hooK)					{ m_IsHookActive = hooK; }
	void					SetSwordActive(bool sword)					{ m_IsSwordActive = sword; }
	void					SetDAQ(int DAQ)								{ m_DAQ = DAQ; }
	void					SetNumSmokeBombs(unsigned int smoke)		{ m_numSmokeBomb = smoke; }
	void					SetHookLevel(int newHook)					{ m_Hook = newHook; }
	void					SetSwordLevel(int newSword)					{ m_Sword = newSword; }
	void					SetTimer(float newTimer)					{ m_timer = newTimer; }
	void					SetSafeZone(bool newZone);
	void					SetPowerUpTimer(float newTimer)				{ m_PowerUpTimer = newTimer; }
	void					SetGameLevel(int newLevel)					{ m_GameLvl = newLevel; }
	void					AddActivePower(int newPower);
	void					SetUpAnimation(TimeStamp up)				{ m_Up = up; }
	void					SetDownAnimation(TimeStamp down)			{ m_Down = down; }
	void					SetLeftAnimation(TimeStamp left)			{ m_Left = left; }
	void					SetHasKey(bool key)							{ m_bHaskey = key; }
	void					SetCharmed(bool charm)						{ m_bCharmed = charm; }
	void					SetSpikeHit(bool hit)						{ m_spikeHit = hit; }
	void					SetDoorCollide(bool col)					{ m_bDoorCol = col; }

	void					SetBeingPulled(bool pull)					{ m_IsBeingPulled = pull; }
	
	void					PlayPickupSound();

	bool					CheckCollision(SGD::Point point);
	void					CheckPowerUp();
	virtual void HandleEvent(const SGD::Event* pEvent) override;

	void		ApplyKnockBack();
	void		PlayMoney();
	void		PlayStrength();
	void		PlayTime();
	void		PlaySpeed();
	void		PlayHealth();
};