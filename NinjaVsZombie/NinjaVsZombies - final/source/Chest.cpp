#include "Chest.h"


#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>


/**************************************************************/
// DESTRUCTOR
/*virtual*/ Chest::~Chest(void)
{
	assert(m_unRefCount == 0 && "~Entity - entity was destroyed while references remain!");
}


/**************************************************************/
// Update
//	- move the entity's position by its velocity
/*virtual*/ void Chest::Update(float elapsedTime)
{

}


/**************************************************************/
// Render
//	- draw the entity's image at its position
/*virtual*/ void Chest::Render(void)
{

}


/**************************************************************/
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void Chest::HandleCollision(const IEntity* Chesther)
{

}


/**************************************************************/
// GetRect
//	- calculate the entity's bounding rectangle
/*virtual*/ SGD::Rectangle Chest::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}