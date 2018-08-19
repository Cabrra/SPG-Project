#pragma once
#include "Enemy.h"
#include "TimeStamp.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"

#include <vector>

class Zombie : public Enemy, public SGD::Listener
{
private:
	AIState						m_myState		= AI_PATROL;
	float						m_StateTimer	= 0.0f;
	float						m_fcountinc		= 0.0f;
	float						m_stunTimer		= 0.0f;
	std::vector<SGD::Point>		m_Waypoints;
	SGD::Point					m_EscapePoint	= { 0, 0 };
	unsigned int				m_currPoint;
	int							m_countTobeat	= 10;
	bool						fighting		= false;
	bool						showfightbar	= false;
	bool						m_Armored		= false;
	bool						m_IsHooked      = false;
	bool						m_Won			= false;
	bool						m_movedPlayer	= false;

	SGD::Point					m_LastKnownPoint;

	SGD::HTexture				m_Armoredpic	= SGD::INVALID_HANDLE;

	// animations
	TimeStamp walkleft;
	TimeStamp walkdown;
	TimeStamp walkup;
	TimeStamp attackleft;
	TimeStamp attackdown;
	TimeStamp attackup;
	TimeStamp stealleft;
	TimeStamp stealdown;
	TimeStamp stealup;
	TimeStamp death;
	bool left = true;
	bool up = false;
	bool down = false;
	bool right = false;
	bool walking = true;
	bool attacking = false;
	bool stealing = false;
	bool dying = false;


	SGD::HAudio					m_hit = SGD::INVALID_HANDLE;
	SGD::HAudio					m_death = SGD::INVALID_HANDLE;
	SGD::HAudio					m_attack = SGD::INVALID_HANDLE;
	SGD::HAudio					m_alarmed = SGD::INVALID_HANDLE;
	bool						playonce = false;

public:
	Zombie(bool armor);
	~Zombie();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	void					UpdateIdle(float elapsedTime);
	void					UpdateAttack(float elapsedTime);
	void					UpdateAlert(float elapsedTime);
	void					UpdateCaution(float elapsedTime);
	void					UpdatePatrol(float elapsedTime);
	void					UpdateEscape(float elapsedTime);

	virtual int				GetType(void)	const			override	{ return ENT_ZOMBIE; }
	int						GetCountToBeat() const						{ return m_countTobeat; }
	void					SetCountToBeat(int newBeat)					{ m_countTobeat = newBeat; }
	virtual void			HandleCollision(const IEntity* pOther)	override;

	void					AddWayPoint(SGD::Point newPoint){ m_Waypoints.push_back(newPoint); }
	virtual void			HandleEvent(const SGD::Event* pEvent) override;

	void					CreateDrop();
	bool					IsArmored()						{ return m_Armored; }
	bool					IsHooked()						{ return m_IsHooked; }
	bool					HasWon()						{ return m_Won; }

	void					SetIsArmored(bool armor)		{ m_Armored = armor; }
	void					SetIsHooked(bool hook)			{ m_IsHooked = hook; }
	void					SetStateTime(float newTime)		{ m_StateTimer = newTime; }
	void					SetState(AIState newState)		{ m_myState = newState; }
	void					SetHasWon(bool won)				{ m_Won = won; }
	void					SetmovedPlayer(bool move)		{ m_movedPlayer = move; }


	void SetWalkLeft(TimeStamp left) { walkleft = left; }
	void SetWalkDown(TimeStamp down) { walkdown = down; }
	void SetWalkUp(TimeStamp up) { walkup = up; }
	void SetAttackLeft(TimeStamp left) { attackleft = left; }
	void SetAttackDown(TimeStamp down) { attackdown = down; }
	void SetAttackUp(TimeStamp up) { attackup = up; }
	void SetStealLeft(TimeStamp left) { stealleft = left; }
	void SetStealDown(TimeStamp down) { stealdown = down; }
	void SetStealUp(TimeStamp up) { stealup = up; }
	void SetDeath(TimeStamp dead) { death = dead; }

	bool EscapeCollision(SGD::Point point);

};