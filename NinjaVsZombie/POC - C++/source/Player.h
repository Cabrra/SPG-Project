#pragma once


#include "Unit.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class Pizza;

class Player : public Unit, public SGD::Listener
{
	bool m_HavePizza = false;
	bool m_IsHookActive = false;
	bool m_IsSwordActive = false;
	bool m_HaveHook = false;
	bool m_HaveSword = false;
	bool m_IsPlayerVisible = true;

	float m_Hook = 1;
	float m_Sword = 1;
	float m_moveTimer = 0.0f;

	unsigned int m_DAQ = 0;
	unsigned int m_numSmokeBomb = 0;

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	SGD::HAudio		m_pickUpSound = SGD::INVALID_HANDLE;

	float m_timer;

	bool m_stealth = false;
	float m_soundIntensity = 0.0f;
	

public:
	Player();
	~Player();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_PLAYER; }
	virtual SGD::Rectangle	GetRect(void)	const			override;
	virtual void			HandleCollision(const IEntity* pOther)	override;

	bool					HavePizza(void)								{ return m_HavePizza; }
	bool					HaveHook(void)								{ return m_HaveHook; }
	bool					HaveSword(void)								{ return m_HaveSword; }
	bool					IsHookActive(void)							{ return m_IsHookActive; }
	bool					IsSwordActive(void)							{ return m_IsSwordActive; }
	unsigned int			GetDAQ(void)								{ return m_DAQ; }
	unsigned int			GetNumSmokeBombs(void)						{ return m_numSmokeBomb; }
	float					GetHookLevel(void)							{ return m_Hook; }
	float					GetSwordLevel(void)							{ return m_Sword; }
	float					GetTimer(void)								{ return m_timer; }
	bool					GetStealth(void)							{ return m_stealth; }
	float					GetSoundIntensity(void)						{ return m_soundIntensity; }

	void					SetHavePizza(bool newHaveP)					{ m_HavePizza = newHaveP; }
	void					SetHaveHook(bool newHaveH)					{ m_HaveHook = newHaveH; }
	void					SetHaveSword(bool newHaveS)					{ m_HaveSword = newHaveS; }
	void					SetHookActive(bool hooK)					{ m_IsHookActive = hooK; }
	void					SetSwordActive(bool sword)					{ m_IsSwordActive = sword; }
	void					SetDAQ(unsigned int DAQ)					{ m_DAQ = DAQ; }
	void					SetNumSmokeBombs(unsigned int smoke)		{ m_numSmokeBomb = smoke; }
	void					SetHookLevel(float newHook)					{ m_Hook = newHook; }
	void					SetSwordLevel(float newSword)				{ m_Sword = newSword; }
	void					SetTimer(float newTimer)					{ m_timer = newTimer; }
	void					SetCurrHealth(float newHealth);
	void					SetSoundIntensity(float newSound)			{ m_soundIntensity = newSound; }

	void					PlayPickupSound();

	virtual void HandleEvent(const SGD::Event* pEvent) override;
};