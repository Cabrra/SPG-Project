#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include "TimeStamp.h"
class LustWrath :
	public Enemy, SGD::Listener
{
public:
	enum m_eState{IDLE,APPROACH,FLEE,ATTACK,FLIRT,DEAD};

private:
	bool m_bHeartActive = false;
	bool m_bAttacked = false;
	float m_fKissTimer = 0.0f;
	float m_fAttackTimer = 0.0f;
	SGD::Point m_ptFleePos = {0,0};
	bool m_bWrath = false;
	m_eState m_CurrState = IDLE;
	float m_fDeathTimer = 0.0f;
	float m_fIdleTimer = 0.0f;
	float m_fMoveTimer = 0.0f;
	SGD::Point m_ptControlPos = { 0, 0 };

	// animations
	TimeStamp m_Idle;
	TimeStamp m_Walking;
	TimeStamp m_Attacking;
	TimeStamp m_Kissing;
	//TimeStamp m_Dying;

	bool flipped = false;
	bool m_att = false;
	bool m_kiss = false;
	bool m_move = false;
	bool m_idle = false;
	bool m_dead = false;


public:
	LustWrath();
	virtual ~LustWrath();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;
	virtual void			HandleCollision(const IEntity* pOther)	override;
	virtual void			HandleEvent(const SGD::Event* pEvent) override;
	virtual SGD::Rectangle				GetRect(void)	const			override;

	virtual int				GetType(void)	const			override	{ return ENT_LUST; }

	void Flirt(float elapsedTime);
	void Approach(float elapsedTime);
	void Flee(float elapsedTime);
	void Idle(float elapsedTime);
	void Attack(float elapsedTime);

	bool GetWrath()const { return m_bWrath; }
	bool GetHeartActive()const { return m_bHeartActive; }
	float GetKissTimer()const { return m_fKissTimer;}
	float GetAttackTimer()const { return m_fAttackTimer; }
	float GetIdleTimer()const { return m_fIdleTimer; }
	float GetDeathTimer()const { return m_fDeathTimer; }
	float GetMoveTimer()const { return m_fMoveTimer; }
	SGD::Point GetFleePos()const { return m_ptFleePos; }
	m_eState GetCurrState()const { return m_CurrState; }

	void SetWrath(bool mad) { m_bWrath = mad; }
	void SetHeartActive(bool kiss) { m_bHeartActive = kiss; }
	void SetKissTimer(float time) { m_fKissTimer = time; }
	void SetAttackTimer(float time) { m_fAttackTimer = time; }
	void SetDeathTimer(float time) { m_fDeathTimer = time; }
	void SetIdleTimer(float time) { m_fIdleTimer = time; }
	void SetMoveTimer(float time) { m_fMoveTimer = time; }
	void SetFleePos(SGD::Point pos) { m_ptFleePos = pos; }
	void SetCurrState(m_eState state) { m_CurrState = state; }

	void					SetWalkingAnimation(TimeStamp walk)				{ m_Walking = walk; }
	//void					SetDyingAnimation(TimeStamp death)				{ m_Dying = death; }
	void					SetKissingAnimation(TimeStamp kiss)				{ m_Kissing = kiss; }
	void					SetAttackingAnimation(TimeStamp attack)			{ m_Attacking = attack; }
	void					SetIdleAnimation(TimeStamp idle)				{ m_Idle = idle; }

};

