#include "TileSet.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

TileSet::TileSet(const char* image,int cols, int rows, int width, int height)
{
	m_htTileSetImage = SGD::GraphicsManager::GetInstance()->LoadTexture(image);
	m_nTileWidth = width;
	m_nTileHeight = height;
	m_nNumRows = rows;
	m_nNumCols = cols;

	int val = m_nNumRows*m_nNumCols;

	for (int i = 0; i < val; i++)
	{
		Tile* tempTile = new Tile();
		m_pTiles.push_back(tempTile);
	}
	
	SetTileSetIds(val);
}


TileSet::~TileSet()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_htTileSetImage);
	int val = m_nNumRows*m_nNumCols;

	for (int x = 0; x < val; x++)
	{
		delete m_pTiles[x];
	}
	m_pTiles.clear();
}

void TileSet::SetTileSetIds(int tilecount)
{
	for (int x = 0; x < tilecount; x++)
	{
		m_pTiles[x]->m_nID = x;
	}
}

void TileSet::DrawTile(int id,SGD::Point position)
{
	if (id < 0)
		return;

	SGD::Rectangle cell;
	cell.left = float((id % m_nNumCols) * (m_nTileWidth));
	cell.top = float((id / m_nNumCols) * (m_nTileHeight));
	cell.right = cell.left + m_nTileWidth;
	cell.bottom = cell.top + m_nTileHeight;

	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_htTileSetImage, position,
		cell, 0.0f, {},
		SGD::Color(255,255,255,255), { 1.0, 1.0 });
}