#pragma once
#include "../SGD Wrappers/SGD_Handle.h"
#include "IEntity.h"

class Tile : public IEntity
{
protected:
	SGD::HTexture		m_hImage = SGD::INVALID_HANDLE;	// image handle
	SGD::Point			m_ptPosition = SGD::Point{ 0, 0 };		// 2D position
	SGD::Vector			m_vtVelocity = SGD::Vector{ 0, 0 };	// 2D velocity
	SGD::Size			m_szSize = SGD::Size{ 0, 0 };		// 2D size
	float				m_fRotation = 0.0f;					// angle in radians
	bool				isDead = false;

public:
	enum m_eTiletype{ walkable = 1, collidable, hole, trigger };
	int m_nID;//place of the tile in the tileset
	m_eTiletype type;	//type of tile
	bool isTriggered = false;

	Tile();
	virtual ~Tile();

	virtual void	Update			( float elapsedTime )		override;
	virtual void	Render			( void )					override;
	virtual int		GetType(void)	const override { return type; }
	virtual SGD::Rectangle GetRect	( void )	const			override;
	virtual void	HandleCollision	( const IEntity* pOther )	override;

	void HandleTrigger();//handles trigger events for trigger tiles

	SGD::HTexture		GetImage(void) const			{ return m_hImage; }
	SGD::Point			GetPosition(void) const			{ return m_ptPosition; }
	SGD::Vector			GetVelocity(void) const			{ return m_vtVelocity; }
	SGD::Size			GetSize(void) const			{ return m_szSize; }
	float				GetRotation(void) const			{ return m_fRotation; }
	virtual bool		GetIsDead()const	override		{ return isDead; }

	// Mutators:
	void				SetImage(SGD::HTexture	img)	{ m_hImage = img; }
	void				SetPosition(SGD::Point	pos) 	{ m_ptPosition = pos; }
	void				SetVelocity(SGD::Vector	vel) 	{ m_vtVelocity = vel; }
	void				SetSize(SGD::Size		size) 	{ m_szSize = size; }
	void				SetRotation(float			rad)	{ m_fRotation = rad; }


	//leave empty
	virtual void	AddRef(void) override;
	virtual void	Release(void) override;
};

