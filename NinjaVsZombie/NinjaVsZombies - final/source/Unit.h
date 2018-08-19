#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Entity.h"

/**************************************************************/
// Forward class declaration
//	- tells the compiler that the type exists
//	- allows us to create pointers or references
class SpawnPoint;
class AnchorPointAnimation;


class Unit:public Entity
{
	int						m_maxHealth = 100;
	int						m_currHealth = 100;
	int						m_moveSpeed = 100;
	int						m_Strength = 5;
	bool					m_IsFalling = false;
	SpawnPoint*				m_SpawnP = nullptr;
	SGD::HAudio				m_takeDamageSound = SGD::INVALID_HANDLE;
	SGD::HAudio				m_deathSound = SGD::INVALID_HANDLE;
	SGD::HAudio				m_attackSound = SGD::INVALID_HANDLE;
	SGD::HAudio				m_StealSound = SGD::INVALID_HANDLE;
	AnchorPointAnimation*	m_DeathAnim = nullptr;
	AnchorPointAnimation*	m_AttackAnim = nullptr;
	AnchorPointAnimation*	m_TakeDamAnim = nullptr;
	AnchorPointAnimation*	m_WalkAnim = nullptr;


public:
	Unit() = default;
	~Unit() = default;

	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_UNIT; }
	virtual void			HandleCollision(const IEntity* pOther)	override;

	int				GetMaxHP(void)const					{ return m_maxHealth; }
	int				GetCurrHP(void)const				{ return m_currHealth; }
	int				GetMoveSpeed(void)const				{ return m_moveSpeed; }
	int				GetStrength(void)const				{ return m_Strength; }
	SpawnPoint*		GetSpawnPoint(void)const			{ return m_SpawnP; }
	SGD::HAudio		GetDamageSound(void)const			{ return m_takeDamageSound; }
	SGD::HAudio		GetDeathSound(void)const			{ return m_deathSound; }
	SGD::HAudio		GetAttackSound(void)const			{ return m_attackSound; }
	SGD::HAudio		GetStealSound(void)const			{ return m_StealSound; }

	void			SetMaxHP(int newMaxHp)					{ m_maxHealth = newMaxHp; }
	void			SetCurrHP(int newCurrHp);
	void			SetSpawnPoint(SpawnPoint* newSpawn)		{ m_SpawnP = newSpawn; }
	void			SetMoveSpeed(int newMove)				{ m_moveSpeed = newMove; }
	void			SetStrength(int newStrength)			{ m_Strength = newStrength; }
	void			SetDamageSound(SGD::HAudio newSound)	{ m_takeDamageSound = newSound; }
	void			SetDeathSound(SGD::HAudio newSound)		{ m_deathSound = newSound; }
	void			SetAttackSound(SGD::HAudio newSound)	{ m_attackSound = newSound; }
	void			SetStealSound(SGD::HAudio newSound)		{ m_StealSound = newSound; }

	virtual bool			CheckCollision(SGD::Point point);
	virtual bool			CheckBossCollision(SGD::Point point);

	
};