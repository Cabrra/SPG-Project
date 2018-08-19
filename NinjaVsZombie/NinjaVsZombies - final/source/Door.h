#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "GamePlayState.h"
#include "Entity.h"
#include "AnimationManager.h"
#include "TimeStamp.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"

class Door : public Entity, SGD::Listener
{
private:
	bool m_bActive;
	bool m_bVertical;
	bool m_bOpen = false;
	bool m_bClose = false;
	bool m_bUnlock = false;
	bool m_bUnlocked = false;


public:
	TimeStamp ts1;
	TimeStamp ts2;
	TimeStamp ts3;

	Door(bool vertical);
	virtual ~Door();

	bool GetUnlocked()const { return m_bUnlocked; }
	void SetUnlocked(bool active) { m_bUnlocked = active; }

	bool GetActive()const { return m_bActive; }
	void SetActive(bool active) { m_bActive = active; }

	bool IsVertical()const { return m_bVertical; }
	void SetVertical(bool active) { m_bVertical = active; }

	virtual int		GetType(void)	const			override	{ return ENT_DOOR; }

	virtual void	Update(float elapsedTime)		override;
	virtual void	Render(void)					override;
	virtual SGD::Rectangle GetRect(void)	const			override;
	virtual void	HandleCollision(const IEntity* pOther)	override;
	virtual void						HandleEvent(const SGD::Event* pEvent) override;
};

