#include "Map.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Entity.h"
#include "Player.h"
#include "GamePlayState.h"

Map::Map(int rows, int cols, int width, int height)
{
	m_nGridRows = rows;
	m_nGridCols = cols;
	m_nGridWidth = width;
	m_nGridHeight = height;

}


Map::~Map()
{
	for (int row = 0; row < m_nGridRows; row++)
	{
		for (int col = 0; col < m_nGridCols; col++)
		{
			delete m_pTiles[((row*m_nGridCols) + col)];
			m_pTiles[((row*m_nGridCols) + col)] = nullptr;
		}
	}

	delete m_pTileset;
	m_pTileset = nullptr;
}

void Map::InitializeMap()
{
	for (int row = 0; row < m_nGridRows; row++)
	{
		for (int col = 0; col < m_nGridCols; col++)
		{
			Tile* temp = new Tile();
			temp->m_nID = -1;
			temp->type = Tile::hole;

			temp->SetPosition(SGD::Point((float)col*m_pTileset->GetTileWidth()
				, (float)row*m_pTileset->GetTileHeight()));

			m_pTiles.push_back(temp);
		}
	}
}

void Map::InitializeTileset(const char* image,int cols, int rows, int width, int height)
{
	m_pTileset = new TileSet(image,cols,rows,width,height);
}

void Map::SetTile(int col, int row, int id, Tile::m_eTiletype type)
{
	m_pTiles[((row*m_nGridCols) + col)]->SetPosition(
		SGD::Point((float)col*GetGridWidth()
		,(float)row*GetGridHeight()));

	m_pTiles[((row*m_nGridCols) + col)]->SetSize(SGD::Size{(float)m_pTileset->GetTileWidth()
		, (float)m_pTileset->GetTileHeight() });
	
	m_pTiles[((row*m_nGridCols) + col)]->m_nID = id;
	m_pTiles[((row*m_nGridCols) + col)]->type = type;

	//test code
	//if (col == 1 && row == 1)
		//m_pTiles[((row*m_nGridCols) + col)]->type = Tile::m_eTiletype::faketrap;

	switch (m_pTiles[((row*m_nGridCols) + col)]->type)
	{
	case Tile::m_eTiletype::firetrap:
	{
		Entity* trap = GamePlayState::GetInstance()->CreateFireTrap(m_pTiles[((row*m_nGridCols) + col)]);
		trap->Release();
		break;
	}

	case Tile::m_eTiletype::electrictrap:
	{
		Entity* trap = GamePlayState::GetInstance()->CreateElectricTrap(m_pTiles[((row*m_nGridCols) + col)]);
		trap->Release();
		break;
	}

	case Tile::m_eTiletype::spiketrap:
	{
		Entity* trap = GamePlayState::GetInstance()->CreateSpikeTrap(m_pTiles[((row*m_nGridCols) + col)]);
		trap->Release();
		break;
	}

	case Tile::m_eTiletype::faketrap:
	{
		Entity* trap = GamePlayState::GetInstance()->CreateFakeTrap(m_pTiles[((row*m_nGridCols) + col)]);
		trap->Release();
		break;
	}

	case Tile::m_eTiletype::spawntrap:
	{
		Entity* trap = GamePlayState::GetInstance()->CreateSpawnTrap(m_pTiles[((row*m_nGridCols) + col)]);
		trap->Release();
		break;
	}

	default:
		break;
	}

}

void Map::Update(float elapsedTime)
{
	for (int row = 0; row < m_nGridRows; row++)
	{
		for (int col = 0; col < m_nGridCols; col++)
		{
			/*SGD::Point pos;
			pos.x = m_pTiles[((row*m_nGridCols) + col)]->GetPosition().x;
			pos.y = m_pTiles[((row*m_nGridCols) + col)]->GetPosition().y;

			if (pos.x < Game::GetInstance()->GetScreenWidth() && pos.y < Game::GetInstance()->GetScreenHeight())*/
			m_pTiles[((row*m_nGridCols) + col)]->Update(elapsedTime);
		}
	}
}

void Map::Render()
{
	for (int row = 0; row < m_nGridRows; row++)
	{
		for (int col = 0; col < m_nGridCols; col++)
		{
			SGD::Point pos;
			pos.x = m_pTiles[((row*m_nGridCols) + col)]->GetPosition().x + Game::GetInstance()->GetWorldSpace().x;
			pos.y = m_pTiles[((row*m_nGridCols) + col)]->GetPosition().y + Game::GetInstance()->GetWorldSpace().y;

			if (Game::GetInstance()->GetWorldCamera().IsIntersecting(m_pTiles[((row*m_nGridCols) + col)]->GetRect()))
				m_pTileset->DrawTile(m_pTiles[((row*m_nGridCols) + col)]->m_nID, pos);
		}
	}
}

void Map::CheckCollision(const IEntity* pOther)
{
	for (int row = 0; row < m_nGridRows; row++)
	{
		for (int col = 0; col < m_nGridCols; col++)
		{
			if (pOther != nullptr)
			{
				if (m_pTiles[((row*m_nGridCols) + col)]->GetRect().IsPointInRectangle(pOther->GetRect().ComputeCenter()))
				{
					m_pTiles[((row*m_nGridCols) + col)]->HandleCollision(pOther);
				}
			}
		}
	}
}

bool Map::CheckCollision(SGD::Point point, const IEntity* pOther)
{
	Player* ply = (Player*)pOther;
	SGD::Rectangle rect = { point, ply->GetSize() };
	bool move = true;
	for (int row = 0; row < m_nGridRows; row++)
	{
		for (int col = 0; col < m_nGridCols; col++)
		{
			if (m_pTiles[((row*m_nGridCols) + col)]->GetRect().IsIntersecting(rect) == true && pOther->GetType() == Entity::EntityType::ENT_PLAYER && 
				m_pTiles[((row*m_nGridCols) + col)]->GetType() == Tile::m_eTiletype::collidable)
			{
				return false;
			}

			if (m_pTiles[((row*m_nGridCols) + col)]->GetRect().IsIntersecting(rect) == true && pOther->GetType() == Entity::EntityType::ENT_PLAYER &&
				m_pTiles[((row*m_nGridCols) + col)]->GetType() == Tile::m_eTiletype::door)
			{
				Player* mypla = (Player*)pOther;
				if (mypla->GetHasKey())
				{
					m_pTiles[((row*m_nGridCols) + col)]->walkable;
					//m_bActive = false;
					mypla->SetHasKey(false);
				}
				return false;
			}

		}
	}
	return true;
}