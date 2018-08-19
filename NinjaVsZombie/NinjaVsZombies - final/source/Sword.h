#pragma once
#include "Item.h"

#include <vector>

class Lever;
class Unit;
class Sword :
	public Item
{
	Entity*					m_owner				= nullptr;
	Unit*					m_attacked			= nullptr;
	Entity*					m_Lever				= nullptr;

	float					m_startRot			= 0.0f;
	float					m_endRot			= 0.0f;

	bool					m_IsSwinging		= false;

	int						m_Damage			= 0;

	SGD::HAudio				m_SlashSound		= SGD::INVALID_HANDLE;

	struct AttackedEnemy
	{
		Unit* unit;
		SGD::Point pos;
		int	DamTaken;
	};

	std::vector<AttackedEnemy>		m_attackedEnemies;
public:
	Sword(SGD::Point position, float rotation, Entity* owner);
	~Sword();

	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;

	virtual int				GetType(void)	const			override	{ return ENT_SWORD; }
	virtual void			HandleCollision(const IEntity* pOther)	override;

	void					Swing(float elapsedTime);

	/**********************************************************/
	// Accessors:
	Entity*					GetOwner(void)					{ return m_owner; }
	bool					GetIsSwinging(void)				{ return m_IsSwinging; }

	/**********************************************************/
	// Mutators:
	void					SetOwner(Entity* newOwner)		{ m_owner = newOwner; }
	void					SetIsSwinging(bool swing)		{ m_IsSwinging = swing; }

	int						GetDamage()						{ return m_Damage; }

	void					SetDamage(int dam)				{ m_Damage = dam; }
};