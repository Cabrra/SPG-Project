#include "Room.h"
#include "SpawnPoint.h"
#include "Tile.h"
#include "Trigger.h"
#include "Unit.h"
#include "IEntity.h"

Room::Room()
{
}


Room::~Room()
{
	for (unsigned int s = 0; s < m_vSpawns.size(); s++)
	{
		delete m_vSpawns[s];
		m_vSpawns[s] = nullptr;
	}
	m_vSpawns.clear();

	for (unsigned int t = 0; t < m_vTriggers.size(); t++)
	{
	delete m_vTriggers[t];
	m_vSpawns[t] = nullptr;
	}
	m_vTriggers.clear();
}

SGD::Rectangle Room::GetRect() const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Room::Update(float elapsedTime)
{
	for (unsigned int x = 0; x < m_vSpawns.size(); x++)
		m_vSpawns[x]->Update(elapsedTime);
	
	/*for (unsigned int i = 0; i < m_vTiles.size(); i++)
		m_vTiles[i]->Update(elapsedTime);*/
}

void Room::AddSpawnPoint(SpawnPoint* sp)
{
	m_vSpawns.push_back(sp);
}

void Room::AddEscapepoint(SGD::Point ep)
{
	m_vEscapes.push_back(ep);
}

void Room::AddTile(Tile* tile)
{
	m_vTiles.push_back(tile);
}

void Room::CheckCollision(const IEntity* pOther)
{
	for (unsigned int num = 0; num < m_vTiles.size(); num++)
	{
		if (pOther != nullptr)
		{
			if (m_vTiles[num]->GetRect().IsPointInRectangle(pOther->GetRect().ComputeCenter()))
				m_vTiles[num]->HandleCollision(pOther);
		}
	}
}

bool Room::CheckCollision(SGD::Point point, const IEntity* pOther)
{
	Entity* ply = (Entity*)pOther;
	SGD::Rectangle rect = { point, ply->GetSize() };
	for (unsigned int num = 0; num < m_vTiles.size(); num++)
	{
		if (m_vTiles[num]->GetRect().IsIntersecting(rect) == true && m_vTiles[num]->GetType() == Tile::m_eTiletype::collidable)
			return true;
	}
	return false;
}