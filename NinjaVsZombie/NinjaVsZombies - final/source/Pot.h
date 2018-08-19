#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_Handle.h"		// HTexture type
class Pot:
	public Entity
{
public:
	Pot(void) = default;	// default constructor
	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;

	virtual int		GetType(void)	const			override	{ return ENT_BREAKABLE; }
	virtual SGD::Rectangle GetRect(void)	const			override;
	virtual void	HandleCollision(const IEntity* pOther)	override;

protected:
	virtual ~Pot(void);			// VIRTUAL destructor
	/**********************************************************/

private:
	/**********************************************************/
	// reference count
	unsigned int		m_unRefCount = 1;	// calling 'new' gives the prime reference
};

