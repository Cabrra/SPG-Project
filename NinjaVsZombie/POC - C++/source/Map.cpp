#include "Map.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Geometry.h"

Map::Map(int rows, int cols, int width, int height)
{
	m_nGridRows = rows;
	m_nGridCols = cols;
	m_nGridWidth = width;
	m_nGridHeight = height;

}


Map::~Map()
{
	for (int col = 0; col < m_nGridCols; col++)
	{
		for (int row = 0; row < m_nGridRows; row++)
		{
			delete m_pTiles[((row*m_nGridCols)+col)];
			m_pTiles[((row*m_nGridCols) + col)] = nullptr;
		}
	}

	delete m_pTileset;
	m_pTileset = nullptr;
}

void Map::InitializeMap()
{
	for (int col = 0; col < m_nGridCols; col++)
	{
		for (int row = 0; row < m_nGridRows; row++)
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

	//m_pTiles[((row*m_nGridCols) + col)]->SetImage(m_pTileset->GetImage());
}

void Map::Update(float elapsedTime)
{
	for (int col = 0; col < m_nGridCols; col++)
	{
		for (int row = 0; row < m_nGridRows; row++)
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
	for (int col = 0; col < m_nGridCols; col++)
	{
		for (int row = 0; row < m_nGridRows; row++)
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
	for (int col = 0; col < m_nGridCols; col++)
	{
		for (int row = 0; row < m_nGridRows; row++)
		{
			SGD::Rectangle tileRect;
			tileRect.left = float(col * m_pTileset->GetTileWidth());
			tileRect.top = float(row * m_pTileset->GetTileHeight());
			tileRect.right = tileRect.left + m_pTileset->GetTileWidth();
			tileRect.bottom = tileRect.top + m_pTileset->GetTileHeight();


			if (pOther->GetRect().IsIntersecting(tileRect) && m_pTiles[((row*m_nGridCols) + col)]->type == Tile::m_eTiletype::trigger)
				m_pTiles[((row*m_nGridCols) + col)]->HandleCollision(pOther);
		}
	}
}