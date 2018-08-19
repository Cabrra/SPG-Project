#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include <vector>

class Tile;
class Trigger;
class SpawnPoint;
class IEntity;

class Room
{
private:
	SGD::Point m_ptPosition = SGD::Point{ 0, 0 };
	SGD::Size m_szSize = SGD::Size{ 0, 0 };

	int m_nRoomNum = 0;
	bool m_bUnlocked = false;

	SGD::Point m_ptRespawnPos = SGD::Point{ -1, -1 };
	std::vector<Tile*> m_vTiles;
	std::vector<SGD::Point> m_vEscapes;
	std::vector<SpawnPoint*> m_vSpawns;
	std::vector<Trigger*> m_vTriggers;

	bool m_bPlayerHere = false;
	Room* m_pRoomNeighbors[4];


public:
	enum m_eNeighborDir{Left,Right,Top,Bottom};
	Room();
	~Room();

	void Update(float elapsedTime);
	void AddSpawnPoint(SpawnPoint* sp);
	void AddEscapepoint(SGD::Point ep);
	void AddTile(Tile* tile);

	SGD::Rectangle GetRect()const;
	
	//accessors
	SGD::Point GetPosition()const { return m_ptPosition; }
	SGD::Size GetSize()const { return m_szSize; }
	int GetRoomNum()const { return m_nRoomNum; }
	bool GetUnlocked()const { return m_bUnlocked; }

	SGD::Point GetRespawnpoint()const { return m_ptRespawnPos; }
	std::vector<SpawnPoint*> GetSpawnpoints()const { return m_vSpawns; }
	std::vector<Trigger*> GetTriggers()const { return m_vTriggers; }
	std::vector<SGD::Point> GetEscapes()const { return m_vEscapes; }
	std::vector<Tile*> GetTiles()const { return m_vTiles; }

	bool IsPlayerHere()const { return m_bPlayerHere; }
	Room* GetNeighborLeft()const { return m_pRoomNeighbors[Left]; }
	Room* GetNeighborRight()const { return m_pRoomNeighbors[Right]; }
	Room* GetNeighborTop()const { return m_pRoomNeighbors[Top]; }
	Room* GetNeighborBottom()const { return m_pRoomNeighbors[Bottom]; }

	//mutators
	void SetPosition(SGD::Point pos){ m_ptPosition = pos; }
	void SetSize(SGD::Size size) { m_szSize = size; }
	void SetRoomNum(int num) { m_nRoomNum = num; }
	void SetUnlocked(bool set) { m_bUnlocked = set; }

	void SetRespawnpoint(SGD::Point pos) { m_ptRespawnPos = pos; }

	void SetPlayerHere(bool here){ m_bPlayerHere = here; }
	void SetNeighborLeft(Room* room){ m_pRoomNeighbors[Left] = room; }
	void SetNeighborRight(Room* room){ m_pRoomNeighbors[Right] = room; }
	void SetNeighborTop(Room* room){ m_pRoomNeighbors[Top] = room; }
	void SetNeighborBottom(Room* room){ m_pRoomNeighbors[Bottom] = room; }

	void CheckCollision(const IEntity* pOther); // checks collisions for all tiles in map
	bool CheckCollision(SGD::Point point, const IEntity* pOther); // checks collisions for all tiles in map by checking for a point
};