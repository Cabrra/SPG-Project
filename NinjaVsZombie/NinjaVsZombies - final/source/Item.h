#pragma once
#include "Entity.h"
class Item: public Entity
{
public:
	Item();
	~Item();
	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_BASE; }
	virtual SGD::Rectangle GetRect(void)	const			override;
	virtual void	HandleCollision(const IEntity* pOther)	override;
};