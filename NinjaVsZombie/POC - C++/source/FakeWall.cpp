#include "FakeWall.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>


/**************************************************************/
// DESTRUCTOR
/*virtual*/ FakeWall::~FakeWall(void)
{
	assert(m_unRefCount == 0 && "~Entity - entity was destroyed while references remain!");
}


/**************************************************************/
// Update
//	- move the entity's position by its velocity
/*virtual*/ void FakeWall::Update(float elapsedTime)
{

}


/**************************************************************/
// Render
//	- draw the entity's image at its position
/*virtual*/ void FakeWall::Render(void)
{

}


/**************************************************************/
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void FakeWall::HandleCollision(const IEntity* FakeWallher)
{

}

/**************************************************************/
// GetRect
//	- calculate the entity's bounding rectangle
/*virtual*/ SGD::Rectangle FakeWall::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}