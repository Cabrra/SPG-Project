#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_Event.h"
#include <vector>
#include "Enemy.h"
class WayPoint;

class SpawnPoint : public SGD::Listener
{
public:
	enum m_eObjectType
	{
		NONE, PLAYER, ZOMBIE, A_ZOMBIE, RANGED_NINJA,
		AMBUSH_NINJA, MOVABLE_BOX, IMMOVABLE_BOX,
		PIZZA, HOOK, SWORD, SMOKE_BOMB, END_POINT,
		VDOOR, HDOOR, NEUTRAL_ENEMY,LEVER,PRESSUREPOINT,
		KINGPIN,LUST,PRIDE,
	};

private:
	float			m_SpawnTimer = 0.0f;
	float			m_CurrentTime = 0.0f;
	SGD::Point		m_Position = { 0, 0 };
	unsigned int	m_MaxEnemies = 0;
	unsigned int	m_SpawnedUnits = 0;
	bool			m_Active = false;
	bool			playerStart = true;
	float			m_RestartTimer = 20.0f;

	Enemy::AIState		mystate = Enemy::AI_PATROL;
	m_eObjectType m_Type = NONE;
	std::vector<SGD::Point> m_vWaypoints;

public:

	SpawnPoint();
	SpawnPoint(float spawntime, SGD::Point pos, unsigned int max, bool active, m_eObjectType mytype, bool playerstart,
		std::vector<SGD::Point> waypoints);
	~SpawnPoint();

	void AddWaypoint(SGD::Point pos);

	void Update(float elapsedTime);
	virtual void HandleEvent(const SGD::Event* pEvent) override;

	//GETTERS AND SETTERS
	float				GetSpawnTimer()const						{ return m_SpawnTimer; }
	SGD::Point			GetPosition()const							{ return m_Position; }
	float				GetCurrentTime()const						{ return m_CurrentTime; }
	unsigned int		GetMaxEnemies()const						{ return m_MaxEnemies; }
	unsigned int		GetSpawnedUnits()const						{ return m_SpawnedUnits; }
	m_eObjectType		GetType()const								{ return m_Type; }
	std::vector<SGD::Point>	GetWaypoints()const						{ return m_vWaypoints; }
	bool				GetActive()const							{ return m_Active; }

	void				SetSpawnTimer(float newTime)				{ m_SpawnTimer = newTime; }
	void				SetPosition(SGD::Point newPosi)				{ m_Position = newPosi; }
	void				SetCurrentTime(float newCurr)				{ m_CurrentTime = newCurr; }
	void				SetMaxEnemies(unsigned int newMax)			{ m_MaxEnemies = newMax; }
	void				SetType(m_eObjectType type)					{ m_Type = type; }
	void				SetSpawnedUnit(unsigned int spawn)			{ m_SpawnedUnits = spawn; }
	void				SetActive(bool newAct)						{ m_Active = newAct; }
	void				SetMyState(Enemy::AIState newState)			{ mystate = newState; }
};

