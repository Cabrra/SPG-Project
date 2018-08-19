#pragma once

#include "Lever.h"
#include <vector>
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
using namespace std;

class Lever;
class PressurePoint;

class Puzzle:
	public Unbreakable, public SGD::Listener
{
private:
	std::vector<PressurePoint*>			m_PressurePointList;
	std::vector<Lever*>					m_LeverList;

	std::vector<PressurePoint*>			m_CurrentPressurePointList;
	std::vector<Lever*>					m_CurrentLeverList;

	float								m_Timer = 0.0f;
	float								m_CurrTime = 0.0f;
	bool								m_IsSolved = false;
	bool								m_HasOrder = false;
	bool								m_bRestart = false;
	bool								m_isBeenRestarted = false;

	SGD::HAudio dooropensnd = SGD::INVALID_HANDLE;
	SGD::HAudio doorclosesnd = SGD::INVALID_HANDLE;

	std::vector<Entity*>					m_lockedVector;
public:
	Puzzle(std::vector<Entity*>	 locker = {}, float timer = 0.0f, bool restart = false, std::vector<PressurePoint*> pressureList = {},
		std::vector<Lever*> leverList = {}, bool order = false);
	~Puzzle();

	virtual void			Update(float elapsedTime)		override;
	virtual int				GetType(void)	const			override	{ return ENT_PUZZLE; }
	virtual void			Render()						override;
	virtual void			HandleEvent(const SGD::Event* pEvent) override;
	
	bool					CheckLevers();
	bool					CheckPressurePoints();
	int						CheckOrderedLevers();
	int						CheckOrderedPressurePoints();


	std::vector<PressurePoint*>			GetPressurePointList			()const {return m_PressurePointList;}
	std::vector<Lever*>					GetLeverList					()const {return m_LeverList;}
	std::vector<PressurePoint*>			GetCurrentPressurePointList		()const { return m_CurrentPressurePointList; }
	std::vector<Lever*>					GetCurrentLeverList				()const { return m_CurrentLeverList; }
	float								GetTimer						()const {return m_Timer;}
	float								GetCurrTime						()const {return m_CurrTime;}
	bool								GetIsSolved						()const {return m_IsSolved;}
	bool								GetHasOrder						()const {return m_HasOrder;}
	std::vector<Entity*>				GetLocker()const	{ return m_lockedVector; }
	bool								GetRestart						()const { return m_bRestart; }

	void								SetTimer(float newTimer) { m_Timer = newTimer; }
	void								SetCurrTime (float newCurr) { m_CurrTime; }
	void								SetIsSolved(bool newSol) { m_IsSolved = newSol; }
	void								SetHasOrder(bool newOrder) { m_HasOrder = newOrder; }
	void								AddLocker(Entity* newLocker);
	void								SetRestart(bool restart) { m_bRestart = restart; }
};

