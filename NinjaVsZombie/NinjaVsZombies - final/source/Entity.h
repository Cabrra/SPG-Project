/***************************************************************
|	File:		Entity.h
|	Author:		Jagoba Macos
|	Course:		
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#ifndef ENTITY_H
#define ENTITY_H

class Emiter;

#include "IEntity.h"						// IEntity type
#include "../SGD Wrappers/SGD_Handle.h"		// HTexture type

/**************************************************************/
// Entity class
//	- parent class of all game entities, stores the shared data members
//	- velocity-based movement
class Entity : public IEntity
{
	/**********************************************************/
	// MUST have a virtual destructor to allow upcasted pointers
	// to trigger children destructors at deallocation


	bool						m_HavePizza = false;


public:
	/**********************************************************/
	// Entity Types:
	enum EntityType 
	{
		ENT_BASE, ENT_UNIT,
		ENT_PLAYER, ENT_ZOMBIE,
		ENT_NINJA, ENT_AMBUSHNINJA,
		ENT_ARMOREDZOMBIE, ENT_BOSS,
		ENT_SHURIKEN, ENT_ARROW, ENT_ENEMY,
		ENT_BREAKABLE, ENT_UNBREAKABLE, 
		ENT_MOVABLE, ENT_IMMOVABLE, ENT_TILE,
		ENT_ANIMATION, ENT_BULLET,
		ENT_HOOK, ENT_SWORD, ENT_POWERUP,
		ENT_PIZZA, ENT_SMOKEBOMB,ENT_LEVER,
		ENT_TRAP,ENT_FIRETRAP,ENT_ELECTRICTRAP,
		ENT_SPIKETRAP,ENT_FAKETRAP,ENT_NEUTRAL_ENEMY,
		ENT_DOOR, ENT_FAKE_WALL, ENT_FAKE_FLOOR, ENT_PUZZLE,
		ENT_PRESSUREPOINT, ENT_SPAWNTRAP,ENT_LUST,
		ENT_CHALLENGE,ENT_HEART,
	};

	Entity( void )			= default;	// default constructor
	/**********************************************************/
	// Interface:
	//	- virtual functions for children classes to override
	virtual void	Update			( float elapsedTime )		override;
	virtual void	Render			( void )					override;

	virtual int		GetType			( void )	const			override	{	return ENT_BASE;	}
	virtual SGD::Rectangle GetRect	( void )	const			override;
	virtual void	HandleCollision	( const IEntity* pOther )	override;
	

	// Children classes CANNOT override a 'final' method.
	virtual void	AddRef			( void )					final;
	virtual void	Release			( void )					final;

	
	/**********************************************************/
	// Accessors:
	SGD::HTexture		GetImage	( void ) const			{	return m_hImage;		}
	SGD::Point			GetPosition	( void ) const			{	return m_ptPosition;	}
	SGD::Point			GetPositionInWorldSpace(void) const;
		
	SGD::Vector			GetVelocity	( void ) const			{	return m_vtVelocity;	}
	SGD::Size			GetSize(void) const					{	return m_szSize;		}
	SGD::Size			GetScale(void) const				{	return m_Scale;			}
	float				GetRotation	( void ) const			{	return m_fRotation;		}
	virtual bool		GetIsDead()const	override		{	return isDead;			}
	bool				GetHavePizza()const					{	return m_HavePizza;		}
 	bool				IsFalling()							{	return m_Isfalling;		}

	Emiter*				GetEmiter()const					{ return m_ParticleEf; }
	// Mutators:
	void				SetImage	( SGD::HTexture	img  )	{	m_hImage		= img;	}
	void				SetPosition	( SGD::Point	pos  ) 	{	m_ptPosition	= pos;	}
	void				SetVelocity	( SGD::Vector	vel	 ) 	{	m_vtVelocity	= vel;	}
	void				SetSize		( SGD::Size		size ) 	{	m_szSize		= size;	}
	void				SetScale	( SGD::Size		scale)	{	m_Scale			= scale;}
	void				SetRotation ( float			rad  )	{	m_fRotation		= rad;	}
	void				SetFalling	( bool			fall )	{	m_Isfalling		= fall; }
	void				SetEmiter(Emiter* newEmiter);
	void				SetHavePizza(bool			newPizza)			{ m_HavePizza = newPizza; }
protected:
	virtual ~Entity( void );			// VIRTUAL destructor
	/**********************************************************/
	// members:
	SGD::HTexture		m_hImage		= SGD::INVALID_HANDLE;	// image handle
	SGD::Point			m_ptPosition	= SGD::Point{0, 0};		// 2D position
	SGD::Vector			m_vtVelocity	= SGD::Vector{0, 0};	// 2D velocity
	SGD::Size			m_szSize		= SGD::Size{0, 0};		// 2D size
	SGD::Size			m_Scale			= { 1.0f, 1.0f };
	float				m_fRotation		= 0.0f;					// angle in radians
	bool				isDead			= false;
	bool				m_Isfalling		= false;

	Emiter*					m_ParticleEf = nullptr;
private:
	/**********************************************************/
	// reference count
	unsigned int		m_unRefCount	= 1;	// calling 'new' gives the prime reference
};

#endif //ENTITY_H