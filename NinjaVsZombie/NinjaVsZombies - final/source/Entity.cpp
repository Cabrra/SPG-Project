/***************************************************************
|	File:		Entity.cpp
|	Author:		Jagoba Marcos
|	Course:		1406
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#include "Entity.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>
#include "Emiter.h"
#include "Game.h"


/**************************************************************/
// DESTRUCTOR
/*virtual*/ Entity::~Entity( void )
{
	assert( m_unRefCount == 0 && "~Entity - entity was destroyed while references remain!" );
	if (m_ParticleEf != nullptr)
		m_ParticleEf->Release();
}


/**************************************************************/
// Update
//	- move the entity's position by its velocity
/*virtual*/ void Entity::Update( float elapsedTime )
{
	m_ptPosition += m_vtVelocity * elapsedTime;
}


/**************************************************************/
// Render
//	- draw the entity's image at its position
/*virtual*/ void Entity::Render( void )
{
	// Verify the image
	assert( m_hImage != SGD::INVALID_HANDLE && "Entity::Render - image was not set!" );
	

	// Draw the image
	SGD::GraphicsManager::GetInstance()->DrawTexture( 
		m_hImage, m_ptPosition,
		m_fRotation, m_szSize/2 );
}


/**************************************************************/
// GetRect
//	- calculate the entity's bounding rectangle
/*virtual*/ SGD::Rectangle Entity::GetRect( void ) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}


/**************************************************************/
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void Entity::HandleCollision( const IEntity* pOther )
{
	/* DO NOTHING */
	(void)pOther;		// unused parameter
}


/**************************************************************/
// AddRef
//	- increase the reference count
/*virtual*/ void Entity::AddRef( void )
{
	assert( m_unRefCount != 0xFFFFFFFF && "Entity::AddRef - maximum reference count has been exceeded" );

	Entity* th = this;
	++m_unRefCount;
}


/**************************************************************/
// Release
//	- decrease the reference count
//	- self-destruct when the count is 0
/*virtual*/ void Entity::Release( void )
{
	--m_unRefCount;

	if( m_unRefCount == 0 )
		delete this;
}

SGD::Point Entity::GetPositionInWorldSpace() const
{
	return{ m_ptPosition.x + Game::GetInstance()->GetWorldSpace().x, m_ptPosition.y + Game::GetInstance()->GetWorldSpace().y };
}

void Entity::SetEmiter(Emiter* newEmiter)
{
	if (m_ParticleEf != nullptr)
	{
		m_ParticleEf->Release();
		m_ParticleEf = nullptr;
	}

	m_ParticleEf = newEmiter;
	if (m_ParticleEf != nullptr)
		m_ParticleEf->AddRef();
}