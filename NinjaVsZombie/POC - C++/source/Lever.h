#pragma once
#include "Entity.h"

class Lever
{
protected:
	SGD::HTexture		m_hImage = SGD::INVALID_HANDLE;	// image handle
	SGD::Point			m_ptPosition = SGD::Point{ 0, 0 };		// 2D position
	SGD::Vector			m_vtVelocity = SGD::Vector{ 0, 0 };	// 2D velocity
	SGD::Size			m_szSize = SGD::Size{ 0, 0 };		// 2D size

public:
	Lever();
	~Lever();

	Entity* GetObjectToActivate()const { return m_pObjToActivate; }

	void TurnOn();
	void TurnOff();

	void	Update(float elapsedTime);
	void	Render(void);
	SGD::Rectangle GetRect(void) const;
	void	HandleCollision(const IEntity* pOther);


	SGD::HTexture		GetImage(void) const			{ return m_hImage; }
	SGD::Point			GetPosition(void) const			{ return m_ptPosition; }
	SGD::Size			GetSize(void) const			{ return m_szSize; }

	// Mutators:
	void				SetImage(SGD::HTexture	img)	{ m_hImage = img; }
	void				SetPosition(SGD::Point	pos) 	{ m_ptPosition = pos; }
	void				SetSize(SGD::Size		size) 	{ m_szSize = size; }

private:
	Entity* m_pObjToActivate;
	bool isOn = false;

};

