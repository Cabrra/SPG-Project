#pragma once
#include "Enemy.h"
#include "TimeStamp.h"
#include "../SGD Wrappers/SGD_Listener.h"
class Pride : public Enemy, public SGD::Listener
{
	int m_numTrapsTapsTaken = 0;
	bool m_taunt = false;
	bool m_takeaknee = false;
	float m_taunttimer = 0;
	float m_kneetimer = 0;
	float m_attacktimer = 0;
	float m_deathtimer = 0;
	//when we get sprite sheet 
	// animations
	TimeStamp m_Idle;
	TimeStamp m_Walking;
	TimeStamp m_Dying;
	TimeStamp m_Attacking;
	TimeStamp m_Taunting;
	TimeStamp m_TakeaKnee;
	bool left = true;
	bool right = false;
	bool m_att = false;
	bool m_move = false;
	bool m_dead = false;
	bool flipped = false;

public:
	Pride ();
	~Pride ();
	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;
	virtual void			HandleCollision(const IEntity* pOther)	override;
	virtual void			HandleEvent(const SGD::Event* pEvent) override;

	virtual int				GetType(void)	const			override	{ return ENT_BOSS; }
	void					SetWalkingAnimation(TimeStamp walk)				{ m_Walking = walk; }
	void					SetDyingAnimation(TimeStamp death)				{ m_Dying = death; }
	void					SetTauntingAnimation(TimeStamp taunt)				{ m_Taunting = taunt; }
	void					SetAttackingAnimation(TimeStamp attack)				{ m_Attacking = attack; }
	void					SetIdleAnimation(TimeStamp idle)				{ m_Idle = idle; }
	void					SetKneeAnimation(TimeStamp knee)				{ m_TakeaKnee = knee; }




};
