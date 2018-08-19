
#include "Breakable.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>


/**************************************************************/
// DESTRUCTOR
/*virtual*/ Breakable::~Breakable(void)
{
	assert(m_unRefCount >= 1 && "~Entity - entity was destroyed while references remain!");
}


/**************************************************************/
// Update
//	- move the entity's position by its velocity
/*virtual*/ void Breakable::Update(float elapsedTime)
{
	
}


/**************************************************************/
// Render
//	- draw the entity's image at its position
/*virtual*/ void Breakable::Render(void)
{
	
}


/**************************************************************/
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void Breakable::HandleCollision(const IEntity* pOther)
{

}

/**************************************************************/
// GetRect
//	- calculate the entity's bounding rectangle
/*virtual*/ SGD::Rectangle Breakable::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}