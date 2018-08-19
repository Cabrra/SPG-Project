#pragma once
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Map.h"
#include "SpawnPoint.h"
#include "Trigger.h"
#include "Room.h"
#include "Puzzle.h"
#include "Challenge.h"

class LevelManager
{
private:
	Map* m_pLevel;
	std::vector<Room*> m_vRooms;
	//std::vector<Puzzle*> m_vPuzzles;
	//std::vector<Challenge*> m_vChallenges;
	SGD::Point m_rEndPoint = {0,0};
	//std::vector<SpawnPoint*> m_vSpawns;
	//std::vector<Trigger*> m_vTriggers;
	//SGD::Point m_ptRespawn;
	SGD::HTexture m_hStairImg;

public:
	LevelManager();
	~LevelManager();

	bool LoadFile(const char* file);

	void Update(float elapsedTime);

	void RenderEndPoint();

	Map* GetMap()const { return m_pLevel; }
	std::vector<Room*> GetRooms()const { return m_vRooms; }
	//std::vector<Puzzle*> GetPuzzles()const { return m_vPuzzles; }
	//std::vector<Challenge*> GetChallenges()const { return m_vChallenges; }
	SGD::Point GetEndPoint()const { return m_rEndPoint; }
	void SetEndPoint(SGD::Point rect) { m_rEndPoint = rect; }
};
