#pragma once
#include "Unbreakable.h"
#include "Tile.h"

class Lever : public Unbreakable
{
private:
	bool m_bIsOn = false;
	Entity* m_pObjToActivate;
	SGD::HAudio  m_hSound = SGD::INVALID_HANDLE;
	SGD::HAudio  m_hUnSound = SGD::INVALID_HANDLE;
public:
	Lever();
	~Lever();

	Entity* GetObjectToActivate()const { return m_pObjToActivate; }
	void SetObjectToActivate(Entity* obj) { m_pObjToActivate = obj; }

	void TurnOn();
	void TurnOff();
	void Switch();

	virtual void			Update(float elapsedTime)		override;
	virtual void			Render(void)					override;
	virtual SGD::Rectangle	GetRect(void)	const			override;
	virtual int				GetType(void)	const			override	{ return ENT_LEVER; }
	void					HandleCollision(const IEntity* pOther);

	bool				GetActive()const				{ return m_bIsOn; }

	void				SetActive(bool newActive)		{ m_bIsOn = newActive; }
	void				PlayOnSound();
	void				PlayOffSound();

};