#include "Pot.h"


#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>


/**************************************************************/
// DESTRUCTOR
/*virtual*/ Pot::~Pot(void)
{
	assert(m_unRefCount == 0 && "~Entity - entity was destroyed while references remain!");
}


/**************************************************************/
// Update
//	- move the entity's position by its velocity
/*virtual*/ void Pot::Update(float elapsedTime)
{

}


/**************************************************************/
// Render
//	- draw the entity's image at its position
/*virtual*/ void Pot::Render(void)
{

}


/**************************************************************/
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void Pot::HandleCollision(const IEntity* pOther)
{

}

/**************************************************************/
// GetRect
//	- calculate the entity's bounding rectangle
/*virtual*/ SGD::Rectangle Pot::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}
