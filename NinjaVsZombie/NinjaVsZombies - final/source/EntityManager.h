/***************************************************************
|	File:		EntityManager.h
|	Author:		Jagoba Marcos
|	Course:		 
|	Purpose:	EntityManager class stores & maintains all game entities
***************************************************************/

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H


class IEntity;			// uses IEntity*
#include <vector>		// uses std::vector
#include "../SGD Wrappers/SGD_Declarations.h"

class Map;
class Entity;
class Room;
/**************************************************************/
// EntityManager class
//	- stores references to game entities
//	- updates & renders all game entities
class EntityManager
{
public:
	/**********************************************************/
	// Default constructor & destructor
	EntityManager( void )	= default;
	~EntityManager( void )	= default;
	

	/**********************************************************/
	// Entity Storage:
	void	AddEntity	( IEntity* pEntity, unsigned int bucket );
	void	RemoveEntity( IEntity* pEntity, unsigned int bucket );
	void	RemoveEntity( IEntity* pEntity );
	void	RemoveAll	( unsigned int bucket );
	void	RemoveAll	( void );
	

	/**********************************************************/
	// Entity Upkeep:
	void	UpdateAll	( float elapsedTime );
	void	RenderAll	( void );

	void	CheckCollisions( unsigned int bucket1, unsigned int bucket2 );
	void	CheckCollisionWithTiles(Room* room, unsigned int bucket);
	bool	IsGoingToCollide(SGD::Point point, Entity* unit, unsigned int bucket);

private:
	/**********************************************************/
	// Not a singleton, but still don't want the Trilogy-of-Evil
	EntityManager( const EntityManager& )				= delete;
	EntityManager& operator= ( const EntityManager& )	= delete;

	
	/**********************************************************/
	// Typedefs will simplify the templates
	typedef std::vector< IEntity* >		EntityVector;
	typedef std::vector< EntityVector >	EntityTable;
	
	/**********************************************************/
	// members:
	EntityTable	m_tEntities;			// vector-of-vector-of-IEntity* (2D table)
	bool		m_bIterating = false;	// read/write lock
};

#endif //ENTITYMANAGER_H
